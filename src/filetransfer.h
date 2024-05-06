/**
 * @file filetransfer.h
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
#ifndef __FILE_TRANSFER_H
#define __FILE_TRANSFER_H

#include "globals.h"

/**
 * struct MyIO - Structure for encapsulating I/O operations
 * @sockfd:   Socket file descriptor for network operations
 * @readfd:   File descriptor for read operations
 * @writefd:  File descriptor for write operations
 * @buf:      Buffer for storing data temporarily
 * @bufsize:  Size of the buffer
 *
 * This structure is a collection of various I/O parameters required
 * for reading from and writing to files and sockets.
 */
struct MyIO {
  int sockfd, readfd, writefd;
  char buf[MAX_DATA_SIZE];
  size_t bufsize;
};

/**
 * enum FTPCALLBACK - Enumerates available FTP commands
 * @GET:    Command for getting files
 * @PUT:    Command for putting files
 * @HELP:   Command for displaying help
 * @EXIT:   Command for exiting
 * @NCALLBACK: Sentinel value for the number of FTP commands
 *
 * This enumeration lists all FTP commands that can be handled by the system.
 */
enum FTPCALLBACK {
  GET,
  PUT,
  HELP,
  EXIT,
  NCALLBACK,
};

/**
 * typedef ftpcallback - Array of function pointers for FTP commands
 * @MyIO*: A pointer to the MyIO structure containing the necessary I/O parameters
 *
 * Defines an array of function pointers indexed by FTPCALLBACK enum values.
 * Each function handles a specific FTP command.
 */
typedef void(*ftpcallback[NCALLBACK])(struct MyIO*);

typedef struct _ServerData ServerData;
typedef struct _ClientData ClientData;

/**
 * runfiletransfer() - Run the FTP operation based on the command
 * @io:         Pointer to MyIO structure
 * @ftpcallback: Array of function pointers for handling FTP commands
 *
 * Checks the command received and runs the corresponding FTP function.
 *
 * Return: Flag indicating whether a custom command was run
 */
int runfiletransfer(struct MyIO *io, void(*ftpcallback[NCALLBACK])(struct MyIO*))
  __attribute__((__nonnull__(1,2)));

/**
 * initiostruct() - Initialize an I/O structure with the given parameters
 * @sockfd:  The socket file descriptor
 * @readfd:  File descriptor for reading
 * @writefd: File descriptor for writing
 * @io:      Pointer to the MyIO structure to initialize
 *
 * This function initializes the MyIO structure with the given parameters,
 * sets the buffer size to MAX_DATA_SIZE and clears the buffer.
 */
void initiostruct(int sockfd, int readfd, int writefd, struct MyIO *io)
  __attribute__((__nonnull__(4)));

/**
 * openfile_getfd_fromclient() - Obtain file descriptor from client-side
 * @io: Pointer to the MyIO structure containing buffer information
 * @flags: file open flags
 * @mode: file open mode 0 for none
 *
 * The function prompts the client for a filename and returns its file descriptor.
 *
 * Return: File descriptor associated with the file
 */
int openfile_getfd_fromclient(struct MyIO *io, int flags, int mode)
  __attribute__((__nonnull__(1)));

/**
 * sendfile_toclient() - Send a file to the client
 * @io: Pointer to the MyIO structure
 *
 * The function reads from a file descriptor and sends it to the client.
 * It waits for an acknowledgment from the client before proceeding.
 */
void sendfile_toclient(struct MyIO *io)
  __attribute__((__nonnull__(1)));

/**
 * serverhandleget() - Handles GET requests from the client
 * @io: Pointer to the MyIO structure
 *
 * Reads a file from disk and sends it to the client.
 */
void serverhandleget(struct MyIO *io)
  __attribute__((__nonnull__(1)));

/**
 * serverhandleput() - Handles PUT requests from the client
 * @io: Pointer to the MyIO structure
 *
 * Receives a file from the client and saves it to disk.
 */
void serverhandleput(struct MyIO *io)
  __attribute__((__nonnull__(1)));

/**
 * serverhandlehelp() - Sends help text to the client
 * @io: Pointer to the MyIO structure
 *
 * Sends available commands and their syntax to the client.
 */
void serverhandlehelp(struct MyIO *io)
  __attribute__((__nonnull__(1)));

