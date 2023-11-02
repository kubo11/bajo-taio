#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "error_handling.h"
#include "platform_specific.h"

#define USAGE "Usage:"ENDLINE"\tbajo-taio [path]"

int main(int argc, char **argv) {
  ASSERT(argc == 1 || argc == 2, "Wrong number of arguments. "USAGE);

  Graph graph;
  if (argc == 1) {
    load_graph_from_console(&graph);
  }
  else {
    load_graph_from_file(&graph, argv[1]);
  }

  GraphSize graphSize = get_graph_size(&graph);
  
  printf("Graph size: (%d, %d)"ENDLINE, graphSize.vertices_plus_unique_edges, graphSize.vertices_plus_edges);

  destroy_graph(&graph);

  return EXIT_SUCCESS;
}