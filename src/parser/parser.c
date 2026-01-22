#include "parser.h"
#include "parser_internal.h"
#include <stdlib.h>

parser_t *parser_create() {
  parser_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  ctx->token_consume_index = 0;
  ctx->ast.capacity = 0;
  ctx->ast.size = 0;
  return ctx;
}
void *parser_delete() {}

parser_error_e parser_parse(parser_t *ctx) {}

size_t parser_add_node(parser_t *ctx, node_t node);

node_id parser_parse_expresion(parser_t *ctx) {}
node_id parser_parse_term(parser_t *ctx) {}
node_id parser_parse_factor(parser_t *ctx) {}

parser_error_e parser_ast_arena_resize(parser_t *ctx, size_t size) {}
