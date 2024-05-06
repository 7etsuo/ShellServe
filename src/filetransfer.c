/**
 * @file filetransfer.c
 *
 * @author 7etsuo
 * @date 2023
 *
 * Copyright (C) 2023 7etsuo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/limits.h>

#include "globals.h"
#include "networktcp.h"
#include "server_core.h"
#include "filetransfer.h"
#include "mystring.h"
#include "syscalls.h"
#include "client_core.h"

const char *const commandlist = "put\nget\ndel\nhelp\n";

void
initiostruct(int sockfd, int readfd, int writefd, struct MyIO *io)
{
  io->sockfd = sockfd;
  io->readfd = readfd;
  io->writefd = writefd;
  io->bufsize = MAX_DATA_SIZE;
  mymemset(io->buf, 0, io->bufsize);
}

int
openfile_getfd_fromclient(struct MyIO *io, int flags, int mode)
{
  myfprintf(io->sockfd, "filename: ");
  fflush(NULL);
  do_poll(io->sockfd);
  myreadline(io->sockfd, io->buf, io->bufsize-1);

  return mode == 0 ? myopenfile(io->buf, flags) : myopen(io->buf, flags, mode);
}

void
sendfile_tosocket(struct MyIO *io)
{
  size_t nread;
 sendmore:
  nread = readfd_writesocket(io->sockfd, io->buf, NETREADMAX, io->readfd, 0);
  if (nread == NETREADMAX-1) {
    goto sendmore;
  }
  fflush(NULL);
}

/* server FTP function */
void
serverhandleget(struct MyIO *io)
{
  int oldfd = io->readfd;

  myfprintf(io->writefd, "client:: get\n");
  io->readfd = openfile_getfd_fromclient(io, O_RDONLY, 0);
  sendfile_tosocket(io);

  /* wait for send confirmation */
  myreadline(io->sockfd, io->buf, io->bufsize-1);
  closereadfd_restoreoldfd(oldfd, io);
}

void
serverhandleput(struct MyIO *io)
{
  int oldfd = io->writefd;
  myfprintf(io->writefd, "client:: put\n");
  io->writefd = openfile_getfd_fromclient(io, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  readbytes_fromsocket(io, NETREADMAX-1);
  closewritefd_restoreoldfd(oldfd, io);
}

void
serverhandlehelp(struct MyIO *io)
{
  myfprintf(io->readfd, "client:: help\n");
  mysckwrite(io->sockfd, commandlist, mystrlen(commandlist));
}

void
serverhandleexit(struct MyIO *io)
{
  myfprintf(io->writefd, "client:: exit\n");
  _exit(0); /* TODO: Do this cleanly */
}

void
init_clienthandleget(char *savename, struct MyIO *io, size_t savenamelen)
{
  /* init clienthandleget */
  mymemset(savename, 0, savenamelen);
  mymemset(io->buf, 0, io->bufsize);
  do_poll(io->sockfd);
}

void
read_prompt_clienthandle(struct MyIO *io)
{
  /* get the clienthandle prompt */
  do_poll(io->sockfd);
  myreadline(io->sockfd, io->buf, io->bufsize-1);
  myfprintf(io->writefd, io->buf);
}

int
create_savefile_getfd(char *savename, struct MyIO *io)
{
  /* build the save name string */
  mystrcpy(savename, io->buf);
  mystrcat(savename, ".newsave");

  /* make a new filename so we dont overwrite the old file
   * sets mode to +rw,ugo
   **/
  return myopen(savename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

void
send_filename_toserver(struct MyIO *io, int sizename)
{
  /* fix the filename (we need to send this back to the server) */
  io->buf[sizename-1] = '\n';
  io->buf[sizename] = '\0';
  mysckwrite(io->sockfd, io->buf, sizename);
}

void
readbytes_fromsocket(struct MyIO *io, size_t szmax)
{
  size_t bytesread;
 recvmore:
  /* read the data from the network write it to disk */
  do_poll(io->sockfd); /* queue up the data */
  bytesread = readsocket_writefd(io->sockfd, io->buf, io->bufsize, io->writefd);
  if (bytesread == szmax) {
    mymemset(io->buf, 0, io->bufsize);
    goto recvmore;
  }
  /* let the server know we have finished reading the file */
  mysckwrite(io->sockfd, "\n", 2);
}

void
closereadfd_restoreoldfd(int oldfd, struct MyIO *io)
{
  close(io->readfd);
  io->readfd = oldfd;
}

void
closewritefd_restoreoldfd(int oldfd, struct MyIO *io)
{
  close(io->writefd);
  io->writefd = oldfd;
}

void
getfile_fromserver(char *savename, struct MyIO *io, size_t maxread)
{
  size_t nread;
  int oldfd = io->writefd; /* store old writefd */

  /* get the file name from the user */
  nread = myreadline(io->readfd, io->buf, io->bufsize-1);
  io->writefd = create_savefile_getfd(savename, io);
  send_filename_toserver(io, nread);
  readbytes_fromsocket(io, maxread);

  closewritefd_restoreoldfd(oldfd, io);
}

void
sendfile_toserver(struct MyIO *io, size_t maxread)
{
  size_t nread;
  int oldfd = io->readfd;
  char file[PATH_MAX];

  /* get the file name from the user */
  nread = myreadline(io->readfd, io->buf, maxread);
  getcwd(file, PATH_MAX);
  mystrcat(file, "/");
  mystrcat(file, io->buf); /* [TODO]  this could overflow fix */
  send_filename_toserver(io, nread);

  io->readfd = myopenfile(file, O_RDONLY);
  sendfile_tosocket(io);
  closereadfd_restoreoldfd(oldfd, io);
}

void
clienthandleget(struct MyIO *io)
{
  char savename[NETREADMAX+1];

  do_poll(io->sockfd);
  init_clienthandleget(savename, io, NETREADMAX+1);
  read_prompt_clienthandle(io);
  getfile_fromserver(savename, io, NETREADMAX-1);
}

void
clienthandleput(struct MyIO *io)
{
  read_prompt_clienthandle(io);
  sendfile_toserver(io, NETREADMAX-1);
}

void
clienthandlehelp(struct MyIO *io)
{
  ;
}

void
clienthandleexit(struct MyIO *io)
{
  _exit(0);
}

int
runfiletransfer(struct MyIO *io, void(*ftpcallback[NCALLBACK])(struct MyIO*))
{
  int ran_custom_command_flag = 1;

  if ((ran_custom_command_flag = mystrcmp(io->buf, "get")) == 0) {
    ftpcallback[GET](io);
  } else if((ran_custom_command_flag = mystrcmp(io->buf, "put")) == 0) {
    ftpcallback[PUT](io);
  } else if((ran_custom_command_flag = mystrcmp(io->buf, "help")) == 0) {
    ftpcallback[HELP](io);
  } else if((ran_custom_command_flag = mystrcmp(io->buf, "exit")) == 0) {
    ftpcallback[EXIT](io);
  }

  return ran_custom_command_flag;
}

