#include "pipeline.h"
#include "command_handler.h"
#include "syscalls.h"

#include <stdio.h> /* for fflush */

int
parse_pipeline(Pipeline *pipe, int argc, char *argv[])
{
  int command_count = 0;
  int argument_count = 0;

  for (size_t i = 0; i < argc; i++) {
    if (mystrcmp(argv[i], "|") == 0) {
      pipe[command_count].argv[argument_count] = NULL;
      argument_count = 0;
      ++command_count;
      continue;
    }

    pipe[command_count].argv[argument_count++] = argv[i];
    pipe[command_count].argc = argument_count;
  }

  pipe[command_count].argv[argument_count] = NULL;

  return command_count + 1;
}

void
init_pipeline(Pipeline *pipe, int fd)
{
  pipe->argc = 0;

  pipe->fin = NULL;
  pipe->fout = NULL;

  for (int i = 0; i < MAX_NUM_ARGS; i++) {
    pipe->argv[i] = NULL;
  }

  for (int i = 0; i < FDLEN; i++) {
    pipe->fd[i] = -1;
  }

  pipe->sockfd = fd;
}

void
init_pipelines(Pipeline pipes[MAX_NUM_ARGS], int fd)
{
  for (int i = 0; i < MAX_NUM_ARGS; i++) {
    init_pipeline(&pipes[i], fd);
  }
}

void
init_pipesfd(Pipeline *pipe, size_t npipes)
{
  for (int i = 0; i < npipes; i++) {
    mypipe(pipe[i].fd);
    printf("pipe[%d].fd[0] = %d\n", i, pipe[i].fd[0]);
    printf("pipe[%d].fd[0] = %d\n", i, pipe[i].fd[1]);
  }
  fflush(NULL);
}

void
close_pipes(Pipeline *pipe, size_t npipes)
{
  for (int j = 0; j < npipes; j++) {
    myclose(pipe[j].fd[READ_END]);
    myclose(pipe[j].fd[WRITE_END]);
  }
}

void
run_pipeline(Pipeline *pipe, size_t npipes)
{
  pid_t pid;

  init_pipesfd(pipe, npipes);

  for (int i = 0; i < npipes; i++) {
    pid = myfork();
    if (pid == 0) { /* child client */
      dup2_and_close(pipe, npipes, i);
      myexecve(pipe[i].argv[0], &pipe[i].argv[0], g_envp);
    }
    /* parent */
  }
  close_pipes(pipe, npipes); /* close all pipes in parent */

  /* [TODO] use SIG's so we can have bg procs */
  for (int i = 0; i < npipes; i++) {
    mywait(NULL);
  }
}

int
build_pipeline(Pipeline *pipe, ClientData * const client, char * const readbuf)
{
  int argc, npipes;
  char *argv[MAX_LINE_SIZE];

  argc = parse_commandline(argv, readbuf);
  npipes = parse_pipeline(pipe, argc, argv);

  return npipes;
}

