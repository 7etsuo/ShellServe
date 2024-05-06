#ifndef __CLIENT_LOGIN_H
#define __CLIENT_LOGIN_H

#define USER_MAX 1000
#define CREDENTIAL_LEN 256

typedef struct __Credential{
  char username[CREDENTIAL_LEN];
  char password[CREDENTIAL_LEN];
}Credential;

/**
 * verifyuser() - Verify username and password against stored credentials.
 * @username: User-provided username.
 * @password: User-provided password.
 *
 * Iterates through the globally stored credentials, comparing the provided
 * username and password. If a match is found, the index of the matching
 * credential is returned. Otherwise, 0 is returned indicating failure.
 *
 * Return: Index of the matching credential on success, 0 on failure.
 */
int verifyuser(const char * const username, const char * const password)
  __attribute__((__nonnull__(1, 2)));

/**
 * load_credentials() - Load credentials from a file.
 * @filename: Name of the file containing credentials.
 *
 * Reads a specified file for user credentials, each line expected
 * to contain a username and password separated by space. Credentials
 * are stored in the global __credentials array, and the total number
 * of loaded credentials is stored in __ncredentials.
 */
void load_credentials(const char * const filename)
  __attribute__((__nonnull__(1)));

/**
 * load_user_and_password() - Parse a line for username and password.
 * @line: Input line with credentials.
 * @index: Index in the global __credentials array to store the parsed data.
 *
 * Parses a given line for a username and password separated by space,
 * and stores them in the global __credentials array at the specified index.
 */
void load_user_and_password(char *line, int index);

/**
 * get_username_at_index() - Retrieve username at specified index.
 * @i: Index of the credential.
 *
 * Accesses the globally stored credentials and returns the username of
 * the credential at the specified index.
 *
 * Return: Username at the specified index.
 */
char *get_username_at_index(int i);

#endif // __CLIENT_LOGIN_H

