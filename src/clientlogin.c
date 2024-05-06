#include "clientlogin.h"
#include "mystring.h"
#include "syscalls.h"

#include <unistd.h>
#include <fcntl.h>

/* begin private */
Credential __credentials[USER_MAX];
#define __MAXREAD 1024
int __ncredentials;
/* end private */

char *
get_username_at_index(int i)
{
  return __credentials[i].username;
}

int
verifyuser(const char * const username, const char * const password)
{
  for (int i = 0; i < __ncredentials; i++) {
    if (mystrcmp(username, __credentials[i].username) == 0 && mystrcmp(password, __credentials[i].password) == 0) {
      return i;
    }
  }
  return -1;
}

void
load_user_and_password(char *line, int index)
{
  char *token = mystrtok(line, ' ');
  if (token) {
    mystrncpy(__credentials[index].username, token, __MAXREAD-1);
    token = mystrtok(NULL, ' ');
  }
  if (token) {
    mystrncpy(__credentials[index].password, token, __MAXREAD-1);
  }
}

void
load_credentials(const char * const filename)
{
  char buffer[__MAXREAD], *line;
  size_t nread, i;

  int fd = myopen(filename, O_RDONLY, 0);

  /* - 10 in the read to avoid an overflow in mystrtok */
  mymemset(buffer, 0, sizeof(buffer) -1);
  while ((nread = myread(fd, buffer, sizeof(buffer) - 10)) > 0) {
    buffer[nread] = '\0';
    line = mystrtok(buffer, '\n');
    for (i = 0; line && i < USER_MAX; i++) {
      load_user_and_password(line, i);
      line = mystrtok(NULL, '\n');
    }
  }

  myclose(fd);
  __ncredentials = i;
}

