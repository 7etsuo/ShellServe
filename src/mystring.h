/**
 * @file mystring.h
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

#ifndef __MYSTRING_H
#define __MYSTRING_H

#include "globals.h"

/*  (from <stdio.h>) The value returned by fgetc and similar functions to indicate the
    end of the file.  */
#ifndef EOF
#define EOF (-1)
#endif

/**
 * mymemset() - Set N bytes of S to C.
 *
 * @s: Pointer to the destination object.
 * @c: Value to be set.
 * @n: Number of bytes to be set.
 */
void *mymemset(void *s, int c, size_t n)
  __attribute__((__nonnull__(1)));

/**
 * mymemcpy() - Copy N bytes of SRC to DEST.
 *
 * @dest: Pointer to the destination object.
 * @src: Pointer to the source object.
 * @n: Number of bytes to be copied.
 */
void *mymemcpy(void *dest, const void *src, size_t n)
  __attribute__((__nonnull__(1, 2)));

/**
 * mystpcpy() - Copy a string from src to dest, returning a pointer to the end of the resulting string at dest.
 *
 * @dest: Pointer to the destination string.
 * @src: Pointer to the source string.
 */
char *mystpcpy(char *dest, const char *src)
  __attribute__((__nonnull__(1, 2)));

/**
 * mystrcat() - Append the string src to the string dest, returning a pointer dest.
 *
 * @dest: Pointer to the destination string.
 * @src: Pointer to the source string.
 */
char *mystrcat(char *dest, const char *src)
  __attribute__((__nonnull__(1, 2)));

/**
 * mystrncat() - Append at most n bytes from the string src to the string dest, returning a pointer to dest.
 *
 * @dest: Pointer to the destination string.
 * @src: Pointer to the source string.
 * @n: Maximum number of bytes to be appended.
 */
char *mystrncat(char *dest, const char *src, size_t n)
  __attribute__((__nonnull__(1, 2)));

/**
 * mystrlen() - Return the length of the string s.
 *
 * @s: Pointer to the string.
 */
size_t mystrlen(const char *s)
  __attribute__((__nonnull__(1)));

/**
 * mystrcpy() - Copy the string src to dest, returning a pointer to the start of dest.
 *
 * @dst: Pointer to the destination string.
 * @src: Pointer to the source string.
 */
char *mystrcpy(char *dst, const char * const src)
  __attribute__((__nonnull__(1, 2)));

/**
 * mystrncpy() - Copy at most n bytes from string src to dest, returning a pointer to the start of dest.
 *
 * @dst: Pointer to the destination string.
 * @src: Pointer to the source string.
 * @n: Maximum number of bytes to be copied.
 */
char *mystrncpy(char *dst, const char * const src, size_t n)
  __attribute__((__nonnull__(1, 2)));

/**
 * mystrcmp() - Compare the strings s1 with s2.
 *
 * @s1: Pointer to the first string.
 * @s2: Pointer to the second string.
 */
int mystrcmp(const char *s1, const char *s2)
  __attribute__((__nonnull__ (1, 2), __pure__));

/**
 * mystrncmp() - Compare at most n bytes of the strings s1 and s2.
 *
 * @s1: Pointer to the first string.
 * @s2: Pointer to the second string.
 * @n: Maximum number of bytes to be compared.
 */
int mystrncmp(const char *s1, const char *s2, size_t n)
  __attribute__((__nonnull__ (1, 2), __pure__));

/**
 * mystrchr() - Return a pointer to the first occurrence of the character c in the string s.
 *
 * @s: Pointer to the string.
 * @c: Character to be located.
 */
const char *mystrchr(const char *s, int c)
  __attribute__((__nonnull__ (1), __pure__));

/**
 * mystrrchr() - Return a pointer to the last occurrence of the character c in the string s.
 *
 * @s: Pointer to the string.
 * @c: Character to be located.
 */
const char *mystrrchr(const char *s, int c)
  __attribute__((__nonnull__ (1), __pure__));

/**
 * myputs() - Writes a string to stdout.
 *
 * @s: Pointer to the string.
 */
ssize_t myputs(const char *const s)
  __attribute__((__nonnull__(1)));

/**
 * myfdputs() - Writes a string to a file descriptor.
 *
 * @fd: File descriptor.
 * @s: Pointer to the string.
 */
ssize_t myfdputs(int fd, const char *const s)
  __attribute__((__nonnull__(2)));

/**
 * myreadline() - Reads a line from a file descriptor.
 *
 * @fd: File descriptor.
 * @line: Buffer to store the read line.
 * @read_max: Maximum number of bytes to be read.
 */
size_t myreadline(int fd, char * const line, size_t read_max)
  __attribute__((__nonnull__(2)));

