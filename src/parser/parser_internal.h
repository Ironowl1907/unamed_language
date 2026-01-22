#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include "parser.h"
#include "token_stream.h"
#include <stddef.h>

typedef struct ast_arena {
  node_t *nodes;
  size_t size;
  size_t capacity;
} ast_arena_t;

struct parser {
  ast_arena_t ast;
  token_stream_t *tokens;

  size_t token_consume_index;
};

// clang-format off
node_id parser_parse_expresion(parser_t *ctx);
node_id parser_parse_term(parser_t *ctx);
node_id parser_parse_factor(parser_t *ctx);

token_t parser_consume_token(parser_t * ctx);
token_t parser_peek_token(parser_t * ctx);

parser_error_e parser_ast_arena_resize(parser_t * ctx, size_t size);

#endif
