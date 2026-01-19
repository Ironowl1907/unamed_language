#ifndef LEXER_H
#define LEXER_H

// Expresion
// Term
// Factor

#define LEXER_RAW_BUFFER_SIZE 256
#define LEXER_TOKEN_BUFFER_SIZE 256
#define TOKEN__BUFFER_SIZE 16

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
typedef enum token_type {
  SUM,
  RES,
  DIV,
  MUL,
  OPEN_PARENTHESIS,
  CLOSE_PARENTHESIS,
  NEGATION,
  EOL,
} token_type_e;

typedef struct {
  char data[TOKEN__BUFFER_SIZE];
  token_type_e type;
} token_t;

typedef struct {
  char raw_data[LEXER_RAW_BUFFER_SIZE];
  token_t tokens[LEXER_TOKEN_BUFFER_SIZE];

  uint32_t consume_index;
  uint32_t next_token_index;

} lexer_t;

lexer_t *lexer_init();
void lexer_destroy(lexer_t *ctx);

int lexer_set_raw_data(lexer_t *ctx, const char *raw_data, size_t size);
const char *lexer_get_raw_data(lexer_t *ctx);

void lexer_process_data(lexer_t *ctx, token_t *token_buffer, size_t size);

char lexer_consume_char(lexer_t *ctx);
char lexer_peak_char(lexer_t *ctx);

// Debug
void lexer_debug_print_tokens(const lexer_t *ctx);

#endif
