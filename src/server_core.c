/**
 * @file server_core.c
 * @brief Core Server Functionality
 *
 * This file defines the core data structures and functions required for
 * managing server and client interactions.
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
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "clientlogin.h"
#include "globals.h"
#include "networktcp.h"
#include "mystring.h"
#include "syscalls.h"
#include "signals.h"
#include "server_core.h"
#include "clientlogin.h"
#include "command_handler.h"
#include "pipeline.h"
#include "filetransfer.h"

const char *const greeting = "Welcome to MyFTP Server!\n";
const char *const port = "1234";
const char *const prompt = "server> ";

void
initserver(ServerData * const server)
{
  server->port     = port;
  server->greeting = greeting;
  server->bindfd   = initservergetsock(server->port);
  server->runflag  = 1;
  server->outfd    = sys_stdout; /* for logging */
  mymemset(server->readbuf, 0, NETREADMAX);
}

pid_t
acceptandforkclient(ClientData * const client, const ServerData * const server)
{
  static int clientid = 0;
  clientid += 1;
  client->clientfd = myaccept(server->bindfd);
  client->clientid = clientid;

  myfprintf(server->outfd, "::client %d connected\n", client->clientid);

  return myfork();
}

void closeclientfd(ClientData * const client, const ServerData * const server)
{
  close(client->clientfd);
  myfprintf(server->outfd, "::client %d disconnected\n", client->clientid);
  _exit(1);
}

char * const
send_recv_log_io(const char * const send_data,
                 const ClientData * const client,
                 ServerData * const server)
{
  fdputs(server->io->sockfd, send_data);
  mymemset(server->io->buf, 0, server->io->bufsize-1);
  myreadline(server->io->sockfd, server->io->buf, server->io->bufsize-1);
  myfprintf(server->io->writefd, "::client %d sent %s\n", client->clientid, server->io->buf);

  return server->io->buf;
}

char * const
send_recv_log(const char * const send_data,
              const ClientData * const client,
              ServerData * const server)
{
  fdputs(client->clientfd, send_data);
  mymemset(server->readbuf, 0, NETREADMAX-1);
  myreadline(client->clientfd, server->readbuf, NETREADMAX-1);
  myfprintf(server->outfd, "::client %d sent %s\n", client->clientid, server->readbuf);

  return server->readbuf;
}

void
send_greeting(ClientData * const client, ServerData * const server)
{
  fdputs(client->clientfd, server->greeting);
}

void
send_login_result(ClientData * const client, int userindex)
{
  if (userindex != -1) {
    myfprintf(client->clientfd, "welcome back %s\n", get_username_at_index(client->userindex));
  } else {
    myfprintf(client->clientfd, "login failed\n");
  }
}

int
attempt_login(ClientData * const client, ServerData * const server)
{
  int userindex;
  char username[MAX_USER_NAME],password[MAX_USER_NAME], * cptr;

  cptr = send_recv_log("Username: ", client, server);
  mystrncpy(username, cptr, MAX_USER_NAME-1);

  cptr = send_recv_log("Password: ", client, server);
  mystrncpy(password, cptr, MAX_USER_NAME-1);

  userindex = verifyuser(username, password);
  if (userindex == -1) {
    myfprintf(server->outfd, "::client %d failed password attempt\n", client->clientid);
  }

  return userindex;
}

void
do_login(ClientData * const client, ServerData * const server)
{
  send_greeting(client, server);
  client->userindex = -1;
  for (int nlogin = 0; nlogin < MAX_LOGIN_ATTEMPTS && client->userindex == -1; nlogin++) {
    client->userindex = attempt_login(client, server);
  }

  send_login_result(client, client->userindex);

  server->runflag = client->userindex != -1;
}

void
handleclient(ClientData * const client, ServerData * const server)
{
  struct MyIO io;

  initiostruct(client->clientfd, sys_stdout, sys_stdout, &io);
  server->io = &io;


  void(*callbacks[NCALLBACK])(struct MyIO*) = {
    serverhandleget,
    serverhandleput,
    serverhandlehelp,
    serverhandleexit,
  };

  while(server->runflag) {
    fflush(NULL);
    send_recv_log_io(prompt, client, server);
    if (runfiletransfer(server->io, callbacks)) {
      runcommand(client, server->io->buf);
    }
  }
}

void
runserver(ServerData * const server)
{
  ClientData client;

  myfprintf(server->outfd, "::server up\n");
  while (server->runflag) {
    if (acceptandforkclient(&client, server) == 0) { /* we are in a child process */
      myclose(server->bindfd); /* we dont need this in the child */
      do_login(&client, server);
      handleclient(&client, server);
      closeclientfd(&client, server);
    }
    myclose(client.clientfd); /* close client socket in parent don't need it */
  }
  myfprintf(server->outfd, "::server down\n");
}

