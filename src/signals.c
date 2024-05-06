/**
 * @file signals.c
 * @brief Signal Handling Functions
 *
 * This file defines the functions required for handling signals within the server application.
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
#include "signal.h"
#include "syscalls.h"
#include "globals.h"

#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

void
sigchld_handler(int sig)
{
  pid_t pid;
  sigset_t mask_all, old;
  int sav_errno = errno, wstatus;

  mysigemptyset(&mask_all);
  /* we want to get rid of all children processes incase one was blocked */
  while((pid = waitpid(-1, &wstatus, WNOHANG | WUNTRACED)) > 0) {
    /* block all signals */
    mysigprocmask(SIG_BLOCK, &mask_all, &old);
    if (WIFEXITED(wstatus)) {
      myfprintf(sys_stdout, "exited, status=%d\n", WEXITSTATUS(wstatus));
    } else if (WIFSIGNALED(wstatus)) {
      myfprintf(sys_stdout, "killed by signal %d\n", WTERMSIG(wstatus));
    } else if (WIFSTOPPED(wstatus)) {
      myfprintf(sys_stdout, "stopped by signal %d\n", WSTOPSIG(wstatus));
    } else if (WIFCONTINUED(wstatus)) {
      myfprintf(sys_stdout, "continued\n");
    }
    mysigprocmask(SIG_SETMASK, &old, NULL);
  }

  errno = sav_errno;
}

void
sigint_handler(int sig)
{
  myfprintf(sys_stdout, "\nCaught SIGINT\n");
  _exit(0);
}

void
install_handlers(void)
{
  mysigaction(SIGCHLD, sigchld_handler);
  mysigaction(SIGINT, sigint_handler);
}

#if DEBUG
int
main(int argc, char *argv[])
{
  pid_t pid;

  install_handlers();

  for (size_t n = 0; n < 5; n++) {
    pid = myfork();
    if (pid == 0) {
      printf("%d child %ld\n", pid, n);
      sleep(1);
      _exit(1);
    }
    printf("parent %ld\n", n);
  }
}
#endif // DEBUG

