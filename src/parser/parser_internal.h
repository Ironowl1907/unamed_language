#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include "ast.h"
#include "parser.h"
#include "token_stream.h"
#include <stddef.h>

#define PARSER_ERROR_MSG_SIZE 256

struct parser {
  token_stream_t *tokens;
  size_t token_consume_index;

	ast_t * ast;

  parser_error_e last_error;
  char error_msg[PARSER_ERROR_MSG_SIZE];
};

// clang-format off
size_t parser_add_node(parser_t *ctx, node_t node);

node_id parser_parse_expression(parser_t *ctx);
node_id parser_parse_term(parser_t *ctx);
node_id parser_parse_factor(parser_t *ctx);

token_t parser_consume_token(parser_t * ctx);
token_t parser_peek_token(parser_t * ctx);

parser_error_e parser_ast_arena_resize(parser_t * ctx, size_t size);


void parser_set_error(parser_t *ctx, parser_error_e code,
                             const char *fmt, ...);

#endif
