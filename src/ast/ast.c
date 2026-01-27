#include "ast.h"
#include "ast_internal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define AST_ARENA_INIT_SIZE 16

ast_t *ast_create(void) {
  ast_t *ctx;

  ctx = malloc(sizeof *ctx);
  if (!ctx) {
    free(ctx);
    return NULL;
  }

  ctx->nodes = malloc(sizeof(*ctx->nodes) * AST_ARENA_INIT_SIZE);
  if (!ctx->nodes) {
    free(ctx->nodes);
    free(ctx);
    return NULL;
  }

  ctx->capacity = 16;
  ctx->size = 1; // 0 Reserved for errors

  return ctx;
}
void ast_destroy(ast_t *ctx) {
  if (ctx) {
    free(ctx->nodes);
    free(ctx);
  }
}

node_id ast_add_node(ast_t *ctx, node_t node) {
  assert(ctx != NULL);
  assert(ctx->size);
  if (ctx->size == ctx->capacity) {
    ast_error_e error = ast_arena_resize(ctx, ctx->capacity * 2);
    if (error != AST_ERROR_NONE)
      return 0;
  }
  ctx->nodes[ctx->size] = node;

  return ctx->size++;
}

ast_error_e ast_arena_resize(ast_t *ctx, size_t size) {
  if (!ctx)
    return AST_ERROR_NULL_PARAMETER;

  node_t *new_arr = realloc(ctx->nodes, size * sizeof *ctx->nodes);
  if (!new_arr)
    return AST_ERROR_INSUFFICIENT_MEMORY;

  ctx->nodes = new_arr;
  ctx->capacity = size;

  return AST_ERROR_NONE;
}

ast_error_e ast_evaluate(ast_t *ctx, node_id nodeId, double *result) {
  node_t node = ctx->nodes[nodeId];

  double l = 0.0, r = 0.0;
  ast_error_e err;

  switch (node.kind) {

  case NODE_KIND_BINARY_OP:
    err = ast_evaluate(ctx, node.as.binary.left, &l);
    if (err != AST_ERROR_NONE)
      return err;

    err = ast_evaluate(ctx, node.as.binary.right, &r);
    if (err != AST_ERROR_NONE)
      return err;

    switch (node.as.binary.op) {
    case BINARY_OP_ADD:
      *result = l + r;
      break;

    case BINARY_OP_SUB:
      *result = l - r;
      break;

    case BINARY_OP_MUL:
      *result = l * r;
      break;

    case BINARY_OP_DIV:
      if (r == 0.0)
        return AST_ERROR_EVAL_ZERO_DIVISION;
      *result = l / r;
      break;
    }
    break;

  case NODE_KIND_UNARY_OP:
    err = ast_evaluate(ctx, node.as.unary.operand, &l);
    if (err != AST_ERROR_NONE)
      return err;

    switch (node.as.unary.op) {
    case UNARY_OP_NEG:
      *result = -l;
      break;
    }
    break;

  case NODE_KIND_NUMBER:
    *result = node.as.number;
    break;

  case NODE_KIND_INVALID:
    return AST_ERROR_EVAL_AST_INVALID_NODE;
  }

  return AST_ERROR_NONE;
}

static const char *op_to_string(binary_operator_e op) {
  switch (op) {
  case BINARY_OP_ADD:
    return "+";
  case BINARY_OP_SUB:
    return "-";
  case BINARY_OP_MUL:
    return "*";
  case BINARY_OP_DIV:
    return "/";
  default:
    return "UNKNOWN_OP";
  }
}

static void print_indent(size_t depth) {
  for (size_t i = 0; i < depth; ++i)
    printf("  ");
}

static void parser_print_debug_node(ast_t *ctx, node_id id, size_t depth) {
  if (!ctx || id == 0 || id >= ctx->size) {
    print_indent(depth);
    printf("<invalid node %zu>\n", id);
    return;
  }

  node_t *node = &ctx->nodes[id];

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

void ast_print_debug(ast_t *ctx, node_id ast) {
  if (!ctx) {
    printf("<null parser>\n");
    return;
  }

  printf("AST Debug Print (root = %zu):\n", ast);
  parser_print_debug_node(ctx, ast, 0);
}
