#include "graph.h"
#include "error_handling.h"
#include "platform_specific.h"

#include <stdio.h>
#include <stdlib.h>

void initialize_graph(Graph* graph, int vertices, int edges) {
  graph->vertices = vertices;
  graph->edges = edges;
  graph->unique_edges = 0;

  uint32_t *ptr = NULL;
  CHECK(NULL == (ptr = calloc(vertices + 1, sizeof(uint32_t*))));
  graph->adjacency_matrix = (uint32_t **)ptr;
  for (int i = 0; i < vertices + 1; ++i) {
    CHECK(NULL == (ptr = calloc(vertices + 1, sizeof(uint32_t))));
    graph->adjacency_matrix[i] = ptr;
  }
}

void show_graph(const Graph *graph, const char* name) {
  GraphSize graph_size = get_graph_size(graph);
  printf("%s graph size: (%d, %d)" ENDLINE, name, graph_size.vertices_plus_unique_edges, graph_size.vertices_plus_edges);
  printf("Adjacency matrix: \n");

  for (int i = 1; i < graph -> vertices + 1; i++)
  {
    for (int j = 1; j < graph -> vertices + 1; j++)
    {
      printf("%d \t", graph -> adjacency_matrix[i][j]);
    }
    printf("\n");
  }
}

void set_graphs_number_from_console(int* graphs_number) {
  printf("Number of graphs: ");
  scanf("%d", graphs_number);
  CHECK(ferror(stdin));
  ASSERT(*graphs_number == 1 || *graphs_number == 2, "Number of graphs should be equal to 1 or 2");
}

void load_graphs_from_file(Graph *first_graph, Graph *second_graph, const char *first_path, const char *second_path)
{
  int graphs_number = second_path == NULL ? 1 : 2;

  for (int i = 0; i < graphs_number; ++i){
    ASSERT((i == 0 ? first_graph : second_graph) != NULL, "graph is NULL");
    FILE *file;
    ASSERT(NULL != (file = fopen(i == 0 ? first_path : second_path, "r")), "file doesn\'t exist");

    int vertices, edges, beg, end;

    fscanf(file, "%d", &vertices);
    CHECK(ferror(file));
    ASSERT(vertices > 0, "number of vertices should be greater than 0");
    fscanf(file, "%d", &edges);
    CHECK(ferror(file));
    ASSERT(edges >= 0, "number of edges should be greater or equal than 0");

    initialize_graph(i == 0 ? first_graph : second_graph, vertices, edges);

    for (int j = 0; j < edges; ++j)
    {
      fscanf(file, "%d %d", &beg, &end);
      CHECK(ferror(file));
      add_edge(i == 0 ? first_graph : second_graph, beg, end, 1);
    }
  }
}

void load_graphs_from_console(Graph* first_graph, Graph* second_graph, int graphs_number) {
  ASSERT(first_graph != NULL, "First graph is NULL");
  ASSERT(second_graph != NULL, "Second graph is NULL");

  int vertices, edges, beg, end;

  for (int i = 0; i < graphs_number; ++i)
  {
    printf("Number of vertices in %d. graph: ", i+1);
    scanf("%d", &vertices);
    CHECK(ferror(stdin));
    ASSERT(vertices > 0, "number of vertices should be greater than 0");
    printf("Number of edges in %d. graph: ", i+1);
    scanf("%d", &edges);
    CHECK(ferror(stdin));
    ASSERT(edges >= 0, "number of edges should be greater or equal than 0");
    printf("List of edges:" ENDLINE);

    initialize_graph(i == 0 ? first_graph : second_graph, vertices, edges);

    for (int j = 0; j < edges; ++j)
    {
      scanf("%d %d", &beg, &end);
      CHECK(ferror(stdin));
      add_edge(i == 0 ? first_graph : second_graph, beg, end, 1);
    }
  }
}

void destroy_graph(Graph* graph) {
  ASSERT(graph != NULL, "graph is NULL");
  for (int i = 0; i < graph->vertices + 1; ++i)
    free(graph->adjacency_matrix[i]);
  free(graph->adjacency_matrix);
  graph = NULL;
}

void add_edge(Graph *graph, uint32_t beg, uint32_t end, uint32_t edge_number)
{
  ASSERT(graph != NULL, "graph is NULL");
  ASSERT(1 <= beg && beg <= graph->vertices, "beg is out of range");
  ASSERT(1 <= end && end <= graph->vertices, "end is out of range");
  ASSERT(beg != end, "self-loops are not allowed");
  if (!graph->adjacency_matrix[beg][end] && !graph->adjacency_matrix[end][beg]) graph->unique_edges++;
  graph->adjacency_matrix[beg][end] += edge_number;
}

void remove_edge(Graph* graph, uint32_t beg, uint32_t end) {
  ASSERT(graph != NULL, "graph is NULL");
  ASSERT(1 <= beg && beg <= graph->vertices, "beg is out of range");
  ASSERT(1 <= end && end <= graph->vertices, "end is out of range");
  ASSERT(graph->adjacency_matrix[beg][end] != 0, "attempt to remove nonexistant edge");
  graph->adjacency_matrix[beg][end]--;
  if (!graph->adjacency_matrix[beg][end] && !graph->adjacency_matrix[end][beg]) graph->unique_edges--;
}

GraphSize get_graph_size(const Graph* graph) {
  ASSERT(graph != NULL, "graph is NULL");
  GraphSize graphSize = {graph->vertices + graph->unique_edges, graph->vertices + graph->edges};
  return graphSize;
}

uint8_t graph_size_cmp(const GraphSize graphSize1, const GraphSize graphSize2) {
  if (graphSize1.vertices_plus_unique_edges < graphSize2.vertices_plus_unique_edges) return -1;
  if (graphSize1.vertices_plus_unique_edges < graphSize2.vertices_plus_unique_edges) return 1;
  if (graphSize1.vertices_plus_edges < graphSize2.vertices_plus_edges) return -1;
  if (graphSize1.vertices_plus_edges < graphSize2.vertices_plus_edges) return 1;
  return 0;
}

uint8_t graph_cmp(const Graph* graph1, const Graph* graph2) {
  ASSERT(graph1 != NULL, "graph1 is NULL");
  ASSERT(graph2 != NULL, "graph2 is NULL");
  GraphSize graphSize1 = get_graph_size(graph1);
  GraphSize graphSize2 = get_graph_size(graph2);

  return graph_size_cmp(graphSize1, graphSize2);
}

int determine_edges(uint32_t **adjacency_matrix, int first_vertex, int second_vertex) {
  return adjacency_matrix[first_vertex][second_vertex];
}