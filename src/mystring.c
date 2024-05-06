/**
 * @file mystring.c
 * @brief Custom String and Memory Manipulation Library
 *
 * This file declares functions for custom string and memory manipulation,
 * aiming to provide similar functionality as found in standard libraries
 * like <string.h> and <stdio.h>, with some additional features.
 * Each function is carefully documented to ensure clarity in their usage
 * and expected behavior.
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
#include "globals.h"
#include "mystring.h"
#include "syscalls.h"
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#if DEBUG
#include <string.h>
#include <stdio.h>
#include <assert.h>
#endif // DEBUG


/* private */
#define __IO_GETCHAR_BUFSIZE 1024
#define __SZ_PUTL 128
#define __ENVIRON_NAME_MAX 128
#define __ENV_MAX MAX_LINE_SIZE
#define __MAX_CMD_LEN MAX_LINE_SIZE
/* end private */

int
mygetchar(int fd)
{
  static char buf[__IO_GETCHAR_BUFSIZE];
  static char *bufp = buf;
  static int n = 0;

  if (n == 0) {
    n = myread(fd, buf, sizeof buf);
    /* ctrl-d */
    if (n == 0) {
      _exit(1);
    }
    bufp = buf;
    ++n;
  }
  return (--n > 0) ? (unsigned char)*bufp++ : EOF;
}

size_t
myreadline(int fd, char * const line, size_t read_max)
{
  size_t n_total;
  char c = mygetchar(fd);
  for (n_total = 0; n_total < read_max - 1 && c != EOF; n_total++) {
    line[n_total] = c;
    c = mygetchar(fd);
  }
  line[n_total - 1] = '\0';

  return n_total;
}

void
myfprintf(int fd, const char *strn, ...)
{
  va_list ap;
  size_t len;
  int d, c, curchar;
  char buf[__IO_GETCHAR_BUFSIZE];
  char *s;
  va_start(ap, strn);
  while (*strn) {
    curchar = *strn++;
    if (curchar != '%') {
      mywrite(fd, &curchar, 1);
    } else {
      switch (*strn) {
        /* string */
      case 's':
        s = va_arg(ap, char *);
        len = mystrlen(s);
        writechars(fd, s, len);
        strn++;
        break;
        /* int */
      case 'd':
        d = va_arg(ap, int);
        s = myitoa(d, buf);
        len = mystrlen(s);
        mywrite(fd, s, len);
        strn++;
        break;
        /* char */
      case 'c':
        c = (char)va_arg(ap, int);
        mywrite(fd, &c, 1);
        strn++;
        break;
      }
    }
  }
  va_end(ap);
}

char
*myitoa(int num, char *str)
{
  int start, end, i, rem, is_negative, tmp;

  is_negative = i = 0;
  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  if (num < 0) {
    is_negative = 1;
    num = -num;
  }

  while (num != 0) {
    rem = num % 10;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num /= 10;
  }

  if (is_negative) {
    str[i++] = '-';
  }
  str[i] = '\0';

  start = 0;
  end = i - 1;
  while (start < end) {
    tmp = *(str + start);
    *(str + start) = *(str + end);
    *(str + end) = tmp;
    start++;
    end--;
  }

  return str;
}

void
writechars(int fd, void *buf, size_t n)
{
  size_t n_left = n, n_write;
  char *bufp = buf;

  while (n_left > 0) {
    n_write = mywrite(fd, bufp, n_left);
    n_left -= n_write;
    bufp += n_write;
  }
}

void*
mymemset(void *s, int c, size_t n)
{
  char *tmp = s;

  while(n--) {
    *tmp++ = c;
  }

  return s;
}

void*
mymemcpy(void *dest, const void *src, size_t n)
{
  char *d = dest;
  const char *s = src;

  while(n > 0 && *s) {
    *d++ = *s++;
    n--;
  }

  return dest;
}

char*
mystpcpy(char *dest, const char *src)
{
  const char *s = src;

  while((*dest++ = *s++)) {;}

  return dest;
}

char*
mystrcat(char *dest, const char *src)
{
  char *d = dest;
  const char *s = src;

  while(*d++) {;}

  --d;  /* get rid of the null terminator. */

  while((*d++ = *s++)) {;}

  return dest;
}

char*
mystrcpy(char *dest, const char *src)
{
  const char *s = src;
  char *d = dest;

  while((*d++ = *s++)) {;}

  return dest;
}

char *
mystrncpy(char *dst, const char * const src, size_t n)
{
  char *d = dst;
  const char *s = src;

  while(n > 0 && *s) {
    *d++ = *s++;
    n--;
  }
  *d = '\0';

  return dst;
}

size_t
mystrlen(const char *s)
{
  const char *sp = s;

  while(*sp++) {;}

  return (sp - s - 1);
}

int
mystrcmp(const char *s1, const char *s2)
{
  const char *s = s1, *t = s2;
  for ( ; *s == *t; s++, t++) {
    if (*s == '\0') {
      return 0;
    }
  }
  return *s - *t;
}

int
mystrncmp(const char *s1, const char *s2, size_t n)
{
  const char *s1p = s1;
  const char *s2p = s2;

  while(--n > 0 && *s1p && *s2p && (*s1p == *s2p)) {
    s1p++;
    s2p++;
  }

  return (*s1p - *s2p);
}

const char*
mystrchr(const char *s, int c)
{
  const char *sp = s;

  while(*sp) {
    if(*sp == c) {
      return sp;
    }
    sp++;
  }

  return NULL;
}

