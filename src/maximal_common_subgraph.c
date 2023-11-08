#include "maximal_common_subgraph.h"
#include "error_handling.h"
#include "platform_specific.h"

#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

Graph create_modular_product(Graph* first_graph, Graph* second_graph) 
{
    int modular_product_vertices = first_graph -> vertices * second_graph -> vertices;
    int modular_product_edges = 0;
    int first_graph_vertex_start, first_graph_vertex_end = 0;
    int second_graph_vertex_start, second_graph_vertex_end = 0;

    Graph modular_product;
    initialize_graph(&modular_product, modular_product_vertices, 0);

    for (int i = 0; i < modular_product_vertices; i++)
    {
        determine_original_vertices(i, first_graph->vertices, second_graph->vertices, &first_graph_vertex_start, &second_graph_vertex_start);
        for (int j = 0; j < modular_product_vertices; j++)
        {
            determine_original_vertices(j, first_graph->vertices, second_graph->vertices, &first_graph_vertex_end, &second_graph_vertex_end);
            if (first_graph_vertex_start == first_graph_vertex_end || second_graph_vertex_start == second_graph_vertex_end) continue;

            int first_graph_edges = determine_edges_number(first_graph -> adjacency_matrix, first_graph_vertex_start, first_graph_vertex_end);
            int second_graph_edges = determine_edges_number(second_graph -> adjacency_matrix, second_graph_vertex_start, second_graph_vertex_end);

            modular_product.adjacency_matrix[i + 1][j + 1] = determine_edges_number_in_modular_product(first_graph_edges, second_graph_edges);
            modular_product_edges += modular_product.adjacency_matrix[i + 1][j + 1];
        }
    }
    
    modular_product.edges = modular_product_edges;
    return modular_product;
}

void determine_original_vertices(int modular_product_vertex, int first_graph_vertices, int second_graph_vertices, int *first_graph_vertex, int *second_graph_vertex) 
{  
    *first_graph_vertex = modular_product_vertex / second_graph_vertices + 1;
    *second_graph_vertex = modular_product_vertex % second_graph_vertices + 1;
}

uint32_t determine_edges_number_in_modular_product(int first_graph_edges, int second_graph_edges)
{
    if (first_graph_edges != 0 && second_graph_edges != 0) return MIN(first_graph_edges, second_graph_edges);
    if (first_graph_edges == 0 && second_graph_edges == 0) return 1;
    else return 0;
}
