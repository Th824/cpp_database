#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct InputBuffer {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
};

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

// 定义主要命令（如exit...）的执行结果
enum MetaCommandResult {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
};

enum PrepareResult { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT };

enum StatementType { STATEMENT_INSERT, STATEMENT_SELECT };

struct Statement {
  StatementType type;
};

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } else {
    return MetaCommandResult::META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement &statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement.type = StatementType::STATEMENT_INSERT;
    return PrepareResult::PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement.type = StatementType::STATEMENT_SELECT;
    return PrepareResult::PREPARE_SUCCESS;
  }
  return PrepareResult::PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement &statement) {
  switch (statement.type) {
    case (StatementType::STATEMENT_INSERT):
      printf("This is where we would do an insert.\n");
      break;
    case (StatementType::STATEMENT_SELECT):
      printf("This is where we would do a select.\n");
      break;
  }
}

int main(int argc, char *argv[]) {
  InputBuffer *input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case (MetaCommandResult::META_COMMAND_SUCCESS):
          continue;
        case (MetaCommandResult::META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }

    Statement Statement;
    switch (prepare_statement(input_buffer, Statement)) {
      case (PrepareResult::PREPARE_SUCCESS):
        break;
      case (PrepareResult::PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",
               +input_buffer->buffer);
        continue;
    }

    execute_statement(Statement);
    printf("Executed.\n");
  }
}