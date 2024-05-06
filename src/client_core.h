/**
 * @file client_core.h
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
#ifndef __CLIENT_CORE_H
#define __CLIENT_CORE_H

#define DEFAULT_PORT "1234"
#define DEFAULT_IP "127.0.0.1"

#include "globals.h"
#include "filetransfer.h"

/**
 * struct ClientContext - Container for client I/O buffers and socket.
 * @readbuf: Buffer for incoming data.
 * @writebuf: Buffer for outgoing data.
 * @sizebuffers: Size of the read and write buffers.
 * @sockfd: Socket file descriptor for the client.
 *
 * This struct is a container for the client-side I/O buffers and the socket
 * file descriptor.
 */
struct ClientContext {
  char readbuf[MAX_DATA_SIZE + 1];
  char writebuf[MAX_DATA_SIZE + 1];
  int sizebuffers;
  int sockfd;
};

/**
 * do_connect_server - Connects to a server and returns the socket descriptor.
 * @ip: IP address to connect to.
 * @port: Port number to connect to.
 *
 * Resolves the address info for the given IP and port, then attempts to connect.
 * Returns the socket file descriptor.
 */
int do_connect_server(const char * const ip, const char * const port)
  __attribute__((__nonnull__(1, 2)));

/**
 * initclient - Initializes client with server connection and I/O buffer.
 * @argc: Argument count from main.
 * @argv: Argument vector from main.
 * @io: Pointer to struct containing I/O buffer and socket info.
 *
 * Initializes a client connection to a server, either using the IP and port
 * provided as arguments, or defaults. Also initializes I/O buffers.
 */
void initclient(int argc, char *argv[], struct MyIO *io)
  __attribute__((__nonnull__(2, 3)));

/**
 * runclient - Main loop for the client to handle data transfer.
 * @io: Pointer to struct containing I/O buffer and socket info.
 *
 * This function sits in a loop, waiting for data to become available on
 * the client's socket. When data is available, it performs the necessary
 * read and write operations.
 */
void runclient(struct MyIO *io)
  __attribute__((__nonnull__(1)));

/**
 * do_poll - Waits for incoming data on a socket using poll.
 * @sockfd: The file descriptor for the socket to poll.
 *
 * This function uses the poll system call to wait for incoming data on a socket.
 * If poll encounters an error, it prints an error message and exits.
 */
void do_poll(int sockfd);

#endif /* END __CLIENT_CORE_H */

