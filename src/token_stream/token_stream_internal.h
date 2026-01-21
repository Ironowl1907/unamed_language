#ifndef TOKEN_STREAM_INTERNAL_H
#define TOKEN_STREAM_INTERNAL_H

#include "lexer.h"
#include "token_stream.h"
#include <stddef.h>

struct token_stream {
  token_t *arr;
  size_t size;
};

void token_stream_resize(token_stream_t *ctx, size_t new_size);

#endif
