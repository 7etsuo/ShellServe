/**
 * @file syscalls.h
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
#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#include <signal.h>
#include <errno.h>

#include "globals.h"
#include "mystring.h"

/** Define MAP_ANONYMOUS for systems that don't have it */
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS (0x20)
#endif

/* flags for malloc */
#define MALLOC_PROT (PROT_READ | PROT_WRITE)
#define MALLOC_FLAGS (MAP_PRIVATE | MAP_ANONYMOUS)

/* standard streams.  */
#define sys_stdin 0
#define sys_stdout 1
#define sys_stderr 2

/** @msg: error message to be print */
#define printerr_exit(msg)                      \
  do {                                          \
    fdputs(sys_stderr, msg);                    \
    _exit(1);                                   \
  } while(0);

/* wrappers for unit testing */
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
/* end wrappers for unit testing */


/**
 * myopenfile() - open a file 
 * @pathname file to open
 * @mode the mode to open the file in e.g., O_RDONLY (man 2 open)
 * @return returns the fd
 */
int
myopenfile(const char *pathname, int mode)
  __attribute__((__nonnull__(1)));

/**
 * mysckread() - Handles system errors and exits.
 * @err The error message to output.
 * @sck The socket to close if it is not -1.
 * @errnum The error number to exit with.
 */
size_t mysckread(int sck, void *buf, size_t count)
  __attribute__((__nonnull__(2)));


/**
 * mysckread_noblock() - Handles system errors and exits. does not block
 * @err The error message to output.
 * @sck The socket to close if it is not -1.
 * @errnum The error number to exit with.
 */
size_t mysckread_noblock(int sck, void *buf, size_t count)
  __attribute__((__nonnull__(2)));

/**
 * mysckwrite() - Writes to a socket, handling errors.
 * @sck The socket to write to.
 * @buf The buffer to write from.
 * @count The number of bytes to write.
 *
 * Return: The number of bytes written.
 */
size_t mysckwrite(int sck, const void *buf, size_t count)
  __attribute__((__nonnull__(2)));

/**
 * myaccess() - checks whether the calling process can access the file pathname.
 * 	      If pathname is a symbolic link, it is dereferenced.
 * @pathname: The pathname of the file to be accessed.
 * @mode: The mode specifies the accessibility check(s) to be performed,
 *        and is either the value F_OK, or a mask consisting of the bitwise OR of
 * 	  one or more of R_OK, W_OK, and X_OK.
 *
 * Return: On success, zero is returned. On error, -1 is returned,
 * 	   and errno is set appropriately.
 */
int myaccess(const char * const pathname, int mode)
  __attribute__((__nonnull__(1)));

/**
 * myfree() - Free a block of memory allocated by mymalloc().
 * @addr: The address of the block of memory to free.
 * @size: The size of the block of memory to free.
 */
void myfree(void *addr, size_t size)
  __attribute__((__nonnull__(1)));

/**
 * mymalloc() - Allocate size bytes of memory.
 * @size: The number of bytes to allocate.
 *
 * Return: On success, returns a pointer to the allocated memory.
 */
void *mymalloc(size_t size);

/** mysetpgid() - man setpgid(2), sets the PGID of the process specified by pid to pgid.
 *              If pid is zero, then the process ID of the calling process is used.
 *              If pgid is zero, then the PGID of the process specified by pid is made
 *              the same as its process ID.
 * @pid: the process ID of the process whose PGID is to be set
 * @pgid: the new PGID of the process
 */
void mysetpgid(pid_t pid, pid_t pgid);

/**
 * mysigsuspend() - man Sigsuspend(2), temporarily replaces the signal
 * 	         mask of the calling thread with the mask given by mask and then
 *               suspends the thread until delivery of a signal whose action is
 *               to invoke a signal handler or to terminate a process.
 *
 * @mask: the signal mask to use temporarily
 *
 * Return: always returns -1, with errno set to indicate the error
 * 	   (normally, EINTR).
 */
int mysigsuspend(const sigset_t *mask)
  __attribute__((__nonnull__(1)));

