#include "ast_internal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

ast_t *ast_create(void) {
  ast_t *ctx;

  ctx = malloc(sizeof *ctx);
  if (!ctx) {
    free(ctx);
    return NULL;
  }

  // TODO: Remove magical number 16
  ctx->nodes = malloc(sizeof(*ctx->nodes) * 16);
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

size_t ast_add_node(ast_t *ctx, node_t node) {
  assert(ctx != NULL);
  assert(ctx->size);
  if (ctx->size == ctx->capacity) {
    // TODO: Error checking
    ast_error_e error = ast_arena_resize(ctx, ctx->capacity * 2);
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

void ast_evaluate(ast_t *ctx, node_id root, double *result) {}

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
