#ifndef __PIPELINE_H
#define __PIPELINE_H

#define __need_size_t
#include <stddef.h> /* for size_t */

#include "globals.h"
#include "server_core.h"

/**
 * enum pipefd - Enum for indexing pipe file descriptors
 * @READ_END: Index for the read end of the pipe
 * @WRITE_END: Index for the write end of the pipe
 * @FDLEN: Total number of file descriptor ends
 *
 * Used for organizing the file descriptors within a pipe.
 * READ_END is for the reading end, and WRITE_END is for the writing end.
 * FDLEN provides the total number of descriptor ends for sizing arrays, loops, etc.
 */
enum pipefd {
  READ_END,
  WRITE_END,
  FDLEN
};

/**
 * struct __Pipeline - Structure for holding pipeline command and I/O info
 * @argv: Array of pointers to argument strings
 * @fin: Pointer to input file name string
 * @fout: Pointer to output file name string
 * @argc: Count of arguments in argv
 * @fd: Array of pipe file descriptors
 * @sockfd: Socket file descriptor
 *
 * This structure holds all the relevant information for a command pipeline,
 * including the command arguments, any file I/O redirections, and file descriptors.
 */
typedef struct __Pipeline {
  char *argv[MAX_NUM_ARGS];
  char *fin;
  char *fout;
  int argc;
  int fd[FDLEN];
  int sockfd;
} Pipeline;

/**
 * close_pipes - Close all pipe file descriptors in a pipeline array
 * @pipe: Pointer to the array of Pipeline structures
 * @npipes: Number of pipes to close
 *
 * Iterates over each Pipeline structure in the array and closes its
 * read and write file descriptors.
 */
void close_pipes(Pipeline *pipe, size_t npipes);

/**
 * init_pipesfd - Initialize pipe file descriptors in a pipeline array
 * @pipe: Pointer to the array of Pipeline structures
 * @npipes: Number of pipelines
 *
 * Iterates over each Pipeline structure in the array and initializes its
 * pipe file descriptors.
 */
void init_pipesfd(Pipeline *pipe, size_t npipes);

/**
 * build_pipeline - Constructs a pipeline from client data
 * @pipe: Pointer to the array of Pipeline structures
 * @client: Pointer to ClientData structure containing client information
 * @readbuf: Pointer to buffer containing the command to parse
 *
 * Return: Number of pipelines created
 */
int build_pipeline(Pipeline *pipe, ClientData * const client, char * const readbuf)
  __attribute__((__nonnull__(1, 2)));

/**
 * parse_pipeline - Parses command-line arguments to populate a Pipeline array
 * @pipe: Pointer to the array of Pipeline structures
 * @argc: Count of arguments passed
 * @argv: Pointer to array of argument strings
 *
 * Return: Number of commands parsed, +1
 */
int parse_pipeline(Pipeline *pipe, int argc, char *argv[])
  __attribute__((__nonnull__(1, 3)));

/**
 * run_pipeline - Execute the commands in the pipelines
 * @pipe: Pointer to the array of Pipeline structures
 * @npipes: Number of pipelines
 *
 * Iterates over each Pipeline in the array, forking and executing the commands.
 */
void run_pipeline(Pipeline *pipe, size_t npipes);

/**
 * init_pipeline - Initialize a single Pipeline structure
 * @pipe: Pointer to the Pipeline structure to initialize
 * @fd: File descriptor to associate with the Pipeline's sockfd
 *
 * Initialize the argv pointers to NULL, and file descriptors to -1.
 */
void init_pipeline(Pipeline *pipe, int fd);

/**
 * init_pipelines - Initialize an array of Pipeline structures
 * @pipes: Array of Pipeline structures to initialize
 * @fd: File descriptor to associate with each Pipeline's sockfd
 *
 * Iterates over each Pipeline structure in the array and initializes it.
 */
void init_pipelines(Pipeline pipes[MAX_NUM_ARGS], int fd);

#endif /* __PIPELINE_H */

