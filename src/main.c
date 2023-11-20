#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "error_handling.h"
#include "platform_specific.h"
#include "maximum_common_subgraph.h"

#define SIZE_CMD  "size"
#define DISTANCE_CMD "dist"
#define CLIQUE_CMD "cliq"
#define MAX_COMMON_SUBGRAPH_CMD "sub"
#define USAGE "Usage:"ENDLINE"\tbajo-taio {"SIZE_CMD"|"DISTANCE_CMD"|"CLIQUE_CMD"|"MAX_COMMON_SUBGRAPH_CMD"} [path] [path]"

int main(int argc, char **argv) {
  ASSERT(argc >= 2, "No subcommand selected. "USAGE);
  ASSERT(argc <= 4, "Too many arguments. "USAGE);

  int graphs_number;
  Graph first_graph, second_graph;

  if (!strncmp(SIZE_CMD, argv[1], strlen(SIZE_CMD))) {
    ASSERT(argc == 2 || argc == 3, "Size command accepts only one argument - path to graph file or no arguments. "USAGE);

    load_graphs(&first_graph, &second_graph, graphs_number = 1, argc > 2, argv);

    GraphSize graphSize = get_graph_size(&first_graph);
    printf("Graph size: (%d, %d)"ENDLINE, graphSize.vertices_plus_unique_edges, graphSize.vertices_plus_edges);
  }
  else if (!strncmp(DISTANCE_CMD, argv[1], strlen(DISTANCE_CMD))) {
    ASSERT(argc == 2 || argc == 4, "Compare command accepts two argument - paths to graph files or no arguments. "USAGE);

    load_graphs(&first_graph, &second_graph, graphs_number = 2, argc > 2, argv);

    float distance = graph_distance(&first_graph, &second_graph);

    printf("Distance betweend first and second graph: %f", distance);
  }
  else if (!strncmp(CLIQUE_CMD, argv[1], strlen(CLIQUE_CMD))) {
    ASSERT(argc == 2 || argc == 3, "Clique command accepts only one argument - path to graph file or no arguments. "USAGE);

    load_graphs(&first_graph, &second_graph, graphs_number = 1, argc > 2, argv);

    Graph* clique = get_max_clique(&first_graph, false);

    if (clique != NULL) {
      printf("Clique for supplied graph:"ENDLINE);
      print_edges(clique);
      destroy_graph(clique);
    }
    else {
      printf("No clique found."ENDLINE);
    }

    Graph* approx_clique = get_max_clique(&first_graph, true);

    if (approx_clique != NULL) {
      printf("Approximated clique for supplied graph:"ENDLINE);
      print_edges(approx_clique);
      destroy_graph(approx_clique);
    }
    else {
      printf("No approximated clique found."ENDLINE);
    }
  }
  else if (!strncmp(MAX_COMMON_SUBGRAPH_CMD, argv[1], strlen(MAX_COMMON_SUBGRAPH_CMD))) {
    ASSERT(argc == 2 || argc == 4, "Max common subgraph command accepts two argument - paths to graph files or no arguments. "USAGE);

    load_graphs(&first_graph, &second_graph, graphs_number = 2, argc > 2, argv);

    show_graph(&first_graph, "First");
    show_graph(&second_graph, "Second");
    Graph** maximum_common_subgraphs = find_maximum_common_subgraphs(&first_graph, &second_graph);

    show_graph(maximum_common_subgraphs[0], "First graph subgraph");
    show_graph(maximum_common_subgraphs[1], "Second graph subgraph");

    free(maximum_common_subgraphs);
  }
  else {
    fprintf(stderr, "Wrong command. "USAGE);
    return EXIT_FAILURE;
  }

  destroy_graphs(&first_graph, &second_graph, graphs_number);

  return EXIT_SUCCESS;
}
