#include "token_stream.h"
#include "token_stream_internal.h"
#include <stdlib.h>

token_stream_t *token_stream_create(void) {
  token_stream_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  ctx->size = TOKEN_BUFFER_SIZE;

  ctx->arr = malloc(ctx->size * sizeof(token_t));
  return ctx;
}
void token_stream_delete(token_stream_t *ctx) {
  if (ctx) {
    free(ctx->arr);
    free(ctx);
  }
}

void token_stream_append(token_stream_t *ctx, token_t token) {}

inline token_t token_stream_get_index(token_stream_t *ctx, size_t index) {
  return ctx->arr[index];
}
