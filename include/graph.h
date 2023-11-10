#ifndef BAJO_TAIO_GRAPH_H
#define BAJO_TAIO_GRAPH_H

#include <stdint.h>

typedef struct Graph {
  uint32_t vertices;
  uint32_t edges;
  uint32_t unique_edges;
  uint32_t **adjacency_matrix;
} Graph;

typedef struct GraphSize {
  uint32_t vertices_plus_unique_edges;
  uint32_t vertices_plus_edges;
} GraphSize;

void initialize_graph(Graph *graph, int vertices, int edges);

void show_graph(const Graph *graph, const char *name);

void set_graphs_number_from_console(int *graphs_number);

void load_graphs_from_file(Graph *first_graph, Graph *second_graph, const char *first_path, const char *second_path);

void load_graphs_from_console(Graph *first_graph, Graph *second_graph, int graphs_number);

void destroy_graph(Graph *graph);

void add_edge(Graph *graph, uint32_t beg, uint32_t end, uint32_t edges_count);

void remove_edge(Graph *graph, uint32_t beg, uint32_t end);

GraphSize get_graph_size(const Graph *graph);

uint8_t graph_size_cmp(const GraphSize graphSize1, const GraphSize graphSize2);

uint8_t graph_cmp(const Graph *graph1, const Graph *graph2);

int determine_edges(uint32_t **adjacency_matrix, int first_vertex, int second_vertex);

#endif // BAJO_TAIO_GRAPH_H