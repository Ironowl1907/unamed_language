#include "parser.h"
#include "parser_internal.h"
#include "token_stream.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

parser_t *parser_create() {
  parser_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  // TODO: Remove magical number 16
  ctx->ast.nodes = malloc(sizeof(*ctx->ast.nodes) * 16);
  if (!ctx->ast.nodes) {
    free(ctx);
    return NULL;
  }

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

node_id parser_parse_expression(parser_t *ctx) {
  node_id a = parser_parse_term(ctx);

  for (;;) {
    token_t operand = parser_peek_token(ctx);

    if (operand.type == TOKEN_TYPE_SUM) {
      parser_consume_token(ctx);
      node_id b = parser_parse_term(ctx);
      a = parser_add_node(ctx, (node_t){.kind = NODE_KIND_BINARY_OP,
                                        .as.binary.left = a,
                                        .as.binary.right = b,
                                        .as.binary.op = OP_ADD});
    } else if (operand.type == TOKEN_TYPE_RES) {
      parser_consume_token(ctx);
      node_id b = parser_parse_term(ctx);
      a = parser_add_node(ctx, (node_t){.kind = NODE_KIND_BINARY_OP,
                                        .as.binary.left = a,
                                        .as.binary.right = b,
                                        .as.binary.op = OP_SUB});
    } else {
      return a;
    }
  }
}

node_id parser_parse_term(parser_t *ctx) {}
node_id parser_parse_factor(parser_t *ctx) {}

size_t parser_add_node(parser_t *ctx, node_t node) {
  assert(ctx != NULL);
  if (ctx->ast.size == ctx->ast.capacity) {
    // TODO: Error checking
    parser_error_e error = parser_ast_arena_resize(ctx, ctx->ast.capacity * 2);
  }
  ctx->ast.nodes[ctx->ast.size] = node;

  return ctx->ast.size++;
}

parser_error_e parser_ast_arena_resize(parser_t *ctx, size_t size) {
  if (!ctx)
    return PARSER_ERROR_NULL_PARAMETER;

  node_t *new_arr = realloc(ctx->ast.nodes, size * sizeof *ctx->ast.nodes);
  if (!new_arr)
    return PARSER_ERROR_INSUFFICIENT_MEMORY;

  ctx->ast.nodes = new_arr;
  ctx->ast.capacity = size;

  return PARSER_ERROR_NONE;
}

token_t parser_consume_token(parser_t *ctx) {
  return token_stream_get(ctx->tokens, ctx->token_consume_index++);
}
token_t parser_peek_token(parser_t *ctx) {
  return token_stream_get(ctx->tokens, ctx->token_consume_index);
}
