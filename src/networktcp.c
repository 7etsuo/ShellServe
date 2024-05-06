/**
 * @file networktcp.c
 * @brief TCP Network Communication Library
 *
 * This file declares functions for creating and managing TCP connections,
 * including socket creation, binding, listening, and accepting new connections.
 * The library provides a level of abstraction over the standard socket API,
 * with additional error handling and convenience features.*
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
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "syscalls.h"
#include "networktcp.h"
#include "mystring.h"
#include "globals.h"

void
exitfreeaddr(const char * const err, struct addrinfo *ai)
{
  myputs(err);
  if(ai) {
    freeaddrinfo(ai);
  }
  _exit(1);
}

int
initservergetsock(const char *const port)
{
  int bsck;
  struct addrinfo *ai;

  mygetaddrinfo(port, NULL, &ai);
  bsck = bindscklisten(ai);
  freeaddrinfo(ai);

  return bsck;
}

void
mygetaddrinfo(const char *const port, const char *const ip, struct addrinfo **ai)
{
  struct addrinfo hints;              /* template struct to fill out */

  mymemset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;          /* IPV4 */
  hints.ai_socktype = SOCK_STREAM;    /* TCP */
  if (!ip) {
    hints.ai_flags = AI_PASSIVE;    /* fill in ip */
  }

  if(getaddrinfo(ip, port, &hints, ai)) {
    myputs("getaddrinfo error");
    _exit(1);
  }
}

int
mysocket(struct addrinfo *ai)
{
  return socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
}

int
mybind(int sck, struct addrinfo *ai)
{
  int yes=1;

  if (setsockopt(sck,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
    close(sck);
    exitfreeaddr("setsockopt", ai);
  }

  return bind(sck, ai->ai_addr, ai->ai_addrlen);
}

int
sckconnect(struct addrinfo *ai)
{
  int sockfd;
  struct addrinfo *p;
  for(p = ai; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      continue;
    }

    break;
  }

  if (p == NULL) {
    exitfreeaddr("connection failure", ai);
  }

  fdputs(sys_stderr, "client: connected\n");
  freeaddrinfo(ai);

  return sockfd;
}

int
bindscklisten(struct addrinfo *ai)
{
  int bsck;

  for(struct addrinfo *p = ai; p != NULL; p = p->ai_next) {
    bsck = mysocket(ai);
    if(bsck == -1) continue;

    if(mybind(bsck, ai) == 0) break;
  }

  if (mylisten(bsck, BACKLOG) == -1) {
    close(bsck);
    exitfreeaddr("listen error", ai);
  }

  return bsck;
}

int
mylisten(int sck, int backlog)
{
  return listen(sck, backlog);
}

int
myaccept(int bindfd)
{
  int clientfd;

  struct sockaddr_storage clientaddr;
  socklen_t clientsize = sizeof(clientaddr);

  clientfd = accept(bindfd, (struct sockaddr*)&clientaddr, &clientsize);
  if (clientfd == -1) {
    myclose(bindfd);
    printerr_exit("accept error");
  }

  return clientfd;
}

size_t
readsocket_writefd(int sockfd, void *buf, size_t sizebuf, int writefd)
{
  size_t total_written, nread, nwritten;
  char *cbuf = (char *) buf;

  while ((nread = mysckread(sockfd, cbuf, sizebuf-1)) > 0) {
    total_written = 0;
    while (total_written < nread) {
      nwritten = mywrite(writefd, cbuf + total_written, nread - total_written);
      if (nwritten > 0) {
        total_written += nwritten;
      }
    }
    mymemset(buf, 0, sizebuf);
  }
  return total_written;
}

size_t
readfd_writesocket(int sockfd, char * buf, size_t sizebuf, int readfd, int eofflag)
{
  size_t total_sent, nread, nwritten;
  char *cbuf = (char *) buf;

  /* TODO: handle EOF for files */
  while ((nread = myread(readfd, cbuf, sizebuf-1)) > 0) {
    total_sent = 0;
    while (total_sent < nread) {
      nwritten = mysckwrite(sockfd, cbuf + total_sent, nread - total_sent);
      if (nwritten > 0) {
        total_sent += nwritten;
      }
    }
    if (!eofflag) {
      break;
    }
    mymemset(buf, 0, sizebuf);
  }
  return total_sent;
}

