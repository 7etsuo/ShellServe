/**
 * @file client_core.c
 * @brief Core Client Functionality
 *
 * This file defines the core data structures and functions required for
 * managing client interactions.
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
#include <poll.h>
#include <unistd.h> /* _exit */

#include "client_core.h"
#include "globals.h"
#include "mystring.h"
#include "syscalls.h"
#include "networktcp.h"
#include "filetransfer.h"

void
do_poll(int sockfd)
{
  int pollval;

  struct pollfd fds[1];
  fds[0].fd = sockfd;
  fds[0].events = POLLIN;

 polltag:
  pollval = poll(fds, 1, 0);
  if (pollval == -1) {
    myfprintf(sys_stderr, "poll() error");
    _exit(0);
  } else if (pollval == 0) {
    goto polltag; /* timer ran out should not happen since we have it at 0 */
  }
}

void
initclient(int argc, char *argv[], struct MyIO *io)
{
  io->sockfd = do_connect_server (
                                  argc == 2 ? argv[1] : DEFAULT_IP,
                                  argc == 2 ? argv[2] : DEFAULT_PORT
                                  );

  io->bufsize = MAX_DATA_SIZE;
  mymemset(io->buf, 0, io->bufsize);
  initiostruct(io->sockfd, sys_stdin, sys_stdout, io); /* TODO: REMOVE but FIX */
}

int
do_connect_server(const char * const ip, const char * const port)
{
  struct addrinfo *ai;
  mygetaddrinfo(port, ip, &ai);
  return sckconnect(ai);
}

void
runclient(struct MyIO *io)
{
  size_t nsent;
  void(*callbacks[NCALLBACK])(struct MyIO*) = {
    clienthandleget,
    clienthandleput,
    clienthandlehelp,
    clienthandleexit,
  };

  do {
    mymemset(io->buf, 0, io->bufsize);
    do_poll(io->sockfd);

    readsocket_writefd(io->sockfd, io->buf, io->bufsize, io->writefd);
    nsent = readfd_writesocket(io->sockfd, io->buf, io->bufsize, io->readfd, 0);
    io->buf[nsent-1] = '\0';
    runfiletransfer(io, callbacks);
  } while (1);
}
