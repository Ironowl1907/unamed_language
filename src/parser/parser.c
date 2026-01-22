#include "parser.h"
#include "parser_internal.h"
#include "token_stream.h"
#include <assert.h>
#include <stdlib.h>

parser_t *parser_create() {
  parser_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  // TODO: Remove magical number 16
  ctx->ast.nodes = malloc(sizeof(*ctx->ast.nodes) * 16);
  if (!ctx->ast.nodes)
    return NULL;

  ctx->token_consume_index = 0;
  ctx->ast.capacity = 16;
  ctx->ast.size = 0;
  return ctx;
}
void parser_delete(parser_t *ctx) {
  if (ctx) {
    free(ctx->ast.nodes);
    free(ctx);
  }
}

parser_error_e parser_parse(parser_t *ctx) {
  // TODO: Implement
  return PARSER_ERROR_NONE;
}

size_t parser_add_node(parser_t *ctx, node_t node);

node_id parser_parse_expresion(parser_t *ctx) {}
node_id parser_parse_term(parser_t *ctx) {}
node_id parser_parse_factor(parser_t *ctx) {}

parser_error_e parser_ast_arena_resize(parser_t *ctx, size_t size) {}

token_t parser_consume_token(parser_t *ctx, const token_t * token) {
  assert(ctx);

  token = token_stream_get(ctx->tokens, ctx->token_consume_index++);
}
token_t parser_peek_token(parser_t *ctx) {}
