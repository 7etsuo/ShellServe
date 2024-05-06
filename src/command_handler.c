/**
 * @file command_handler.c
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
#include <stdio.h> /* for fflush */

#include "globals.h"
#include "mystring.h"
#include "syscalls.h"
#include "server_core.h"
#include "command_handler.h"
#include "pipeline.h"

char argv_alloc[MAX_NUM_ARGS][MAX_LINE_SIZE];

int
parse_commandline(char *argv[], char *command_line)
{
  char *line;
  int i;
  i = 0;
  for (line = mystrtok(command_line, ' '); i < MAX_NUM_ARGS && line; line = mystrtok(NULL, ' '), i++) {
    if (line[0] == '\0') {
      --i;
      continue; /* skip empty */
    }
    mymemset(argv_alloc[i], 0, MAX_LINE_SIZE);
    mystrcpy(argv_alloc[i], line);
    argv[i] = argv_alloc[i];
  }

  argv[i] = NULL;
  return i;
}

void
runcommand(ClientData * const client, char *readbuf)
{
  int npipes;

  Pipeline pipe[MAX_NUM_ARGS];

  init_pipelines(pipe, client->clientfd);
  npipes = build_pipeline(pipe, client, readbuf);
  run_pipeline(pipe, npipes);
}

void
dup2_not_first_command(Pipeline *pipe, int i)
{
  printf("in i != 0\n");
  mydup2(pipe[i-1].fd[READ_END], sys_stdin);
}

void
dup2_not_last_command(Pipeline *pipe, int i)
{
  printf("in i < npipes-1\n");
  mydup2(pipe[i].fd[WRITE_END], sys_stdout);
}

void
dup2_last_command(Pipeline *pipe, int i)
{
  printf("in i == npipes-1\n");
  mydup2(pipe[i].sockfd, sys_stdout);
}

void
do_dup2_command(Pipeline *pipe, size_t npipes, int i)
{
  if (i != 0) dup2_not_first_command(pipe, i); 
  if (i < npipes-1) dup2_not_last_command(pipe, i);
  else dup2_last_command(pipe, i);
}

void
dup2_and_close(Pipeline *pipe, size_t npipes, int i)
{
  do_dup2_command(pipe, npipes, i);
  close_pipes(pipe, npipes);
  close(pipe[i].sockfd);
}
