#include "graph.h"
#include "error_handling.h"
#include "platform_specific.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialize_graph(Graph* graph, int vertices, int edges) {
  graph->vertices = vertices;
  graph->edges = edges;
  graph->unique_edges = 0;

  graph->out_degrees = calloc(vertices + 1, sizeof(uint32_t));
  graph->in_degrees = calloc(vertices + 1, sizeof(uint32_t));
  uint32_t *ptr = NULL;
  CHECK(NULL == (ptr = calloc(vertices + 1, sizeof(uint32_t*))));
  graph->adjacency_matrix = (uint32_t **)ptr;
  for (int i = 0; i < vertices + 1; ++i) {
    CHECK(NULL == (ptr = calloc(vertices + 1, sizeof(uint32_t))));
    graph->adjacency_matrix[i] = ptr;
  }
}

void load_graph_from_file(Graph* graph, const char* path) {
  ASSERT(graph != NULL, "graph is NULL");
  FILE* file;
  ASSERT(NULL != (file = fopen(path, "r")), "file doesn\'t exist");

  int vertices, edges, beg, end;

  fscanf(file, "%d", &vertices);
  CHECK(ferror(file));
  ASSERT(vertices > 0, "number of vertices should be greater than 0");
  fscanf(file, "%d", &edges);
  CHECK(ferror(file));
  ASSERT(edges >= 0, "number of edges should be greater or equal than 0");

  initialize_graph(graph, vertices, edges);

  for (int i = 0; i < edges; ++i) {
    fscanf(file, "%d %d", &beg, &end);
    CHECK(ferror(file));
    add_edge(graph, beg, end);
  }
}

void load_graph_from_console(Graph* graph) {
  ASSERT(graph != NULL, "graph is NULL");

  int vertices, edges, beg, end;

  printf("Number of vertices: ");
  scanf("%d", &vertices);
  CHECK(ferror(stdin));
  ASSERT(vertices > 0, "number of vertices should be greater than 0");
  printf("Number of edges: ");
  scanf("%d", &edges);
  CHECK(ferror(stdin));
  ASSERT(edges >= 0, "number of edges should be greater or equal than 0");
  printf("List of edges:"ENDLINE);

  initialize_graph(graph, vertices, edges);

  for (int i = 0; i < edges; ++i) {
    scanf("%d %d", &beg, &end);
    CHECK(ferror(stdin));
    add_edge(graph, beg, end);
  }
}

void destroy_graph(Graph* graph) {
  ASSERT(graph != NULL, "graph is NULL");
  for (int i = 0; i < graph->vertices + 1; ++i)
    free(graph->adjacency_matrix[i]);
  free(graph->adjacency_matrix);
  free(graph->out_degrees);
  free(graph->in_degrees);
  graph = NULL;
}

void add_edge(Graph* graph, uint32_t beg, uint32_t end) {
  ASSERT(graph != NULL, "graph is NULL");
  ASSERT(1 <= beg && beg <= graph->vertices, "beg is out of range");
  ASSERT(1 <= end && end <= graph->vertices, "end is out of range");
  ASSERT(beg != end, "self-loops are not allowed");
  if (!graph->adjacency_matrix[beg][end] && !graph->adjacency_matrix[end][beg]) graph->unique_edges++;
  graph->adjacency_matrix[beg][end]++;
  graph->out_degrees[beg]++;
  graph->in_degrees[end]++;
}

void remove_edge(Graph* graph, uint32_t beg, uint32_t end) {
  ASSERT(graph != NULL, "graph is NULL");
  ASSERT(1 <= beg && beg <= graph->vertices, "beg is out of range");
  ASSERT(1 <= end && end <= graph->vertices, "end is out of range");
  ASSERT(graph->adjacency_matrix[beg][end] != 0, "attempt to remove nonexistant edge");
  graph->adjacency_matrix[beg][end]--;
  if (!graph->adjacency_matrix[beg][end] && !graph->adjacency_matrix[end][beg]) graph->unique_edges--;
  graph->out_degrees[beg]--;
  graph->in_degrees[end]--;
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

uint32_t get_max_graph_degree(const Graph* graph)
{
  uint32_t out_max = 0;
  for (uint32_t v = 1; v < graph->vertices + 1; ++v)
    if (graph->out_degrees[v] > out_max)
      out_max = graph->out_degrees[v];

  uint32_t in_max = 0;
  for (uint32_t v = 1; v < graph->vertices + 1; ++v)
    if (graph->in_degrees[v] > in_max)
      in_max = graph->in_degrees[v];

  return in_max > out_max ? in_max : out_max;
}

void get_graph_distribution(const Graph* graph, int32_t* distribution)
{
  for (uint32_t v = 1; v < graph->vertices + 1; ++v)
    distribution[2 * graph->out_degrees[v]] += 1;

  for (uint32_t v = 1; v < graph->vertices + 1; ++v)
    distribution[2 * graph->in_degrees[v] + 1]++;
}

float graph_distance(const Graph* graph1, const Graph* graph2)
{
  uint32_t size1 = (get_max_graph_degree(graph1) + 1) * 2;
  uint32_t size2 = (get_max_graph_degree(graph2) + 1) * 2;

  int32_t* distribution1 = NULL, *distribution2 = NULL;
  CHECK(NULL == (distribution1 = calloc(size1, sizeof(int32_t))));
  CHECK(NULL == (distribution2 = calloc(size2, sizeof(int32_t))));

  get_graph_distribution(graph1, distribution1);
  get_graph_distribution(graph2, distribution2);

  uint32_t min_size, max_size, *longer_distribution;
  if (size1 < size2)
  {
    min_size = size1;
    max_size = size2;
    longer_distribution = distribution2;
  }
  else
  {
    min_size = size2;
    max_size = size1;
    longer_distribution = distribution1;
  }

  float squares_sum = 0;
  uint32_t i;
  for (i = 0; i < min_size; ++i)
    squares_sum += (distribution1[i] - distribution2[i]) * (distribution1[i] - distribution2[i]);

  for (; i < max_size; ++i)
    squares_sum += longer_distribution[i] * longer_distribution[i];

  free(distribution1);
  free(distribution2);
  return sqrt(squares_sum);
}