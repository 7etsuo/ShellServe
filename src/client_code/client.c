/**
 * @file client.c
 *
 * @author 7etsuo
 * @date 2023
 *
 * Description:
 * The main entry point for the client-side application. This program handles
 * the initialization of client connections, user input/output, and file transfer
 * operations with the server.
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
#include "../client_core.h"
#include "../syscalls.h"
#include "../filetransfer.h"

void
apprunner(int argc, char *argv[])
{
  struct MyIO io;

  initclient(argc, argv, &io);
  runclient(&io);
  myclose(io.sockfd);
}

int
main(int argc, char *argv[])
{
  apprunner(argc, argv);

  return 0;
}

