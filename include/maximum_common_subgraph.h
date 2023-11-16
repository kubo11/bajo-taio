#ifndef BAJO_TAIO_MAXIMUM_COMMON_SUBGRAPH_H
#define BAJO_TAIO_MAXIMUM_COMMON_SUBGRAPH_H

#include "graph.h"
#include "error_handling.h"
#include "platform_specific.h"

#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

Graph* create_modular_product(Graph *first_graph, Graph *second_graph);
Graph** find_maximum_common_subgraphs(Graph *first_graph, Graph *second_graph);
Graph** create_maximum_common_subgraphs(Graph* first_graph, Graph* second_graph, Graph* modular_product_clique);
void determine_original_vertices(int modular_product_vertex, int second_graph_vertices, int *first_graph_vertex, int *second_graph_vertex);
uint32_t determine_edges_in_modular_product(int first_graph_edges, int second_graph_edges);

#endif // BAJO_TAIO_MAXIMAL_COMMON_SUBGRAPH_H