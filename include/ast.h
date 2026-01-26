#ifndef AST_H
#define AST_H

#include <stddef.h>
typedef size_t node_id;

typedef enum {
  AST_ERROR_NONE = 0,
  AST_ERROR_NULL_PARAMETER,
  AST_ERROR_INSUFFICIENT_MEMORY,
  AST_ERROR_WRONG_SINTAXIS,

  AST_ERROR_EVAL_ZERO_DIVISION,
  AST_ERROR_EVAL_AST_INVALID_NODE,

} ast_error_e;

typedef enum {
  NODE_KIND_BINARY_OP,
  NODE_KIND_UNARY_OP,
  NODE_KIND_NUMBER,

  NODE_KIND_INVALID,
} node_kind_e;

typedef enum {
  BINARY_OP_ADD,
  BINARY_OP_SUB,
  BINARY_OP_MUL,
  BINARY_OP_DIV,
} binary_operator_e;

typedef enum {
  UNARY_OP_NEG,
} unary_operator_e;

typedef struct node {
  node_kind_e kind;

  union {
    struct {
      binary_operator_e op;
      node_id left;
      node_id right;
    } binary;

    struct {
      unary_operator_e op;
      node_id operand;
    } unary;

    double number;
  } as;
} node_t;

typedef struct ast {
  node_t *nodes;
  size_t size;
  size_t capacity;

  node_id root;
} ast_t;

ast_t *ast_create(void);
void ast_destroy(ast_t *ctx);

ast_error_e ast_evaluate(ast_t *ctx, node_id root, double *result);

ast_error_e ast_arena_resize(ast_t *ctx, size_t size);
size_t ast_add_node(ast_t *ctx, node_t node);

void ast_print_debug(ast_t *ctx, node_id ast);

#endif
