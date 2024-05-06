#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXREAD 0x1000 /* page size */
#define SZPARTITION 0x08
#define MAXPATH 256

typedef enum { LOCAL, REMOTE } AddressType;

struct NodeAddress {
  AddressType type;
  union {
    char local_path[MAXPATH + 1];
    struct {
      char* ip_addr;
      int port;
    } network_info;
  };
};

struct ChunkInfo {
  int chunk_id;
  int offset;
  int size;
  struct NodeAddress node_address;
};

// TODO: RedBlack Tree
struct DFSNode {
  char filename[MAXPATH + 1];
  int num_chunks;
  struct ChunkInfo* chunk_list;
  struct DFSNode* left;
  struct DFSNode* right;
};

void partidion_data(void*, size_t, size_t);
void* mymemcpy(void* dest, const void* src, size_t n);
void concat_paths(const char* base, const char* relative, char* result,
                  size_t size);
void generate_chunk_name(const char* base_filename, int chunk_id, char* result);
char* int_to_str(int num, char* str);
void reverse_str(char* start, char* end);
size_t mystrlen(const char* str);

int main(int argc, char** argv) {
  int fd = open("blob", O_RDONLY);
  if (fd == -1) {
    perror("open() error");
    exit(EXIT_FAILURE);
  }

  char read_buf[MAXREAD + 1] = {0};
  ssize_t n_read;
  while ((n_read = read(fd, read_buf, MAXREAD)) > 0)
    partidion_data(read_buf, n_read, SZPARTITION);

  if (n_read == -1) {
    perror("read() error");
    exit(EXIT_FAILURE);
  }

  close(fd);

  return 0;
}

void partidion_data(void* buf, size_t buf_size, size_t partition_size) {
  size_t n_partitions = buf_size / partition_size;
  size_t n_remaining = buf_size % partition_size;
  size_t n_chunks = n_partitions + n_remaining;

  struct ChunkInfo chunks[n_chunks];
  AddressType type = LOCAL;

  for (size_t i = 0; i < n_partitions; i++) {
    char* partition = (char*)malloc(partition_size * sizeof(char));
    if (!partition) {
      perror("malloc error");
      exit(EXIT_FAILURE);
    }

    mymemcpy(partition, &((char*)buf)[i * partition_size], partition_size);

    for (size_t j = 0; j < partition_size; j++) {
      putchar(partition[j]);
    }

    chunks[i].size = partition_size;
    chunks[i].offset = (i * partition_size);
    chunks[i].chunk_id = i;
    chunks[i].node_address.type = type;

    char relative[MAXPATH + 1];  // TODO: This will be the filename for our DFS
                                 // metadata struct
    generate_chunk_name("blob.txt", chunks[i].chunk_id, relative);

    char base_path[MAXPATH + 1];
    getcwd(base_path, MAXPATH);
    concat_paths(base_path, relative, chunks[i].node_address.local_path,
                 MAXPATH);
    puts(chunks[i].node_address.local_path);
  }

  // TODO: make chunk for n_remaining
}

void* mymemcpy(void* dest, const void* src, size_t n) {
  char* d = dest;
  const char* s = src;

  while (n--) *d++ = *s++;

  return dest;
}

void concat_paths(const char* base, const char* relative, char* buf,
                  size_t size) {
  char* ptr = buf;
  while (*base && size--) *ptr++ = *base++;

  *ptr++ = '/';
  size--;

  while (*relative && size--) *ptr++ = *relative++;

  *ptr = '\0';
}

size_t mystrlen(const char* str) {
  const char* s = str;
  while (*s++);

  return s - str;
}

void generate_chunk_name(const char* base_filename, int chunk_id, char* buf) {
  int num_digits_and_null = 5;
  int i = 0, j = 0;
  // prefix chunk with username


  while (base_filename[i] != '\0' && base_filename[i] != '.')
    buf[j++] = base_filename[i++];

  const char suffix[] = "_chunk_";
  size_t len = mystrlen(suffix) - 1;
  for (int k = 0; k < len; k++) {
    buf[j++] = suffix[k];
  }

  char chunk_id_str[num_digits_and_null];
  char* p = int_to_str(chunk_id, chunk_id_str);

  for (int k = 0; p[k] && k < num_digits_and_null - 1; k++) {
    buf[j++] = p[k];
  }

  if (base_filename[i] == '.') {
    while (base_filename[i]) {
      buf[j++] = base_filename[i++];
    }
  }
  buf[j] = '\0';
}

void reverse_str(char* start, char* end) {
  while (start < end) {
    *start = *start ^ *end;
    *end = *start ^ *end;
    *start = *start ^ *end;

    start++;
    end--;
  }
}

char* int_to_str(int num, char* str) {
  char* p = str;
  do {
    *p++ = '0' + (num % 10);
    num /= 10;
  } while (num > 0);
  *p = '\0';

  reverse_str(str, p - 1);

  return str;
}
