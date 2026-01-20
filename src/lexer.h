#ifndef LEXER_H
#define LEXER_H

// Expresion
// Term
// Factor

#define LEXER_RAW_BUFFER_SIZE 256
#define LEXER_TOKEN_BUFFER_SIZE 256
#define TOKEN_BUFFER_SIZE 16

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum token_type {
  TOKEN_TYPE_SUM,
  TOKEN_TYPE_RES,
  TOKEN_TYPE_DIV,
  TOKEN_TYPE_MUL,
  TOKEN_TYPE_OPEN_PARENTHESIS,
  TOKEN_TYPE_CLOSE_PARENTHESIS,
  TOKEN_TYPE_NEGATION,
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_EOF,
} token_type_e;

typedef struct {
  char data[TOKEN_BUFFER_SIZE];
  token_type_e type;
} token_t;

typedef struct {
  char raw_data[LEXER_RAW_BUFFER_SIZE];
  token_t tokens[LEXER_TOKEN_BUFFER_SIZE];

  uint32_t cursor;
  uint32_t token_count;

} lexer_t;

lexer_t *lexer_create();
void lexer_free(lexer_t *ctx);

int lexer_set_raw_data(lexer_t *ctx, const char *raw_data, size_t size);
const char *lexer_get_raw_data(const lexer_t *ctx);

void lexer_process_data(lexer_t *ctx);

char lexer_consume_char(lexer_t *ctx);
char lexer_peek_char(const lexer_t *ctx);
void lexer_append_token(lexer_t *ctx, token_type_e type, const char *data);

// Debug
void lexer_debug_print_tokens(const lexer_t *ctx);

// Helpers
uint8_t is_numeric(const char c);

#endif
