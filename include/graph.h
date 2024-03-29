#ifndef BAJO_TAIO_GRAPH_H
#define BAJO_TAIO_GRAPH_H

#include <stdint.h>

#include "bitset.h"

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

void initialize_graph(Graph *graph, int vertices);

void show_graph(const Graph *graph, const char *name);

bool is_single_graph(FILE *in);

void load_graphs(Graph **graphs, int graphs_number, FILE *in, FILE *out);

void load_graphs_from_file(Graph ***graphs, int *graphs_number, char **paths);

void load_graphs_from_console(Graph ***graphs, int *graphs_number);

void destroy_graph(Graph *graph);

void add_edge(Graph *graph, uint32_t beg, uint32_t end, uint32_t edges_count);

void remove_edge(Graph *graph, uint32_t beg, uint32_t end);

GraphSize get_graph_size(const Graph *graph);

uint8_t graph_size_cmp(const GraphSize graphSize1, const GraphSize graphSize2);

uint8_t graph_cmp(const Graph *graph1, const Graph *graph2);

int determine_edges(uint32_t **adjacency_matrix, int first_vertex, int second_vertex);

uint32_t get_max_graph_degree(const Graph* graph);

uint32_t* get_graph_distribution(const Graph* graph);

float graph_distance(const Graph* graph1, const Graph* graph2);

void print_edges(Graph* graph);

uint32_t** get_list_of_plain_cliques(Graph *graph);

void store_clique(Bitset *clique, Bitset **cliques, uint32_t *num_of_cliques);

void BronKerbosch(Bitset *R, Bitset *P, Bitset *X, uint32_t vertices, Bitset **adjacency_matrix, Bitset **cliques, uint32_t *num_of_cliques);

Bitset** create_bitset_adjacency_matrix(Graph *graph);

void destroy_bitset_adjacency_matrix(Bitset **bitset_adjacency_matrix, int vertices);

Graph* extract_clique(Graph *graph, Bitset *clique);

uint32_t clique_get_max_p(Graph *clique);

uint8_t p_clique_cmp(Graph *clique1, uint32_t p1, Graph *clique2, uint32_t p2);

Graph** get_max_clique(Graph *graph, int *max_clique_number, bool aprox, bool modular_clique);

#endif // BAJO_TAIO_GRAPH_H