/**
 * writechars() - writes n chars to FD.
 *
 * @fd: the fd to write to
 * @buf: the buf to be written
 * @n: the number of bytes to write
 */
void writechars(int fd, void *buf, size_t n)
  __attribute__((__nonnull__(2)));

/**
 * myitoa() - converts an integer to a string
 * @num: The integer to convert
 * @str: string to store the integer in
 */
char *myitoa(int num, char *str)
  __attribute__((__nonnull__(2)));

/**
 * myfprintf() - will write a variable args string to fd.
 *
 * @fd: the fd to write to
 * @strn: the formatted string to write supports %c chars %d integers %s strings
 */
void myfprintf(int fd, const char *strn, ...)
  __attribute__((__nonnull__(2)));

/**
 * mygetchar() - Reads a character from a file descriptor.
 *
 * @fd: File descriptor.
 */
int mygetchar(int fd);

/**
 * myputs() - Writes a string to stdout.
 *
 * @s: Pointer to the string to be written.
 *
 * Return: On success, the number of characters written is returned.
 *         On error, -1 is returned, and errno is set appropriately.
 */
ssize_t myputs(const char *const s)
  __attribute__((__nonnull__(1)));

/**
 * fdputs() - Writes a string to a specified file descriptor.
 *
 * @fd: The file descriptor to write to.
 * @s: Pointer to the string to be written.
 *
 * Return: On success, the number of characters written is returned.
 *         On error, -1 is returned, and errno is set appropriately.
 */
ssize_t fdputs(int fd, const char *const s)
  __attribute__((__nonnull__(2)));

//// TODO: Add attributes and implement these
/**
 * mystrsep() - Extracts the initial token from stringp that is delimited by one of the bytes in delim.
 *
 * @stringp: Pointer to the string.
 * @delim: Delimiter characters.
 */
char *mystrsep(char **stringp, const char * const delim);

/**
 * mygetcwd() - Gets the current working directory.
 *
 * @buf: Buffer to store the current working directory.
 * @size: Size of the buffer.
 */
char *mygetcwd(char *buf, size_t size);

/**
 * mygetenv() - Gets the value of an environment variable.
 *
 * @name: Name of the environment variable.
 */
char *mygetenv(const char *name);

/**
 * sigputl() - Writes a long value to stdout.
 *
 * @lv: Long value.
 */
ssize_t sigputl(long lv);

/**
 * sigputs() - Writes a string to stdout.
 *
 * @s: Pointer to the string.
 */
ssize_t sigputs(char s[]);

/**
 * sigerror() - Writes an error message to stderr.
 *
 * @s: Error message.
 */
void sigerror(char s[]);

/**
 * @fdgetline() - get line from file
 * @fd: file descriptor
 *
 * Return: line from file or NULL
 */
char *fdgetline(int fd);

/**
 * mygetcwd() - get current working directory
 * @buf: buffer to store cwd
 * @size: size of buffer
 *
 * Return: returns the cwd
 */
char *mygetcwd(char *buf, size_t size)
  __attribute__((__nonnull__(1)));

/**
 * mygetenv() - get environment variable
 * @name: name of environment variable
 *
 * Return: pointer to environment variable
 */
char *mygetenv(const char *name)
  __attribute__((__nonnull__(1)));

/**
 * mystrsep() - string separator
 * @stringp: the string to separate
 * @delim: the delimiter to separate by
 *
 * Get next token from string *stringp, where tokens are possibly-empty
 * strings separated by characters from delim.
 *
 * Writes NULs into the string at *stringp to end tokens.
 * delim need not remain constant from call to call.
 * On return, *stringp points past the last NUL written (if there might
 * be further tokens), or is NULL (if there are definitely no more tokens).
 *
 * Return the next token or NULL if there are no more tokens.
 */

/*
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
char *mystrsep(char **stringp, const char *delim)
  __attribute__((__nonnull__(1,2)));

/**
 * WARNING: Pad atleast 2 nulls on the end of your buffer
 * mystrtok - A simplified tokenizer based on a single delimiter.
 * @str: The string to be tokenized. The first call to my_strtok should pass the
 *       string to be tokenized, and subsequent calls should pass NULL.
 *
 * @delim: The delimiter character is used to tokenize the string.
 *
 * This function tokenizes the input string @str using the delimiter character
 * @delim. only supports a single delimiter character
 *
 * The function maintains a static internal state to keep track of the next token
 * position within the input string for subsequent calls.
 *
 * Return: Returns a pointer to the next token found in the string, or NULL if
 *         there are no more tokens to be found.
 */
char *mystrtok(char *str, char delim);

#endif // __MYSTRING_H
