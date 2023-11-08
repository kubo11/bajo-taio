#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "error_handling.h"
#include "platform_specific.h"
#include "maximal_common_subgraph.h"

#define USAGE "Usage:"ENDLINE"\tbajo-taio [path]"

void show_graph(Graph graph, int number);

int main(int argc, char **argv) {
  ASSERT(argc == 1 || argc == 2 || argc == 3, "Wrong number of arguments. "USAGE);

  int graphs_number;
  Graph first_graph, second_graph;
  if (argc == 1) {
    set_graphs_number_from_console(&graphs_number);
    load_graphs_from_console(&first_graph, &second_graph, graphs_number);
  }
  else {
    load_graphs_from_file(&first_graph, &second_graph, argv[1], argv[2]);
  }

  // Graph modular_product = create_modular_product(&first_graph, &second_graph);
  // printf("MODULAR v %d \n", modular_product.vertices);
  // printf("MODULAR e %d \n", modular_product.edges);

  // for (int i = 1; i < modular_product.vertices + 1; i++)
  // {
  //   for (int j = 1; j < modular_product.vertices + 1; j++)
  //   {
  //     printf("%d \t", modular_product.adjacency_matrix[i][j]);
  //   }
  //   printf("\n");
  // }

  show_graph(first_graph, 1);
  if (graphs_number == 2 || argc == 3){
    show_graph(second_graph, 2);
  }
  
  return EXIT_SUCCESS;
}

void show_graph(Graph graph, int number) {
  GraphSize graph_size = get_graph_size(&graph);
  printf("%d. graph size: (%d, %d)" ENDLINE, number, graph_size.vertices_plus_unique_edges, graph_size.vertices_plus_edges);
  destroy_graph(&graph);
}