#include "lexer.h"
#include "parser.h"
#include "token_stream.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main() {
  // Input some data
  const char *input = "234 + 3 * 2 / 4 + 549";

  lexer_t *lexer = lexer_create();
  if (!lexer) {
    printf("Error creating lexer");
    return 1;
  }

  token_stream_t *token_stream = token_stream_create();
  if (!token_stream) {
    printf("Error creating token stream");
    return 1;
  }

  lexer_error_e error;
  error = lexer_set_raw_data(lexer, input, strlen(input));
  if (error != LEXER_ERROR_NONE) {
    printf("Lexer error code %d", error);
  }

  lexer_set_token_stream(lexer, token_stream);

  error = lexer_process_data(lexer);
  if (error != LEXER_ERROR_NONE) {
    printf("Lexer error code: %d\n", error);
    return -1;
  }

  lexer_debug_print_tokens(lexer);

  parser_t *parser = parser_create();
  if (!parser) {
    printf("Error creating parser");
    return 1;
  }

  token_stream_delete(token_stream);
  lexer_free(lexer);

  return 0;
}
