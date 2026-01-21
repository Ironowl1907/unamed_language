#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include <stddef.h>
#define TOKEN_BUFFER_SIZE 16

typedef struct token_stream token_stream_t;

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

// clang-format off
token_stream_t * token_stream_create(void);
void token_stream_delete(token_stream_t *ctx);

void token_stream_append(token_stream_t *ctx, token_t token);

inline token_t token_stream_get_index(token_stream_t *ctx, size_t index);

#endif