const char*
mystrrchr(const char *s, int c)
{
  const char *sp = s;
  const char *spp = NULL;

  while(*sp) {
    if(*sp == c) {
      spp = sp;
    }
    sp++;
  }

  return spp;
}

ssize_t
myputs(const char *const s)
{
  return write(STDOUT_FILENO, s, mystrlen(s));
}

ssize_t
fdputs(int fd, const char *const s)
{
  size_t len = mystrlen(s);
  return write(fd, s, len);
}

char *
fdgetline(int fd)
{
  static char buf[__MAX_CMD_LEN];
  static char *bufptr = buf;
  static int n = 0;
  char *ptr = bufptr;

  if (n == 0) {
    n = myread(fd, buf, __MAX_CMD_LEN);
    bufptr = buf;
  }

  while (n > 0) {
    if (*bufptr == '\n') {
      *bufptr = '\0';
      bufptr++;
      n--;
      return ptr;
    }
    bufptr++;
    n--;
  }

  return NULL;
}

char *
mygetcwd(char *buf, size_t size)
{
  char *pathptr = mygetenv("PWD");
  mystrncpy(buf, pathptr, size);
  return buf;
}

char *
mygetenv(const char *name)
{
  int i, len;
  char *envptr;
  extern char **environ;
  char environ_name[__ENVIRON_NAME_MAX]; /* name of environment variable */

  for (i = 0; environ[i] != NULL; i++) {
    envptr = environ[i];
    len = mystrlen(name);
    mystrncpy(environ_name, envptr, len + 1);
    if (environ_name[len] != '=') {
      continue;
    }
    environ_name[len] = '\0';
    if (mystrcmp(environ_name, name) == 0) {
      return envptr + len + 1;
    }
  }
  return NULL;
}

char *
mystrsep(char **stringp, const char *delim)
{
  char *s;
  const char *spanp;
  int c, sc;
  char *tok;

  if ((s = *stringp) == NULL) {
    return (NULL);
  }
  for (tok = s;;) {
    c = *s++;
    spanp = delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0) {
          s = NULL;
        } else {
          s[-1] = 0;
        }
        *stringp = s;
        return (tok);
      }
    } while (sc != 0);
  }
  /* NOTREACHED */
}

/* [TODO]
 * 1. test
 * 2. document
 * 3. add prototype
 */
char *
remove_whitespace(char *line)
{
  while (*line && *line != '\n' && (*line == ' ' || *line == '\t')) {
    line++;
  }

  return line;
}

/* [TODO]
 * 1. Make robust.
 * 2. add functionality for multiple characters
 * 3. add flags for whitespace
 * very buggy pad end of string with '\0'
 * and don't fill to max to avoid overflows
 */
char *
mystrtok(char *str, char delim)
{
  static char *next_token = NULL;
  char *start_token;

  if (str != NULL) {
    next_token = str;
  } else if (next_token == NULL) {
    return NULL;
  }

  next_token = remove_whitespace(next_token);

  start_token = next_token;

  while (*next_token != '\0' && *next_token != delim) {
    next_token++;
  }

  if (*next_token != '\0') {
    *next_token = '\0';
    next_token++;
  } else if (*(next_token+1) == '\0') {
    next_token = NULL;
  } else {
    next_token++;
  }

  return start_token;
}

#if DEBUG
void
strchrtest(void)
{
  printf("strchrtest test\n");

  const char *str = "This is a string";
  const char *s = mystrchr(str, 's');
  assert(*s == 's');

  s = mystrchr(str, '4');
  assert(s == NULL);

  s = mystrchr(str, '\0');
  assert(*s == '\0');
}

void
strrchrtest(void)
{
  printf("strchrtest test\n");

  const char *str = "This is a string";
  const char *s = mystrrchr(str, 's');
  assert(*s == 's');

  s = mystrrchr(str, '4');
  assert(s == NULL);

  s = mystrrchr(str, '\0');
  assert(*s == '\0');
}

void
strcmptest(void)
{
  printf("strcmp test\n");

  int s1 = mystrcmp("", "");
  int s2 = strcmp("", "");

  assert(s1 == s2);

  s1 = mystrcmp("a", "a");
  s2 = strcmp("a", "a");

  assert(s1 == s2);

  s1 = mystrcmp("abcd", "abc");
  s2 = strcmp("abcd", "abc");

  assert(s1 == s2);

  s1 = mystrcmp("abc", "abcde");
  s2 = strcmp("abc", "abcde");

  assert(s1 == s2);

  s1 = mystrcmp("abcde", "abcde");
  s2 = strcmp("abcde", "abcde");

  assert(s1 == s2);
}

void
strncmptest(void)
{
  printf("strncmp test\n");

  int s1 = mystrncmp("", "", 0);
  int s2 = strncmp("", "", 0);

  assert(s1 == s2);

  s1 = mystrncmp("", "", 10);
  s2 = strncmp("", "", 10);

  assert(s1 == s2);

  s1 = mystrncmp("a", "a", 1);
  s2 = strncmp("a", "a", 1);

  assert(s1 == s2);

  s1 = mystrncmp("abcd", "abc", 4);
  s2 = strncmp("abcd", "abc", 4);

  assert(s1 == s2);

  s1 = mystrncmp("abc", "abcde", 5);
  s2 = strncmp("abc", "abcde", 5);

  assert(s1 == s2);

  s1 = mystrncmp("abcde", "abcde", 5);
  s2 = strncmp("abcde", "abcde", 5);

  assert(s1 == s2);
}

int
main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;


  char dest[1000];

  strcmptest();
  strncmptest();
  strchrtest();
  strrchrtest();

  return 0;
}
#endif // DEBUG
