#include "../lexer.h"

struct lexer {
  char raw_data[LEXER_RAW_BUFFER_SIZE];
  token_t tokens[LEXER_TOKEN_BUFFER_SIZE];

  uint32_t cursor;
  uint32_t token_count;
};

// Helpers
uint8_t is_numeric(const char c);
