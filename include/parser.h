#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token_stream.h"
#include <stddef.h>

typedef struct parser parser_t;

typedef enum {
  PARSER_ERROR_NONE = 0,
  PARSER_ERROR_NULL_PARAMETER,
  PARSER_ERROR_INSUFFICIENT_MEMORY,
  PARSER_ERROR_WRONG_SINTAXIS,
  PARSER_ERROR_AST_CREATION,

} parser_error_e;

parser_t *parser_create(token_stream_t *token_stream);
void parser_delete(parser_t *ctx);

parser_error_e parser_parse(parser_t *ctx, ast_t **out_ast);

const char *parser_get_error_string(parser_t *ctx);
parser_error_e parser_get_error_code(parser_t *ctx);

#endif
