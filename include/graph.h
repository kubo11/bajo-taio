#ifndef BAJO_TAIO_GRAPH_H
#define BAJO_TAIO_GRAPH_H

#include <stdint.h>

typedef struct Graph {
  uint32_t vertices;
  uint32_t edges;
  uint32_t unique_edges;
  uint32_t *out_degrees;
  uint32_t *in_degrees;
  uint32_t **adjacency_matrix;
} Graph;

typedef struct GraphSize {
  uint32_t vertices_plus_unique_edges;
  uint32_t vertices_plus_edges;
} GraphSize;

void initialize_graph(Graph* graph, int vertices, int edges);

void load_graph_from_file(Graph* graph, const char* path);

void load_graph_from_console(Graph* graph);

void destroy_graph(Graph* graph);

void add_edge(Graph* graph, uint32_t beg, uint32_t end);

void remove_edge(Graph* graph, uint32_t beg, uint32_t end);

GraphSize get_graph_size(const Graph* graph);

uint8_t graph_size_cmp(const GraphSize graphSize1, const GraphSize graphSize2);

uint8_t graph_cmp(const Graph* graph1, const Graph* graph2);

uint32_t get_max_graph_degree(const Graph* graph);

int32_t* get_graph_distribution(const Graph* graph);

float graph_distance(const Graph* graph1, const Graph* graph2);

#endif // BAJO_TAIO_GRAPH_H