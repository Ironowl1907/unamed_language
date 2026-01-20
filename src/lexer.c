#include "lexer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

lexer_t *lexer_init(void) {
  lexer_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  ctx->consume_index = 0;
  ctx->next_token_index = 0;
  return ctx;
}
void lexer_destroy(lexer_t *ctx) {
  if (!ctx)
    free(ctx);
}

int lexer_set_raw_data(lexer_t *ctx, const char *raw_data, size_t size) {
  if (!ctx || !raw_data)
    return -1;

  if (size >= LEXER_RAW_BUFFER_SIZE)
    return -1;

  memcpy(ctx->raw_data, raw_data, size);
  ctx->raw_data[size] = '\0';

  return 0;
}
const char *lexer_get_raw_data(lexer_t *ctx) { return ctx->raw_data; }

void lexer_process_data(lexer_t *ctx, token_t *token_buffer, size_t size) {
  uint32_t read_index = 0;
  while (ctx->raw_data[read_index++] != '\0') {
    char a = lexer_consume_char(ctx);

    // switch (a) { case '+': }
  }

  printf("\n");
}

void lexer_append_token(lexer_t *ctx, const char *data, token_type_e type) {
  size_t last_index = ctx->next_token_index;
  ctx->tokens[last_index].type = type;
  memcpy(ctx->tokens[last_index].data, data, strlen(data));

  ++ctx->next_token_index;
}

void lexer_debug_print_tokens(const lexer_t *ctx) {
  for (uint32_t i = 0; i < ctx->next_token_index; ++i) {
    printf("Token Type: %d, Token data: %s", ctx->tokens[i].type,
           ctx->tokens[i].data);
  }
  printf("\n");
}

char lexer_consume_char(lexer_t *ctx) {
  return ctx->raw_data[ctx->consume_index++];
}
char lexer_peak_char(lexer_t *ctx) { return ctx->raw_data[ctx->consume_index]; }