/**
 * serverhandleexit() - Closes the client connection
 * @io: Pointer to the MyIO structure
 *
 * Terminates the connection with the client.
 */
void serverhandleexit(struct MyIO *io)
  __attribute__((__nonnull__(1)));

/**
 * init_clienthandleget() - Initialize client "get" handling
 * @savename:    Buffer to save the file name
 * @io:          Pointer to MyIO structure
 * @savenamelen: Length of the save name
 *
 * Clears existing data and preps for new command.
 */
void clienthandleget(struct MyIO *io)
  __attribute__((__nonnull__(1)));

void clienthandleput(struct MyIO *io)
  __attribute__((__nonnull__(1)));

void clienthandlehelp(struct MyIO *io)
  __attribute__((__nonnull__(1)));

void clienthandleexit(struct MyIO *io)
  __attribute__((__nonnull__(1)));

/**
 * read_prompt_clienthandle() - Read server prompt for client
 * create_savefile_getfd()    - Create and return file descriptor for saving
 * send_filename_toserver()   - Send file name back to the server
 *
 * @io:          Pointer to the MyIO structure
 * @savename:    Buffer to save the file name (only for create_savefile_getfd)
 * @sizename:    Size of the name being sent (only for send_filename_toserver)
 *
 * These functions handle reading the prompt, creating save files, and
 * sending file names to the server.
 */
void read_prompt_clienthandle(struct MyIO *io)
  __attribute__((__nonnull__(1)));

int create_savefile_getfd(char *savename, struct MyIO *io)
  __attribute__((__nonnull__(1,2)));

void send_filename_toserver(struct MyIO *io, int sizename)
  __attribute__((__nonnull__(1)));

void getfile_fromserver(char *savename, struct MyIO *io, size_t maxread)
  __attribute__((__nonnull__(1,2)));

void readbytes_fromsocket(struct MyIO *io, size_t szmax)
  __attribute__((__nonnull__(1)));

void init_clienthandleget(char *savename, struct MyIO *io, size_t savenamelen)
  __attribute__((__nonnull__(1,2)));

/**
 * closewritefd_restoreoldfd() - Closes write file descriptor and restores old FD
 * @oldfd: Old file descriptor to restore
 * @io: Pointer to MyIO structure
 *
 * Closes the current write file descriptor and restores the previous one.
 */
void closewritefd_restoreoldfd(int oldfd, struct MyIO *io)
  __attribute__((__nonnull__(2)));

/**
 * closereadfd_restoreoldfd() - Closes read file descriptor and restores old FD
 * @oldfd: Old file descriptor to restore
 * @io: Pointer to MyIO structure
 *
 * Closes the current read file descriptor and restores the previous one.
 */
void closereadfd_restoreoldfd(int oldfd, struct MyIO *io)
  __attribute__((__nonnull__(2)));

/**
 * sendfile_tosocket() - Send file data to a socket
 * @io: Pointer to the MyIO structure containing necessary I/O parameters
 *
 * Reads chunks of data from the file descriptor specified in the MyIO
 * structure and sends it to the socket until the entire file has been sent.
 * It reads a maximum of NETREADMAX-1 bytes in each iteration.
 *
 * Note: This function uses 'goto' for loop control and employs the
 * readfd_writesocket function for actual I/O. Also, it flushes the
 * output buffer at the end.
 */
void sendfile_tosocket(struct MyIO *io)
  __attribute__((__nonnull__(1)));

/**
 * sendfile_toserver() - Send a file to the server
 * @io: Pointer to the MyIO structure containing necessary I/O parameters
 * @maxread: Maximum number of bytes to read from the file
 *
 * This function performs the following tasks in sequence:
 * 1. Reads a file name from the user, appending it to the current working directory.
 * 2. Sends the file name to the server.
 * 3. Opens the specified file for reading.
 * 4. Calls sendfile_tosocket() to actually send the file data to the server.
 * 5. Restores the original file descriptor for reading.
 *
 * Note:
 * - The function uses the myreadline, getcwd, mystrcat, and myopenfile
 *   utility functions for various operations.
 * - The function has a [TODO] comment indicating potential for buffer overflow
 *   that should be addressed.
 * - The original read file descriptor is saved and restored during the operation.
 */
void sendfile_toserver(struct MyIO *io, size_t maxread)
  __attribute__((__nonnull__(1)));

#endif /* __FILE_TRANSFER_H */

