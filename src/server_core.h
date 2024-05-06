/**
 * @file server_core.h
 * @brief Core Server Functionality
 *
 * This file defines the core data structures and functions required for
 * managing server interactions.
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
#ifndef __SERVER_CORE_H
#define __SERVER_CORE_H

#include "networktcp.h"
#include "globals.h" /* for max arguments */
#include <unistd.h> /* for pid_t */

#define MAX_LOGIN_ATTEMPTS 3

struct MyIO;

/**
 * @struct ServerData
 * @brief Holds data specific to the server's operation.
 *
 * @var ServerData::io
 * Pointer to a MyIO structure containing I/O parameters.
 * [TODO]: Consider refactoring this.
 *
 * @var ServerData::bindfd
 * Socket file descriptor used for binding the server to a port.
 *
 * @var ServerData::port
 * Port number where the server listens, represented as a string.
 *
 * @var ServerData::greeting
 * A greeting string that could be sent to the client upon connection.
 *
 * @var ServerData::commandlist
 * List of supported commands as a string.
 *
 * @var ServerData::readbuf
 * Buffer used for reading data from the client. Sized to NETREADMAX + 1 to account for null-terminator.
 *
 * @var ServerData::runflag
 * A flag to control whether the server continues running.
 *
 * @var ServerData::outfd
 * File descriptor for output, possibly used for logging server actions.
 */
typedef struct _ServerData {
  struct MyIO *io; /* TODO: clean this up */
  int bindfd;
  const char *port;
  const char *greeting;
  const char *commandlist;
  char readbuf[NETREADMAX+1];
  int runflag;
  int outfd;
}ServerData;

/**
 * @struct ClientData
 * @brief Holds client-related data.
 *
 * @ClientData.clientfd
 * Client socket descriptor.
 * @ClientData.clientid
 * Client identifier.
 * @ClientData.userindex
 * entry for usertable
 */
typedef struct _ClientData {
  int clientfd, clientid, userindex;
} ClientData;

/**
 * initserver() - Initializes server data structure.
 *
 * @server: Pointer to ServerData structure.
 */
void initserver(ServerData * const server)
  __attribute__((__nonnull__(1)));

/**
 * runserver() - Main server loop, handles client connections and server shutdown.
 *
 * @server: Pointer to ServerData structure.
 */
void runserver(ServerData * const server)
  __attribute__((__nonnull__(1)));

/**
 * handleexit() - Sets run flag based on received data.
 *
 * @server: Pointer to ServerData structure.
 */
void handleexit(ServerData * const server)
  __attribute__((__nonnull__(1)));

/**
 * acceptandforkclient() - Waits for a client connection and returns client process ID.
 *
 * @client: Pointer to ClientData structure.
 * @server: Pointer to ServerData structure.
 *
 * Return: Process ID of the client.
 */
pid_t acceptandforkclient(ClientData * const client, const ServerData * const server)
  __attribute__((__nonnull__(1, 2)));

/**
 * closeclientfd() - Closes client connection and logs the disconnection.
 *
 * @client: Pointer to ClientData structure.
 * @server: Pointer to ServerData structure.
 */
void closeclientfd(ClientData * const client, const ServerData * const server)
  __attribute__((__nonnull__(1, 2)));

/**
 * handleclient() - Handles client communication in a loop.
 *
 * @client: Pointer to ClientData structure.
 * @server: Pointer to ServerData structure.
 */
void handleclient(ClientData * const  client, ServerData * const server)
  __attribute__((__nonnull__(1, 2)));

void send_greeting(ClientData * const client, ServerData * const server)
  __attribute__((__nonnull__(1, 2)));

void send_login_result(ClientData * const client, int userindex)
  __attribute__((__nonnull__(1)));

int attempt_login(ClientData * const client, ServerData * const server)
  __attribute__((__nonnull__(1, 2)));

/**
 * send_recv_log() - Sends a prompt to the client, reads the client's response, and logs the interaction.
 *
 * @send_data Pointer to a constant char, the data to be sent to the client.
 * @client Pointer to a constant ClientData structure, holding client-related information.
 * @server Pointer to ServerData structure, holding server-related information.
 * Return: Pointer to a char, the client's response.
 */
char * const
send_recv_log(const char * const send_data,
              const ClientData * const client,
              ServerData * const server)
  __attribute__((__nonnull__(1, 2, 3)));
/* TODO: FIX THIS IF YOU HAVE TIME */
char * const
send_recv_log_io(const char * const send_data,
                 const ClientData * const client,
                 ServerData * const server)
  __attribute__((__nonnull__(1, 2, 3)));
/* [TODO] pipeline.h ? */

#endif // SERVER_CORE_H
