#ifndef BAJO_TAIO_MAXIMAL_COMMON_SUBGRAPH_H
#define BAJO_TAIO_MAXIMAL_COMMON_SUBGRAPH_H

#include "graph.h"

Graph create_modular_product(Graph *first_graph, Graph *second_graph);
void determine_original_vertices(int modular_product_vertex, int second_graph_vertices, int *first_graph_vertex, int *second_graph_vertex);
uint32_t determine_edges_in_modular_product(int first_graph_edges, int second_graph_edges);

#endif // BAJO_TAIO_MAXIMAL_COMMON_SUBGRAPH_H