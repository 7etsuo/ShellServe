# Compiler
CC = gcc

# Compilation flags
CFLAGS = -Wall -O0 -g -MMD -MP -I/usr/local/include -I. -I./client_code -I./server_code

# Directories for objects
OBJ_DIR = obj

# Common sources and objects
COMMON_SRC_DIR = .
COMMON_SRCS = $(wildcard $(COMMON_SRC_DIR)/*.c)
COMMON_OBJS = $(patsubst $(COMMON_SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(COMMON_SRCS))
COMMON_DEPS = $(patsubst $(COMMON_SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(COMMON_SRCS))

# Client specific settings
CLIENT_SRC_DIR = client_code
CLIENT_SRCS = $(wildcard $(CLIENT_SRC_DIR)/*.c)
CLIENT_OBJS = $(patsubst $(CLIENT_SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CLIENT_SRCS))
CLIENT_DEPS = $(patsubst $(CLIENT_SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(CLIENT_SRCS))
CLIENT_EXEC = client

# Server specific settings
SERVER_SRC_DIR = server_code
SERVER_SRCS = $(wildcard $(SERVER_SRC_DIR)/*.c)
SERVER_OBJS = $(patsubst $(SERVER_SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SERVER_SRCS))
SERVER_DEPS = $(patsubst $(SERVER_SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(SERVER_SRCS))
SERVER_EXEC = server

# check unit testing
TEST_SRC_DIR = test
TEST_SRCS = $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))
TEST_DEPS = $(patsubst $(TEST_SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(TEST_SRCS))
TEST_EXEC = unit_tests
TEST_LIBS = -lcheck -lm -lrt -lpthread -lsubunit

all: setup $(CLIENT_EXEC) $(SERVER_EXEC) test

test: setup $(COMMON_OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(COMMON_OBJS) $(TEST_OBJS) $(TEST_LIBS)

$(CLIENT_EXEC): $(COMMON_OBJS) $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) $(COMMON_OBJS) $(CLIENT_OBJS)

$(SERVER_EXEC): $(COMMON_OBJS) $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) $(COMMON_OBJS) $(SERVER_OBJS)

$(OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(CLIENT_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SERVER_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC) $(TEST_EXEC)
	rm -rf $(OBJ_DIR)

setup:
	mkdir -p $(OBJ_DIR)

-include $(patsubst %,$(OBJ_DIR)/%,$(COMMON_DEPS) $(CLIENT_DEPS) $(SERVER_DEPS))

.PHONY: all clean setup