/**
 * mysigismember() - man sigismember(2), tests whether signum is a member of set.
 * @set: the signal set to test
 * @signum: the signal to test for membership in set
 *
 * Return: 1 if signum is a member of set, 0 if not.
 */
int mysigismember(const sigset_t *set, int signum);

/**
 * mysigdelset() - man sigdelset(2), delete signal signum from set.
 * @set: the signal set to delete signum from
 * @signum: the signal to delete from set
 * Remove SIGNO from SET
 */
void mysigdelset(sigset_t *set, int signum);

/**
 * mysigaddset() - man sigaddset(2), add signal signum from set.
 * @set: the signal set to add signum to
 * @signum: the signal to add to set
 * Add SIGNO to SET.
 */
void mysigaddset(sigset_t *set, int signum);

/**
 * mysigfillset() - man sigfillset(2), initializes set to full, including all signals.
 * @set: the signal set to initialize
 * Add SIGNO to SET.
 */
void mysigfillset(sigset_t *set);

/**
 * mysigemptyset() - man sigemptyset(2), initializes the signal set given by set to empty,
 *                 with all signals excluded from the set.
 * @set: the signal set to initialize
 * Clear all signals from SET.
 */
void mysigemptyset(sigset_t *set);

/**
 * mysigprocmask() - man sigprocmask(2), is used to fetch and/or change
 *		   the signal mask of the calling thread.
 * @how: SIG_BLOCK | SIG_UNBLOCK | SIG_SETMASK
 * @set: If  set  is NULL, then the signal mask is unchanged
 * @oldset: If oldset is non-NULL, the previous value of the signal mask is stored in oldset.
 * Get and/or change the set of blocked signals
 */
void mysigprocmask(int how, const sigset_t *set, sigset_t *oldset);

/**
 * mysignal() - Installs a new signal handler for the specified signal,
 *              with options for signal handling and masking.
 *
 * @signum: The signal number for which the handler is being set.
 *          Valid signals are defined in signal.h (e.g., SIGINT, SIGTERM, etc.).
 *
 * @handler: A pointer to the function to be called when the signal is received.
 *           This function will be passed three arguments:
 *           - The signal number.
 *           - A pointer to a siginfo_t structure (providing signal-related information).
 *           - A pointer to a ucontext_t structure (providing machine context).
 *
 * This function wraps the sigaction system call, providing a mechanism to change
 * the action taken by a process on receipt of a specific signal. The SA_RESTART flag
 * ensures that certain interrupted system calls are restarted, and the SA_SIGINFO flag
 * provides additional signal-related information to the handler.
 *
 * The function also initializes a new signal mask, ensuring that no additional
 * signals are masked during the execution of the handler.
 *
 * Error Handling:
 * In case of an error during the sigaction system call, this function calls
 * printerr_exit to handle the error and exit the process.
 *
 * Usage:
 * To use this function, define a signal handler function with the appropriate
 * signature, and then call mysignal, passing the signal number and the handler function.
 *
 * Example:
 * void handler(int signo, siginfo_t *info, void *context) { ... }
 * mysignal(SIGINT, handler);
 *
 */
void mysignal(int signum, void (*handler)(int, siginfo_t *, void *));

/**
 * mysigaction() - man sigaction(2), Get and/or set the
 *               action for signal SIG.
 * @signum: specifies the signal and can be any valid
 *          signal except SIGKILL and SIGSTOP
 * @handler: specifies the action to be associated
 *           with signum. A pointer to a signal handling
 *           function. This function receives the signal
 *           number as its only argument.
 */
void mysigaction(int signum, void (*handler)(int));

/**
 * myopen() - Opens a file or device specified by the pathname, returning a file descriptor.
 *
 * @pathname: The path to the file or device to be opened.
 * @flags: The file status flags and file access modes of the open file description.
 *         Common flags include O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_TRUNC, etc.
 * @mode: The file mode bits of the open file description when a new file is created.
 *        This argument is needed only when O_CREAT or O_TMPFILE is specified in flags.
 *        Common mode bits include S_IRUSR, S_IWUSR, S_IXUSR, etc.
 *
 * Return: On success, returns a non-negative file descriptor.
 *         On error, -1 is returned, and errno is set appropriately.
 *
 */
