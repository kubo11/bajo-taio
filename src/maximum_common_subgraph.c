#include "maximum_common_subgraph.h"

Graph* create_modular_product(Graph *first_graph, Graph *second_graph) 
{
    int vertices = first_graph -> vertices * second_graph -> vertices;
    int edges = 0;
    int first_graph_vertex_start, first_graph_vertex_end = 0;
    int second_graph_vertex_start, second_graph_vertex_end = 0;

    Graph *modular_product = (Graph *)calloc(1, sizeof(Graph));
    initialize_graph(modular_product, vertices, 0);

    for (int i = 0; i < vertices; i++)
    {
        determine_original_vertices(i, second_graph->vertices, &first_graph_vertex_start, &second_graph_vertex_start);
        for (int j = 0; j < vertices; j++)
        {
            determine_original_vertices(j, second_graph->vertices, &first_graph_vertex_end, &second_graph_vertex_end);
            if (first_graph_vertex_start == first_graph_vertex_end || second_graph_vertex_start == second_graph_vertex_end) continue;

            int first_graph_edges = determine_edges(first_graph -> adjacency_matrix, first_graph_vertex_start, first_graph_vertex_end);
            int second_graph_edges = determine_edges(second_graph -> adjacency_matrix, second_graph_vertex_start, second_graph_vertex_end);
            int modular_product_edges = determine_edges_in_modular_product(first_graph_edges, second_graph_edges);

            if (modular_product_edges != 0) add_edge(modular_product, i + 1, j + 1, modular_product_edges);
            edges += modular_product -> adjacency_matrix[i + 1][j + 1];
        }
    }
    
    modular_product -> edges = edges;
    return modular_product;
}

void determine_original_vertices(int modular_product_vertex, int second_graph_vertices, int *first_graph_vertex, int *second_graph_vertex) 
{  
    *first_graph_vertex = modular_product_vertex / second_graph_vertices + 1;
    *second_graph_vertex = modular_product_vertex % second_graph_vertices + 1;
}

uint32_t determine_edges_in_modular_product(int first_graph_edges, int second_graph_edges)
{
    if (first_graph_edges != 0 && second_graph_edges != 0) return MIN(first_graph_edges, second_graph_edges);
    if (first_graph_edges == 0 && second_graph_edges == 0) return 1;
    else return 0;
}

Graph** find_maximum_common_subgraphs(Graph *first_graph, Graph *second_graph){
    ASSERT(first_graph != NULL, "first_graph is NULL");
    ASSERT(second_graph != NULL, "second_graph is NULL");

    Graph* modular_product = create_modular_product(first_graph, second_graph);
    Graph* modular_product_clique = get_max_clique(modular_product);

    Graph** maximum_common_subgraphs = create_maximum_common_subgraphs(first_graph, second_graph, modular_product_clique);

    destroy_graph(modular_product);
    destroy_graph(modular_product_clique);

    return maximum_common_subgraphs;
}

Graph** create_maximum_common_subgraphs(Graph* first_graph, Graph* second_graph, Graph* modular_product_clique){
    Graph** maximum_common_subgraphs = (Graph **)calloc(2, sizeof(Graph *));
    ASSERT(maximum_common_subgraphs != NULL, "Cannot create maximum_common_subgraphs");

    maximum_common_subgraphs[0] = (Graph *)calloc(1, sizeof(Graph));
    maximum_common_subgraphs[1] = (Graph *)calloc(1, sizeof(Graph));

    initialize_graph(maximum_common_subgraphs[0], first_graph->vertices, 0);
    initialize_graph(maximum_common_subgraphs[1], second_graph->vertices, 0);

    int first_graph_vertex_start, first_graph_vertex_end = 0;
    int second_graph_vertex_start, second_graph_vertex_end = 0;

    for (int u = 1; u <= modular_product_clique -> vertices; u++)
    {
        for (int v = 1; v <= modular_product_clique -> vertices; v++)
        {
            if (modular_product_clique -> adjacency_matrix[u][v] != 0) {
                determine_original_vertices(u - 1, second_graph->vertices, &first_graph_vertex_start, &second_graph_vertex_start);
                determine_original_vertices(v - 1, second_graph->vertices, &first_graph_vertex_end, &second_graph_vertex_end);
                
                int first_graph_edges_number = first_graph -> adjacency_matrix[first_graph_vertex_start][first_graph_vertex_end];
                int second_graph_edges_number = second_graph -> adjacency_matrix[second_graph_vertex_start][second_graph_vertex_end];

                int edges_number = MIN(first_graph_edges_number, second_graph_edges_number);
                if (edges_number == 0) continue;

                add_edge(maximum_common_subgraphs[0], first_graph_vertex_start, first_graph_vertex_end, edges_number);
                add_edge(maximum_common_subgraphs[1], second_graph_vertex_start, second_graph_vertex_end, edges_number);

                maximum_common_subgraphs[0] -> edges += edges_number;
                maximum_common_subgraphs[1] -> edges += edges_number;
            }
        }
    }
    
    return maximum_common_subgraphs;
}
