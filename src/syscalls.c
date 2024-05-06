/**
 * @file syscalls.c
 * @brief System Call Wrappers and Error Handlers
 *
 * This file provides wrappers around system calls to handle errors robustly.
 * It includes utility functions for handling system errors and exiting, and
 * wrappers for various system calls to handle errors automatically.
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
#define _POSIX_C_SOURCE 200809L /* for signal.h DO NOT MOVE */

#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>

#include "syscalls.h"
#include "mystring.h"

void
syserrorexit(const char * const err, int sck, int errnum)
{
  fdputs(sys_stderr, err);
  if(sck != -1) {
    close(sck);
  }
  _exit(errnum);
}

/* start wrappers for unit testing */
ssize_t
Read(int fd, void *buf, size_t count)
{
  return read(fd, buf, count);
}

ssize_t
Write(int fd, const void *buf, size_t count)
{
  return write(fd, buf, count);
}
/* end wrappers for unit testing */

int
myopenfile(const char *pathname, int mode)
{
  int fd = open(pathname, mode);
  if (fd == -1) {
    printerr_exit("open() error\n");
  }
  return fd;
}

size_t
mysckread(int sck, void *buf, size_t count)
{
  ssize_t total_read = 0, nread;
  char *cbuf = (char *) buf;

  /* we want non-block mode */
  int old_flags = fcntl(sck, F_GETFL, 0);
  if (old_flags == -1 || fcntl(sck, F_SETFL, old_flags | O_NONBLOCK) == -1) {
    syserrorexit("fcntl error", sck, 1);
  }

  while (total_read < count) {
    nread = Read(sck, cbuf + total_read, count - total_read);
    if (nread < 0) {
      if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
      syserrorexit("mysckread()", sck, errno);
    }
    if (nread == 0) {
      return total_read;
    }
    total_read += nread;
  }

  /* need to put back into blocking for poll */
  if (fcntl(sck, F_SETFL, old_flags) == -1) { 
    syserrorexit("fcntl error", sck, 1);
  }

  return total_read;
}

size_t
mysckwrite(int sck, const void *buf, size_t count)
{
  ssize_t total = 0, nwritten;
  const char *cbuf = (const char *) buf;

  while (total < count) {
    nwritten = Write(sck, cbuf + total, count - total);
    if(nwritten == -1) {
      syserrorexit("write error", sck, 1);
    }
    if (nwritten == 0) {
      return total;
    }
    total += nwritten;
  }
  return total;
}

size_t
mysckread_noblock(int sck, void *buf, size_t count)
{
  ssize_t total_read = 0;
  ssize_t nread;

  /* we want non-block mode */
  int old_flags = fcntl(sck, F_GETFL, 0);
  if (old_flags == -1 || fcntl(sck, F_SETFL, old_flags | O_NONBLOCK) == -1) {
    syserrorexit("fcntl error", sck, 1);
  }

  while (1) {
    nread = Read(sck, ((char *)buf) + total_read, count - total_read);
    if (nread == -1) {
      if (errno == EAGAIN ||
          errno == EWOULDBLOCK) { /* nothing more to read */
        break;
      } else {
        syserrorexit("read error", sck, 1);
      }
    } else if (nread == 0) { /* end of read */
      break;
    }

    total_read += nread;
    if (total_read == count) { /* need more room */
      break;
    }
  }

  if (fcntl(sck, F_SETFL, old_flags) == -1) {
    syserrorexit("fcntl error", sck, 1);
  }

  return total_read;
}

pid_t
myfork(void)
{
  pid_t cur_pid = fork();
  if (cur_pid == -1) {
    printerr_exit("fork() error");
  }
  return cur_pid;
}

void
myexecve(const char *pathname, char *const argv[], char *const envp[])
{
  char command[PATH_MAX];

  mystrcpy(command, "/usr/bin/");
  mystrcat(command, pathname);

  if (execve(command, argv, envp) == -1) {
    printerr_exit("execve error()\n");
  }
}

pid_t
mywait(int *wstatus)
{
  pid_t pid;
  if ((pid = wait(wstatus)) == -1) {
    printerr_exit("wait() error\n");
  }
  return pid;
}