int myopen(const char *pathname, int flags, mode_t mode)
  __attribute__((__nonnull__(1)));

/**
 * myclose() - closes a file descriptor, so that it no longer refers to any
 *           file and may be reused.
 * @fd: the resources associated with the file description are freed
 */
void myclose(int fd);

/**
 * mypipe() - man pipe(2) Create a one-way communication channel (pipe).
 *          If successful, two file descriptors are stored in pipefd;
 *          bytes written on pipefd[1] can be read from pipefd[0].
 *          Returns 0 if successful, -1 if not.
 * @pipefd: The array pipefd is used to return two file
 *          descriptors referring to the ends of the pipe.
 *
 * pipefd[0] refers to the read end of the pipe.
 * pipefd[1] refers  to the  write  end of the pipe.
 */
void mypipe(int *pipefd)
  __attribute__((__nonnull__(1)));

/**
 * mydup2() - man dup2(2) Duplicate oldfd to newfd, closing
 *          newfd and making it open on the same file.
 * @oldfd: the descriptor that we want to duplicate
 * @newfd: the result fd.
 *
 * Return: returns the new file descriptor.
 */
int mydup2(int oldfd, int newfd);

/**
 * mywrite() - man write(1) Write N bytes of BUF to FD.
 *           Return the number written
 * @fd: the fd to write to
 * @buf: the buffer to write from
 * @count: the number of bytes to write
 *
 * Return: the number of bytes written
 */
ssize_t mywrite(int fd, const void *buf, size_t count)
  __attribute__((__nonnull__(2)));

/**
 * myread() - man read(2) Read NBYTES into BUF from FD.
 *          Return the number read
 * @fd: the fd to read from
 * @buf: the buffer to read to
 * @nbytes: the number of bytes to read
 *
 * Return: the number of bytes read
 */
ssize_t myread(int fd, void *buf, size_t nbytes)
  __attribute__((__nonnull__(2)));

/**
 * mywaitpid() - man waitpid(2) suspends execution of the calling
 *             thread until a child specified by pid argument has changed state.
 * @pid: the pid of the child to wait for.
 * @wstatus: If wstatus is not NULL Waitpid() stores status information
 *           in the int to which it points.
 * @status: The value of options is an OR of zero or more options
 *
 * The value of @pid can be:
 * < -1 wait child PID is the absolute value of @pid.
 *   -1 wait child any pid.
 *    0 wait child PID is calling process at time of call to waitpid().
 * >  0 wait child PID equal to the value of @pid.
 *
 * The value of @options can be:
 * WNOHANG return immediately if no child has exited.
 * WUNTRACED also return if a child has stopped.
 * WCONTINUED also return if a stopped child has been resumed by delivery of SIGCONT.
 */
pid_t mywaitpid(pid_t pid, int *wstatus, int options);

/**
 * mywait() - man, wait(2) Wait for a child to die. When one does,
 *          put its status in *STAT_LOC and return its process ID.
 * @wstatus: if not NULL, store status information
 *           in the int to which it points
 *
 * Return: on success returns the process ID
 * of the terminated child.
 */
pid_t mywait(int *wstatus);

/**
 * myexecve() - man execve(2) Replace the current process,
 *            executing PATH with arguments ARGV and
 *            environment ENVP. ARGV and ENVP are terminated
 *            by NULL pointers.
 * @pathname: absolute path with command
 * @argv: command and arguments for the command
 * @envp: environment variables
 */
void myexecve(const char *pathname, char *const argv[], char *const envp[]);

/**
 * myfork() - man fork(2) Clone the calling process, creating an exact copy.
 *
 * Return: on success, the PID of the child process is returned
 *         to the parent, and 0 is produced in the child
 */
pid_t myfork(void)
  __attribute__((__warn_unused_result__));

#endif // __SYSCALLS_H

