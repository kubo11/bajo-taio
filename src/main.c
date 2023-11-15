#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "error_handling.h"
#include "platform_specific.h"
#include "maximal_common_subgraph.h"

#define USAGE "Usage:"ENDLINE"\tbajo-taio [path]"

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

  show_graph(&first_graph, "First");
  if (graphs_number == 2 || argc == 3)
  {
    show_graph(&second_graph, "Second");
    Graph modular_product = create_modular_product(&first_graph, &second_graph);
    show_graph(&modular_product, "Modular product");

    Graph* clique = get_max_clique(&modular_product);

    if (clique != NULL) {
      printf("Clique for supplied graph:"ENDLINE);
      print_edges(clique);
      destroy_graph(clique);
    }
    else {
      printf("No clique found."ENDLINE);
    }

    destroy_graph(&modular_product);
    destroy_graph(&second_graph);
  }
  else {
    GraphSize graphSize = get_graph_size(&first_graph);
    
    printf("Graph size: (%d, %d)"ENDLINE, graphSize.vertices_plus_unique_edges, graphSize.vertices_plus_edges);

    Graph* clique = get_max_clique(&first_graph);

    if (clique != NULL) {
      printf("Clique for supplied graph:"ENDLINE);
      print_edges(clique);
      destroy_graph(clique);
    }
    else {
      printf("No clique found."ENDLINE);
    }
  }

  destroy_graph(&first_graph);
  return EXIT_SUCCESS;
}
