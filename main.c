#include "src/lexer.h"
#include <stddef.h>
#include <stdio.h>

int main() {
  // Input some data
  const char *input = "2 + 3 * 2";
  const size_t size = 9;

  lexer_t *lexer = lexer_init();
  if (!lexer)
    printf("Error creating lexer");

  lexer_set_raw_data(lexer, input, size);

  lexer_process_data(lexer);

  lexer_debug_print_tokens(lexer);

  lexer_destroy(lexer);

  return 0;
}
