#include "lexer.h"
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

lexer_t *lexer_create(void) {
  lexer_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  ctx->cursor = 0;
  ctx->token_count = 0;
  return ctx;
}
void lexer_free(lexer_t *ctx) {
  if (ctx)
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
const char *lexer_get_raw_data(const lexer_t *ctx) { return ctx->raw_data; }

void lexer_process_data(lexer_t *ctx) {
  while (lexer_peek_char(ctx) != '\0') {
    char a = lexer_consume_char(ctx);

    if (isspace((unsigned char)a))
      continue;

    switch (a) {
    case '+':
      lexer_append_token(ctx, TOKEN_TYPE_SUM, NULL);
      break;
    case '-':
      lexer_append_token(ctx, TOKEN_TYPE_RES, NULL);
      break;
    case '*':
      lexer_append_token(ctx, TOKEN_TYPE_MUL, NULL);
      break;
    case '/':
      lexer_append_token(ctx, TOKEN_TYPE_DIV, NULL);
      break;
    case '(':
      lexer_append_token(ctx, TOKEN_TYPE_OPEN_PARENTHESIS, NULL);
      break;
    case ')':
      lexer_append_token(ctx, TOKEN_TYPE_CLOSE_PARENTHESIS, NULL);
      break;
    default:
      if (is_numeric(a)) {
        char buffer[TOKEN_BUFFER_SIZE];
        size_t index = 0;

        buffer[index++] = a;

        while (is_numeric(lexer_peek_char(ctx))) {
          buffer[index++] = lexer_consume_char(ctx);
        }

        buffer[index] = '\0';
        lexer_append_token(ctx, TOKEN_TYPE_NUMBER, buffer);
      } else {
        printf("Unknown character: '%c'\n", a);
      }
      break;
    }
  }
  lexer_append_token(ctx, TOKEN_TYPE_EOF, NULL);
}

void lexer_append_token(lexer_t *ctx, token_type_e type, const char *data) {
  if (ctx->token_count >= LEXER_TOKEN_BUFFER_SIZE)
    return;

  size_t i = ctx->token_count;

  ctx->tokens[i].type = type;

  if (data) {
    strncpy(ctx->tokens[i].data, data, TOKEN_BUFFER_SIZE - 1);
    ctx->tokens[i].data[TOKEN_BUFFER_SIZE - 1] = '\0';
  } else {
    ctx->tokens[i].data[0] = '\0';
  }

  ctx->token_count++;
}

void lexer_debug_print_tokens(const lexer_t *ctx) {
  uint32_t index = 0;
  for (;;) {
    if (ctx->tokens[index].type == TOKEN_TYPE_EOF)
      break;
    printf("TOKEN: %d", ctx->tokens[index].type);
    if (ctx->tokens[index].data[0] != '\0')
      printf(" Data: %s\n", ctx->tokens[index].data);
    else
      printf("\n");
    ++index;
  }
  printf("\n");
}

char lexer_consume_char(lexer_t *ctx) {
  return ctx->raw_data[ctx->cursor++];
}
char lexer_peek_char(const lexer_t *ctx) { return ctx->raw_data[ctx->cursor]; }

uint8_t is_numeric(const char c) { return (int)c > 47 && (int)c < 58; }
