/**
 * @file server.c
 *
 * @author 7etsuo
 * @date 2023
 *
 * Description:
 * This is the main entry point for the server-side application. It is responsible for
 * initializing server data, setting up signal handlers, and launching the main server loop.
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
#include "../server_core.h"
#include "../signals.h"
#include "../clientlogin.h"

void
apprunner(void)
{
  ServerData server;

  load_credentials("credentials.txt");
  install_handlers();
  initserver(&server);
  runserver(&server);
}

int
main(int argc, char **argv, char **envp)
{
  g_envp = envp;
  apprunner();

  return 0;
}

