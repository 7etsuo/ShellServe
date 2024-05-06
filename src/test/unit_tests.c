/**
 * @file unit_tests.c
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
#include <check.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "../client_core.h"
#include "../syscalls.h"
#include "../server_core.h"
#include "../filetransfer.h"

#ifndef READ_END
#define READ_END 0
#endif /* READ_END */

#ifndef WRITE_END
#define WRITE_END 1
#endif /* WRITE_END */


START_TEST(test_mysckread_success)
{
  int pipefd[2];
  char buffer[5] = {0};
  char *data = "data";
  size_t nread;

  pipe(pipefd);
  ck_assert(write(pipefd[WRITE_END], data, 4) != -1);

  nread = mysckread(pipefd[READ_END], buffer, 4);

  ck_assert_int_eq(nread, 4);
  ck_assert_str_eq(buffer, "data");

  ck_assert(close(pipefd[READ_END]) != -1);
  ck_assert(close(pipefd[WRITE_END]) != -1);
}
END_TEST

START_TEST(test_mysckread_large_buffer)
{
  int sockets[2];
  char send_buffer[10000];
  char recv_buffer[10000];
  ssize_t nread;

  for (int i = 0; i < 10000; i++) {
    send_buffer[i] = i % 256;
  }

  ck_assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) != -1);

  write(sockets[0], send_buffer, sizeof(send_buffer));

  nread = mysckread(sockets[1], recv_buffer, sizeof(recv_buffer));

  ck_assert_int_eq(nread, sizeof(send_buffer));
  ck_assert(memcmp(send_buffer, recv_buffer, sizeof(send_buffer)) == 0);

  ck_assert(close(sockets[0]) != -1);
  ck_assert(close(sockets[1]) != -1);
}
END_TEST

START_TEST(test_mysckwrite_success)
{
  int pipefd[2];
  char buffer[5] = {0};
  char *data = "data";
  size_t nwritten;

  pipe(pipefd);
  nwritten = mysckwrite(pipefd[WRITE_END], data, 4);
  ck_assert(read(pipefd[READ_END], buffer, 4) != -1);
  ck_assert_str_eq(buffer, "data");
  ck_assert_int_eq(nwritten, 4);

}
END_TEST

START_TEST(test_mymalloc_and_myfree_success)
{
  char pattern = 0x41;
  char *mem = NULL;
  size_t size = 1024;
  void *addr = mymalloc(size);
  ck_assert(addr != NULL);
  memset(addr, pattern, size);

  mem = (char *)addr;
  for (size_t i = 0; i < size; i++) {
    ck_assert_int_eq(mem[i], pattern);
  }

  myfree(addr, size);
}
END_TEST

Suite *
system_suite(void)
{
  Suite *s = suite_create("Syscalls Test Suite");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_mysckread_success);
  tcase_add_test(tc_core, test_mysckread_large_buffer);
  tcase_add_test(tc_core, test_mysckwrite_success);
  tcase_add_test(tc_core, test_mymalloc_and_myfree_success);
  suite_add_tcase(s, tc_core);

  return s;
}

Suite *
client_interaction_suite(void)
{
  return NULL;
}

int
main(void)
{
  int nfailed;
  Suite *s = system_suite();
  SRunner *sr = srunner_create(s);

  srunner_set_log(sr, "test.log");
  srunner_set_tap(sr, "/dev/stdout");

  /* can also add CK_VERBOSE here which is basically test.log */
  srunner_run_all(sr, CK_NORMAL);
  nfailed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nfailed;
}

