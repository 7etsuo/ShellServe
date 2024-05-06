/**
 * @file command_handler.h
 * @brief
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
#ifndef __COMMAND_HANDLER_H
#define __COMMAND_HANDLER_H

#include "globals.h"
#include "server_core.h"

/**
 * argv_alloc - Global 2D character array for command arguments
 *
 * This 2D array is used to store individual argument strings for commands.
 * Each row represents an argument, and each column represents a character in
 * that argument.
 */
extern char argv_alloc[MAX_NUM_ARGS][MAX_LINE_SIZE];

/**
 * struct __Pipeline - Pipeline structure to manage command pipelines
 * @argv: Array to hold pointers to argument strings for the command
 * @fin: File name for input redirection, NULL if not applicable
 * @fout: File name for output redirection, NULL if not applicable
 * @argc: Number of arguments in argv
 * @fd: Array of file descriptors (READ_END, WRITE_END) for the pipe
 * @sockfd: Socket file descriptor for client connection
 *
 * This structure encapsulates all information needed to manage a single
 * command in a pipeline. It includes the command arguments, file redirection
 * options, and various file descriptors for pipes and sockets.
 */
typedef struct __Pipeline Pipeline;

/**
 * parse_commandline - Parse a command line into an argv array
 * @argv: Array to hold pointers to argument strings
 * @command_line: Command line string to parse
 *
 * Parses a command line string into an array of argument strings.
 * Each token, separated by spaces, becomes an individual argument.
 *
 * Return: Number of arguments parsed.
 */
int parse_commandline(char **argv, char *command_line)
  __attribute__((__nonnull__(1, 2)));

/**
 * execcmd - Execute a command using execve()
 * @argv: Array of command arguments
 *
 * Executes a command by calling execve(). It uses a globally-defined
 * environment variable set, g_envp.
 */
void execcmd(char *argv[])
  __attribute__((__nonnull__(1)));

/**
 * runcommand - Run a command within the context of a client
 * @client: Client context containing client-specific data
 * @readbuf: Buffer containing the command to run
 *
 * Initializes pipelines, builds them, and then runs the command.
 */
void runcommand(ClientData * const client, char *readbuf);

/**
 * dup2_and_close - Redirects I/O and closes unneeded pipes
 * @pipe: Pipeline structure array
 * @npipes: Number of pipes in the pipeline
 * @i: Index of the current command in the pipeline
 *
 * Redirects the I/O for a specific command in the pipeline and
 * closes all unnecessary file descriptors.
 */
void dup2_and_close(Pipeline *pipe, size_t npipes, int i)
  __attribute__((__nonnull__(1)));

/**
 * do_dup2_command - Dup2 handling for a pipeline command
 * @pipe: Pipeline structure array
 * @npipes: Number of pipes in the pipeline
 * @i: Index of the current command in the pipeline
 *
 * Handles dup2() calls for file descriptor redirection for a
 * specific command in the pipeline.
 */
void do_dup2_command(Pipeline *pipe, size_t npipes, int i)
  __attribute__((__nonnull__(1)));

/**
 * dup2_last_command - Handle last command in a pipeline
 * @pipe: Pipeline structure array
 * @i: Index of the last command in the pipeline
 *
 * Redirects stdout to the socket for the last command in the pipeline.
 */
void dup2_last_command(Pipeline *pipe, int i)
  __attribute__((__nonnull__(1)));

/**
 * dup2_not_last_command - Handle non-last commands in a pipeline
 * @pipe: Pipeline structure array
 * @i: Index of the current non-last command in the pipeline
 *
 * Redirects stdout to the write end of the pipe for commands that
 * are not the last in the pipeline.
 */
void dup2_not_last_command(Pipeline *pipe, int i)
  __attribute__((__nonnull__(1)));


/**
 * dup2_not_first_command - Handle non-first commands in a pipeline
 * @pipe: Pipeline structure array
 * @i: Index of the current non-first command in the pipeline
 *
 * Redirects stdin from the read end of the previous pipe.
 */
void dup2_not_first_command(Pipeline *pipe, int i)
  __attribute__((__nonnull__(1)));

#endif // __COMMAND_HANDLER_H
