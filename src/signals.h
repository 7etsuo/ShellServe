/**
 * @file signals.h
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
#ifndef __SIGNALS_H
#define __SIGNALS_H

/**
 * sigchld_handler() - Handles SIGCHLD signal.
 *
 * This function is triggered when a SIGCHLD signal is received, indicating a child process
 * has changed state. It reaps all terminated child processes and logs their exit statuses.
 *
 * @sig: The signal number (not used).
 */
void sigchld_handler(int sig);

/**
 * sigint_handler() - Handles SIGINT signal.
 *
 * This function is triggered when a SIGINT signal is received, usually from a user pressing Ctrl+C.
 * It logs a message and exits the program.
 *
 * @sig: The signal number (not used).
 */
void sigint_handler(int sig);

/**
 * install_handlers() - Installs signal handlers.
 *
 * This function installs the sigchld_handler and sigint_handler functions to handle SIGCHLD and SIGINT
 * signals, respectively.
 */
void install_handlers(void);

#endif // __SIGNALS_H
