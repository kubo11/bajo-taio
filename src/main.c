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
  Graph **graphs;

  if (!strncmp(SIZE_CMD, argv[1], strlen(SIZE_CMD))) {
    ASSERT(argc == 2 || argc == 3, "Size command accepts only one argument - path to graph file or no arguments. "USAGE);

    if (argc > 2) {
      graphs_number = 1;
      load_graphs_from_file(&graphs, &graphs_number, &(argv[2]));
    }
    else {
      load_graphs_from_console(&graphs, &graphs_number);
    }

    GraphSize graphSize = get_graph_size(graphs[0]);
    printf("Graph size: (%d, %d)"ENDLINE, graphSize.vertices_plus_unique_edges, graphSize.vertices_plus_edges);
  }
  else if (!strncmp(DISTANCE_CMD, argv[1], strlen(DISTANCE_CMD))) {
    ASSERT(argc >= 2 && argc <= 4, "Compare command accepts one or two paths to graph files or no arguments. "USAGE);

    if (argc > 2) {
      if (argc == 4) graphs_number = 2;
      load_graphs_from_file(&graphs, &graphs_number, &(argv[2]));
    }
    else {
      load_graphs_from_console(&graphs, &graphs_number);
    }
    ASSERT(graphs_number == 2, "Wrong number of graphs.");

    float distance = graph_distance(graphs[0], graphs[1]);

    printf("Distance between first and second graph: %f", distance);
  }
  else if (!strncmp(CLIQUE_CMD, argv[1], strlen(CLIQUE_CMD))) {
    ASSERT(argc == 2 || argc == 3, "Clique command accepts only one argument - path to graph file or no arguments. "USAGE);

    if (argc > 2) {
      graphs_number = 1;
      load_graphs_from_file(&graphs, &graphs_number, &(argv[2]));
    }
    else {
      load_graphs_from_console(&graphs, &graphs_number);
    }

    TIME start, end;
    TIME_SETUP();

    MEASURE(start);
    int max_clique_number = 0;
    Graph** clique = get_max_clique(graphs[0], &max_clique_number, false, false);
    MEASURE(end);

    float regular_time;
    DIFF(start, end, regular_time);

    if (clique != NULL) {
      int iter = 0;
      while (clique[iter] != NULL) {
        printf("#%d clique for supplied graph:"ENDLINE, iter+1);
        print_edges(clique[iter]);
        destroy_graph(clique[iter]);
        iter++;
      }
      free(clique);
    }
    else {
      printf("No clique found."ENDLINE);
    }

    MEASURE(start);
    Graph** approx_clique = get_max_clique(graphs[0], &max_clique_number, true, false);
    MEASURE(end);

    float approx_time;
    DIFF(start, end, approx_time);

    if (approx_clique != NULL) {
      int iter = 0;
      while (approx_clique[iter] != NULL) {
        printf("#%d approximated clique for supplied graph:"ENDLINE, iter+1);
        print_edges(approx_clique[iter]);
        destroy_graph(approx_clique[iter]);
        iter++;
      }
      free(approx_clique);
    }
    else {
      printf("No approximated clique found."ENDLINE);
    }

    printf("Time elapsed for regular algorithm: %.21fs"ENDLINE, regular_time);
    printf("Time elapsed for approximated algorithm: %.21fs"ENDLINE, approx_time);
  }
  else if (!strncmp(MAX_COMMON_SUBGRAPH_CMD, argv[1], strlen(MAX_COMMON_SUBGRAPH_CMD))) {
    ASSERT(argc >= 2 || argc <= 4, "Max common subgraph command accepts one or two paths to graph files or no arguments. "USAGE);

    if (argc > 2) {
      if (argc == 4) graphs_number = 2;
      load_graphs_from_file(&graphs, &graphs_number, &(argv[2]));
    }
    else {
      load_graphs_from_console(&graphs, &graphs_number);
    }
    ASSERT(graphs_number == 2, "Wrong number of graphs.");

    show_graph(graphs[0], "First");
    show_graph(graphs[1], "Second");

    TIME start, end;
    TIME_SETUP();

    MEASURE(start);
    Graph** maximum_common_subgraphs = find_maximum_common_subgraphs(graphs[0], graphs[1], false);
    MEASURE(end);

    float regular_time;
    DIFF(start, end, regular_time);

    show_graph(maximum_common_subgraphs[0], "First graph subgraph");
    show_graph(maximum_common_subgraphs[1], "Second graph subgraph");

    MEASURE(start);
    Graph** approx_maximum_common_subgraphs = find_maximum_common_subgraphs(graphs[0], graphs[1], true);
    MEASURE(end);

    float approx_time;
    DIFF(start, end, approx_time);

    show_graph(approx_maximum_common_subgraphs[0], "Approximated first graph subgraph");
    show_graph(approx_maximum_common_subgraphs[1], "Approximated second graph subgraph");

    printf("Time elapsed for regular algorithm: %.21fs"ENDLINE, regular_time);
    printf("Time elapsed for approximated algorithm: %.21fs"ENDLINE, approx_time);

    free(maximum_common_subgraphs);
    free(approx_maximum_common_subgraphs);
  }
  else {
    fprintf(stderr, "Wrong command. "USAGE);
    return EXIT_FAILURE;
  }

  for (int i = 0; i < graphs_number; ++i) {
    destroy_graph(graphs[i]);
    graphs[i] = NULL;
  }
  free(graphs);

  return EXIT_SUCCESS;
}
