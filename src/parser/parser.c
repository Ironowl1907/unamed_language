#include "parser.h"
#include "parser_internal.h"
#include "token_stream.h"
#include <assert.h>
#include <stdarg.h>
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
  ctx->ast.size = 1; // 0 Reserved to error

  ctx->last_error = PARSER_ERROR_NONE;
  ctx->error_msg[0] = '\0';
  return ctx;
}
void parser_delete(parser_t *ctx) {
  if (ctx) {
    free(ctx->ast.nodes);
    free(ctx);
  }
}

parser_error_e parser_parse(parser_t *ctx) {
  node_id ast = parser_parse_expression(ctx);
  if (ast == 0) {
    return PARSER_ERROR_WRONG_SINTAXIS;
  }
  parser_print_debug(ctx, ast);
  return PARSER_ERROR_NONE;
}

node_id parser_parse_expression(parser_t *ctx) {
  node_id a = parser_parse_term(ctx);
  if (a == 0)
    return 0;

  for (;;) {
    token_t operand = parser_peek_token(ctx);

    if (operand.type == TOKEN_TYPE_SUM) {
      parser_consume_token(ctx);
      node_id b = parser_parse_term(ctx);
      if (b == 0)
        return 0;
      a = parser_add_node(ctx, (node_t){.kind = NODE_KIND_BINARY_OP,
                                        .as.binary.left = a,
                                        .as.binary.right = b,
                                        .as.binary.op = OP_ADD});
      if (a == 0)
        return 0;
    } else if (operand.type == TOKEN_TYPE_RES) {
      parser_consume_token(ctx);
      node_id b = parser_parse_term(ctx);
      if (b == 0)
        return 0;
      a = parser_add_node(ctx, (node_t){.kind = NODE_KIND_BINARY_OP,
                                        .as.binary.left = a,
                                        .as.binary.right = b,
                                        .as.binary.op = OP_SUB});
      if (a == 0)
        return 0;
    } else {
      return a;
    }
  }
}

node_id parser_parse_term(parser_t *ctx) {
  node_id a = parser_parse_factor(ctx);
  if (a == 0)
    return 0;

  for (;;) {
    token_t operand = parser_peek_token(ctx);

    if (operand.type == TOKEN_TYPE_MUL) {
      parser_consume_token(ctx);
      node_id b = parser_parse_factor(ctx);
      if (b == 0)
        return 0;
      a = parser_add_node(ctx, (node_t){.kind = NODE_KIND_BINARY_OP,
                                        .as.binary.left = a,
                                        .as.binary.right = b,
                                        .as.binary.op = OP_MUL});
      if (a == 0)
        return 0;
    } else if (operand.type == TOKEN_TYPE_DIV) {
      parser_consume_token(ctx);
      node_id b = parser_parse_factor(ctx);
      if (b == 0)
        return 0;
      a = parser_add_node(ctx, (node_t){.kind = NODE_KIND_BINARY_OP,
                                        .as.binary.left = a,
                                        .as.binary.right = b,
                                        .as.binary.op = OP_DIV});
      if (a == 0)
        return 0;
    } else {
      return a;
    }
  }
}

node_id parser_parse_factor(parser_t *ctx) {
  token_t tok = parser_peek_token(ctx);

  if (tok.type == TOKEN_TYPE_NUMBER) {
    token_t a = parser_consume_token(ctx);
    return parser_add_node(
        ctx, (node_t){.kind = NODE_KIND_NUMBER, .as.number = a.data});
  }

  if (tok.type == TOKEN_TYPE_NEGATION) {
    parser_consume_token(ctx);
    node_id n = parser_parse_factor(ctx);
    if (!n) {
      parser_set_error(ctx, PARSER_ERROR_WRONG_SINTAXIS,
                       "Expected expression after unary '-'");
      return 0;
    }
    return parser_add_node(ctx, (node_t){.kind = NODE_KIND_UNARY_OP,
                                         .as.unary.op = OP_NEG,
                                         .as.unary.operand = n});
  }

  parser_set_error(ctx, PARSER_ERROR_WRONG_SINTAXIS,
                   "Unexpected token at position %zu",
                   ctx->token_consume_index);
  return 0;
}

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

static const char *op_to_string(operator_e op) {
  switch (op) {
  case OP_ADD:
    return "+";
  case OP_SUB:
    return "-";
  case OP_MUL:
    return "*";
  case OP_DIV:
    return "/";
  case OP_NEG:
    return "NEG";
  default:
    return "UNKNOWN_OP";
  }
}

static void print_indent(size_t depth) {
  for (size_t i = 0; i < depth; ++i)
    printf("  ");
}

static void parser_print_debug_node(parser_t *ctx, node_id id, size_t depth) {
  if (!ctx || id == 0 || id >= ctx->ast.size) {
    print_indent(depth);
    printf("<invalid node %zu>\n", id);
    return;
  }

  node_t *node = &ctx->ast.nodes[id];

  print_indent(depth);

  switch (node->kind) {
  case NODE_KIND_NUMBER:
    printf("Number(%f)\n", node->as.number);
    break;

  case NODE_KIND_UNARY_OP:
    printf("UnaryOp(%s)\n", op_to_string(node->as.unary.op));
    parser_print_debug_node(ctx, node->as.unary.operand, depth + 1);
    break;

  case NODE_KIND_BINARY_OP:
    printf("BinaryOp(%s)\n", op_to_string(node->as.binary.op));
    parser_print_debug_node(ctx, node->as.binary.left, depth + 1);
    parser_print_debug_node(ctx, node->as.binary.right, depth + 1);
    break;

  case NODE_KIND_INVALID:
  default:
    printf("InvalidNode\n");
    break;
  }
}

void parser_print_debug(parser_t *ctx, node_id ast) {
  if (!ctx) {
    printf("<null parser>\n");
    return;
  }

  printf("AST Debug Print (root = %zu):\n", ast);
  parser_print_debug_node(ctx, ast, 0);
}

void parser_set_error(parser_t *ctx, parser_error_e code, const char *fmt,
                      ...) {
  if (!ctx)
    return;

  ctx->last_error = code;

  if (!fmt) {
    ctx->error_msg[0] = '\0';
    return;
  }

  va_list args;
  va_start(args, fmt);
  vsnprintf(ctx->error_msg, sizeof(ctx->error_msg), fmt, args);
  va_end(args);
}

const char *parser_get_error_string(parser_t *ctx) {
  if (!ctx || ctx->last_error == PARSER_ERROR_NONE)
    return NULL;
  return ctx->error_msg;
}

parser_error_e parser_get_error_code(parser_t *ctx) {
  if (!ctx)
    return PARSER_ERROR_NULL_PARAMETER;
  return ctx->last_error;
}
