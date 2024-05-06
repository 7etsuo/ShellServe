/**
 * @file networktcp.h
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
#ifndef __NETWORKTCP_H
#define __NETWORKTCP_H

#include "globals.h"

#define NETREADMAX MAX_DATA_SIZE
#define BACKLOG 0x10

#include <netdb.h>

/**
 * mygetaddrinfo() - Wrapper for getaddrinfo with custom handling.
 *
 * @port: Port number as a string.
 * @ip: IP address as a string.
 * @ai: Double pointer to address info structure.
 */
void mygetaddrinfo(const char *const port, const char *const ip, struct addrinfo **ai)
  __attribute__((__nonnull__(1, 3)));

/**
 * mysocket() - Wrapper for socket creation.
 *
 * @ai: Pointer to address info structure.
 *
 * Return: Socket descriptor.
 */
int mysocket(struct addrinfo *ai)
  __attribute__((__nonnull__(1)));

/**
 * mybind() - Wrapper for bind with custom handling.
 *
 * @sck: Socket descriptor.
 * @ai: Pointer to address info structure.
 *
 * Return: Result of bind call.
 */
int mybind(int sck, struct addrinfo *ai)
  __attribute__((__nonnull__(2)));

/**
 * mylisten() - Wrapper for listen.
 *
 * @sck: Socket descriptor.
 * @backlog: Backlog value.
 *
 * Return: Result of listen call.
 */
int mylisten(int sck, int backlog)
  __attribute__((__pure__));

/**
 * bindscklisten() - Binds a socket and sets it to listen.
 *
 * @ai: Pointer to address info structure.
 *
 * Return: Bound socket descriptor.
 */
int bindscklisten(struct addrinfo *ai)
  __attribute__((__nonnull__(1)));

/**
 * initservergetsock() - Initializes server and gets socket.
 *
 * @port: Port number as a string.
 *
 * Return: Socket descriptor.
 */
int initservergetsock(const char *const port)
  __attribute__((__nonnull__(1), __pure__));

/**
 * sckconnect() - Establishes a connection using address info.
 *
 * @ai: Pointer to address info structure.
 *
 * Return: Socket descriptor of the connection.
 */
int sckconnect(struct addrinfo *ai)
  __attribute__((__nonnull__(1)));

/**
 * myaccept() - Wrapper for accept with custom handling.
 *
 * @bindfd: Socket descriptor of binding socket.
 *
 * Return: Socket descriptor of new client connection.
 */
int myaccept(int bindfd)
  __attribute__((__pure__));

/**
 * readfd_writesocket()
 */
size_t readfd_writesocket(int sockfd, char * buf, size_t sizebuf, int readfd, int eofflag)
  __attribute__((nonnull(2)));

/**
 * readsocket_writefd()
 */
size_t readsocket_writefd(int sockfd, void *buf, size_t sizebuf, int writefd)
  __attribute__((nonnull(2)));

#endif // __NETWORKTCP_H

