/**
 * @file globals.h
 * @brief Global Definitions and Debugging Flags
 *
 * This file contains global enumerations, definitions, and includes necessary for
 * managing debugging levels and including specific types across the application.
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
#ifndef __GLOBAL_H
#define __GLOBAL_H

#define	__need_NULL
#define __need_size_t
#include <stddef.h>
#include <sys/types.h> /* ssize_t */

#define MAX_DATA_SIZE 0x1000 /* page sized */
#define MAXDATASIZE MAX_DATA_SIZE
#define MAX_LINE_SIZE 1024
#define MAX_PROMPT_SIZE 256
#define MAX_USER_NAME 256
#define MAX_NUM_ARGS 16
#define MAX_PATH_SIZE MAX_LINE_SIZE

/**
 * @enum debug
 * @brief Debugging level enumeration.
 *
 * This enumeration defines the debugging levels that can be used throughout the application
 * to conditionally compile or execute code based on the current debugging level.
 */
enum debug {
  NONE,
  SYSCALLDEBUG,
  SIGNALDEBUG
};

extern char **g_envp;

#endif // __GLOBAL_H

