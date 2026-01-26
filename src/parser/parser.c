#include "parser.h"
#include "ast.h"
#include "parser_internal.h"
#include "token_stream.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

parser_t *parser_create(token_stream_t *token_stream) {
  parser_t *ctx = malloc(sizeof *ctx);
  if (!ctx) {
    free(ctx);
    return NULL;
  }

  ctx->tokens = token_stream;
  ctx->token_consume_index = 0;

  ctx->last_error = PARSER_ERROR_NONE;
  ctx->error_msg[0] = '\0';
  return ctx;
}
void parser_delete(parser_t *ctx) {
  if (ctx) {
    free(ctx);
  }
}

parser_error_e parser_parse(parser_t *ctx, ast_t **out_ast) {
  ctx->ast = ast_create();
  if (!ctx->ast) {
    return PARSER_ERROR_AST_CREATION;
  }

  ctx->ast->root = parser_parse_expression(ctx);
  if (ctx->ast == 0) {
    return PARSER_ERROR_WRONG_SINTAXIS;
  }
  *out_ast = ctx->ast;
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
      a = ast_add_node(ctx->ast, (node_t){.kind = NODE_KIND_BINARY_OP,
                                          .as.binary.left = a,
                                          .as.binary.right = b,
                                          .as.binary.op = BINARY_OP_ADD});
      if (a == 0)
        return 0;
    } else if (operand.type == TOKEN_TYPE_RES) {
      parser_consume_token(ctx);
      node_id b = parser_parse_term(ctx);
      if (b == 0)
        return 0;
      a = ast_add_node(ctx->ast, (node_t){.kind = NODE_KIND_BINARY_OP,
                                          .as.binary.left = a,
                                          .as.binary.right = b,
                                          .as.binary.op = BINARY_OP_SUB});
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
      a = ast_add_node(ctx->ast, (node_t){.kind = NODE_KIND_BINARY_OP,
                                          .as.binary.left = a,
                                          .as.binary.right = b,
                                          .as.binary.op = BINARY_OP_MUL});
      if (a == 0)
        return 0;
    } else if (operand.type == TOKEN_TYPE_DIV) {
      parser_consume_token(ctx);
      node_id b = parser_parse_factor(ctx);
      if (b == 0)
        return 0;
      a = ast_add_node(ctx->ast, (node_t){.kind = NODE_KIND_BINARY_OP,
                                          .as.binary.left = a,
                                          .as.binary.right = b,
                                          .as.binary.op = BINARY_OP_DIV});
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
    return ast_add_node(
        ctx->ast, (node_t){.kind = NODE_KIND_NUMBER, .as.number = a.data});
  }

  if (tok.type == TOKEN_TYPE_RES) {
    parser_consume_token(ctx);
    node_id n = parser_parse_factor(ctx);
    if (!n) {
      parser_set_error(ctx, PARSER_ERROR_WRONG_SINTAXIS,
                       "Expected expression after unary '-'");
      return 0;
    }
    return ast_add_node(ctx->ast, (node_t){.kind = NODE_KIND_UNARY_OP,
                                           .as.unary.op = UNARY_OP_NEG,
                                           .as.unary.operand = n});
  }

  if (parser_peek_token(ctx).type == TOKEN_TYPE_OPEN_PARENTHESIS) {
    parser_consume_token(ctx);
    node_id a = parser_parse_expression(ctx);
    if (parser_peek_token(ctx).type != TOKEN_TYPE_CLOSE_PARENTHESIS) {
      parser_set_error(
          ctx, PARSER_ERROR_WRONG_SINTAXIS,
          "Expected ')' at position %zu, instead type: %d, data: %d",
          ctx->token_consume_index,
          token_stream_get(ctx->tokens, ctx->token_consume_index).type,
          token_stream_get(ctx->tokens, ctx->token_consume_index).data);
    }
    parser_consume_token(ctx);
    return a;
  }

  parser_set_error(
      ctx, PARSER_ERROR_WRONG_SINTAXIS,
      "Unexpected token at position %zu, type: %d, data: %d",
      ctx->token_consume_index,
      token_stream_get(ctx->tokens, ctx->token_consume_index).type,
      token_stream_get(ctx->tokens, ctx->token_consume_index).data);
  return 0;
}

token_t parser_consume_token(parser_t *ctx) {
  assert(ctx);
  assert(ctx->tokens);
  return token_stream_get(ctx->tokens, ctx->token_consume_index++);
}
token_t parser_peek_token(parser_t *ctx) {
  assert(ctx);
  assert(ctx->tokens);
  return token_stream_get(ctx->tokens, ctx->token_consume_index);
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
