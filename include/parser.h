#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

typedef struct parser parser_t;

typedef size_t node_id;

typedef enum {
  PARSER_ERROR_NONE = 0,

} parser_error_e;

typedef enum {
  NODE_TYPE_SUM,
  NODE_TYPE_REST,
  NODE_TYPE_MUL,
  NODE_TYPE_DIV,
  NODE_TYPE_NEG,

  NODE_TYPE_FACTOR,

} parser_node_type_e;

typedef struct node {
  node_id right, left;
  parser_node_type_e type;
} node_t;

parser_t *parser_create();
void *parser_delete();

parser_error_e parser_parse(parser_t *ctx);

size_t parser_add_node(parser_t *ctx, node_t node);


#endif
