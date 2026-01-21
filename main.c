#include "lexer.h"
#include "token_stream.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main() {
  // Input some data
  const char *input = "234 + 3 * 2";

  lexer_t *lexer = lexer_create();
  if (!lexer)
    printf("Error creating lexer");

  token_stream_t *token_stream = token_stream_create();
  if (!token_stream)
    printf("Error creating token stream");

  lexer_set_raw_data(lexer, input, strlen(input));

  lexer_set_token_stream(lexer, token_stream);

  lexer_process_data(lexer);

  lexer_debug_print_tokens(lexer);

  lexer_free(lexer);

  return 0;
}
