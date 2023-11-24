#include "maximum_common_subgraph.h"

Graph* create_modular_product(Graph *first_graph, Graph *second_graph) 
{
    int vertices = first_graph -> vertices * second_graph -> vertices;
    int first_graph_vertex_start, first_graph_vertex_end = 0;
    int second_graph_vertex_start, second_graph_vertex_end = 0;

    Graph *modular_product = (Graph *)calloc(1, sizeof(Graph));
    initialize_graph(modular_product, vertices);

    for (int i = 0; i < vertices; i++)
    {
        determine_original_vertices(i, second_graph->vertices, &first_graph_vertex_start, &second_graph_vertex_start);
        for (int j = 0; j < vertices; j++)
        {
            determine_original_vertices(j, second_graph->vertices, &first_graph_vertex_end, &second_graph_vertex_end);
            if (first_graph_vertex_start == first_graph_vertex_end || second_graph_vertex_start == second_graph_vertex_end) continue;

            int first_graph_edges = determine_edges(first_graph -> adjacency_matrix, first_graph_vertex_start, first_graph_vertex_end);
            int second_graph_edges = determine_edges(second_graph->adjacency_matrix, second_graph_vertex_start, second_graph_vertex_end);
            int modular_product_edges = determine_edges_in_modular_product(first_graph_edges, second_graph_edges);

            if (modular_product_edges != 0) add_edge(modular_product, i + 1, j + 1, modular_product_edges);
        }
    }
    
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

Graph** find_maximum_common_subgraphs(Graph *first_graph, Graph *second_graph, bool aprox){
    ASSERT(first_graph != NULL, "first_graph is NULL");
    ASSERT(second_graph != NULL, "second_graph is NULL");

    int max_clique_number = 0;
    Graph* modular_product = create_modular_product(first_graph, second_graph);
    Graph **modular_product_cliques = get_max_clique(modular_product, &max_clique_number, aprox, true);
    Graph** maximum_common_subgraphs = create_maximum_common_subgraphs(first_graph, second_graph, modular_product_cliques, max_clique_number);

    destroy_graph(modular_product);
    for (int i = 0; i < max_clique_number; i++)
    {
        destroy_graph(modular_product_cliques[i]);
    }
    

    return maximum_common_subgraphs;
}

Graph** create_maximum_common_subgraphs(Graph* first_graph, Graph* second_graph, Graph** modular_product_clique, int max_clique_number){
    Graph** maximum_common_subgraphs = (Graph **)calloc(2, sizeof(Graph *));
    ASSERT(maximum_common_subgraphs != NULL, "Cannot create maximum_common_subgraphs");

    maximum_common_subgraphs[0] = (Graph *)calloc(1, sizeof(Graph));
    maximum_common_subgraphs[1] = (Graph *)calloc(1, sizeof(Graph));

    initialize_graph(maximum_common_subgraphs[0], first_graph->vertices);
    initialize_graph(maximum_common_subgraphs[1], second_graph->vertices);

    Bitset *first_graph_bitset = create_bitset(first_graph->vertices + 1);
    Bitset *second_graph_bitset = create_bitset(second_graph->vertices + 1);
    uint32_t max_vertices = 0;
    int max_index = 0;

    int first_graph_vertex_start, first_graph_vertex_end = 0;
    int second_graph_vertex_start, second_graph_vertex_end = 0;

    for (int i = 0; i < max_clique_number; i++)
    {
        for (int u = 1; u <= modular_product_clique[i]->vertices; u++)
        {
            for (int v = 1; v <= modular_product_clique[i]->vertices; v++)
            {
                if (modular_product_clique[i]->adjacency_matrix[u][v] != 0)
                {
                    determine_original_vertices(u - 1, second_graph->vertices, &first_graph_vertex_start, &second_graph_vertex_start);
                    determine_original_vertices(v - 1, second_graph->vertices, &first_graph_vertex_end, &second_graph_vertex_end);

                    set_bit(first_graph_bitset, first_graph_vertex_start);
                    set_bit(first_graph_bitset, first_graph_vertex_end);
                    set_bit(second_graph_bitset, second_graph_vertex_end);
                    set_bit(second_graph_bitset, second_graph_vertex_end);
                }
            }
        }

        if (count_set_bits(first_graph_bitset) > max_vertices && count_set_bits(second_graph_bitset) > max_vertices)
        {
            max_index = i;
            max_vertices = count_set_bits(first_graph_bitset);
        }
    }

    for (int u = 1; u <= modular_product_clique[max_index]->vertices; u++)
    {
        for (int v = 1; v <= modular_product_clique[max_index]->vertices; v++)
        {
            if (modular_product_clique[max_index]->adjacency_matrix[u][v] != 0)
            {
                determine_original_vertices(u - 1, second_graph->vertices, &first_graph_vertex_start, &second_graph_vertex_start);
                determine_original_vertices(v - 1, second_graph->vertices, &first_graph_vertex_end, &second_graph_vertex_end);

                int first_graph_edges_number = first_graph->adjacency_matrix[first_graph_vertex_start][first_graph_vertex_end];
                int second_graph_edges_number = second_graph->adjacency_matrix[second_graph_vertex_start][second_graph_vertex_end];

                int edges_number = MIN(first_graph_edges_number, second_graph_edges_number);
                if (edges_number == 0)
                    continue;

                add_edge(maximum_common_subgraphs[0], first_graph_vertex_start, first_graph_vertex_end, edges_number);
                add_edge(maximum_common_subgraphs[1], second_graph_vertex_start, second_graph_vertex_end, edges_number);
            }
        }
        }

    return maximum_common_subgraphs;
}