pid_t
mywaitpid(pid_t pid, int *wstatus, int options)
{
  pid_t rpid;

  if ((rpid = waitpid(pid, wstatus, options)) < 0) {
    printerr_exit("waitpid() error\n");
  }
  return rpid;
}

ssize_t
myread(int fd, void *buf, size_t nbytes)
{
  ssize_t n_read = read(fd, buf, nbytes);
  if (n_read == -1) {
    printerr_exit("read() error\n");
  }
  return n_read;
}

ssize_t
mywrite(int fd, const void *buf, size_t count)
{
  ssize_t n_write = write(fd, buf, count);
  if (n_write == -1) {
    printerr_exit("write() error\n");
  }
  return n_write;
}

int
mydup2(int oldfd, int newfd)
{
  /** @fd: the new fd */
  int fd = dup2(oldfd, newfd);
  if (fd == -1) {
    printerr_exit("dup2() error\n");
  }

  return fd;
}

void
mypipe(int *pipefd)
{
  if (pipe(pipefd) == -1) {
    printerr_exit("pipe() error\n");
  }
}

void
myclose(int fd)
{
  if (close(fd) == -1) {
    printerr_exit("close() error\n");
  }
}

int
myopen(const char *pathname, int flags, mode_t mode)
{
  int fd = open(pathname, flags, mode);
  if (fd == -1) {
    printerr_exit("open() error\n");
  }
  return fd;
}

void
mysigaction(int signum, void (*handler)(int))
{
  /**
   * @sa: the new action for signal signum
   *       is installed from act.
   */
  struct sigaction sa;

  sa.sa_flags = SA_RESTART;
  sa.sa_handler = handler;
  mysigemptyset(&sa.sa_mask);

  if (sigaction(signum, &sa, NULL) == -1) {
    printerr_exit("sigaction() error\n");
  }
}

void
mysignal(int signum, void (*handler)(int, siginfo_t *, void *))
{
  /**
   * @sa: the new action for signal signum
   *       is installed from act.
   */
  struct sigaction sa;

  sa.sa_flags = SA_RESTART | SA_SIGINFO; /* restarts system call */
  sa.sa_sigaction = handler;
  mysigemptyset(&sa.sa_mask);

  if (sigaction(signum, &sa, NULL) == -1) {
    printerr_exit("signal() error\n");
  }
}

void
mysigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
  if (sigprocmask(how, set, oldset) == -1) {
    printerr_exit("sigprocmask() error\n");
  }
}

void
mysigemptyset(sigset_t *set)
{
  if (sigemptyset(set) == -1) {
    printerr_exit("sigemptyset() error\n");
  }
}

void
mysigfillset(sigset_t *set)
{
  if (sigfillset(set) == -1) {
    printerr_exit("sigfillset() error\n");
  }
}

void
mysigaddset(sigset_t *set, int signum)
{
  if (sigaddset(set, signum) == -1) {
    printerr_exit("sigaddset() error\n");
  }
}

void
mysigdelset(sigset_t *set, int signum)
{
  if (sigdelset(set, signum) == -1) {
    printerr_exit("sigdelset() error\n");
  }
}

int
mysigismember(const sigset_t *set, int signum)
{
  int ret;
  if ((ret = sigismember(set, signum)) == -1) {
    printerr_exit("sigismember() error\n");
  }
  return ret;
}

int
mysigsuspend(const sigset_t *mask)
{
  int ret = sigsuspend(mask);
  if (errno != EINTR) {
    printerr_exit("sigsuspend() error\n");
  }
  return ret;
}

void
mysetpgid(pid_t pid, pid_t pgid)
{
  if (setpgid(pid, pgid) == -1) {
    printerr_exit("setpgid() error\n");
  }
}

void *
mymalloc(size_t size)
{
  void *addr = NULL;

  addr = mmap(NULL, size, MALLOC_PROT, MALLOC_FLAGS, -1, 0);
  if (addr == MAP_FAILED) {
    printerr_exit("mmap() error\n");
  }

  return addr;
}

void
myfree(void *addr, size_t size)
{
  if ((munmap(addr, size)) == -1) {
    printerr_exit("munmap() error\n");
  }
}

int
myaccess(const char * const pathname, int mode)
{
  return access(pathname, mode);
}

