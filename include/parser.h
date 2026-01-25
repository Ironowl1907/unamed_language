#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

typedef struct parser parser_t;

typedef size_t node_id;

typedef enum {
  PARSER_ERROR_NONE = 0,
  PARSER_ERROR_NULL_PARAMETER,
  PARSER_ERROR_INSUFFICIENT_MEMORY,
  PARSER_ERROR_WRONG_SINTAXIS,

} parser_error_e;

typedef enum {
  NODE_KIND_BINARY_OP,
  NODE_KIND_UNARY_OP,
  NODE_KIND_NUMBER,

  NODE_KIND_INVALID,
} node_kind_e;

typedef enum {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_NEG, // For unary
} operator_e;

typedef struct node {
  node_kind_e kind;

  union {
    struct {
      operator_e op;
      node_id left;
      node_id right;
    } binary;

    struct {
      operator_e op;
      node_id operand;
    } unary;

    double number;
  } as;
} node_t;

parser_t *parser_create();
void parser_delete(parser_t *ctx);

parser_error_e parser_parse(parser_t *ctx);

void parser_print_debug(parser_t *ctx, node_id ast);

#endif
