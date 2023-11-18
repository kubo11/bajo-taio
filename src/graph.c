#include "graph.h"
#include "error_handling.h"
#include "platform_specific.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define CLIQUES_BUFFER_SIZE 1000000

void initialize_graph(Graph *graph, int vertices, int edges) {
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

void show_graph(const Graph *graph, const char *name) {
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

void set_graphs_number_from_console(int *graphs_number) {
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

void load_graphs_from_console(Graph *first_graph, Graph *second_graph, int graphs_number) {
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

void destroy_graph(Graph *graph) {
  ASSERT(graph != NULL, "graph is NULL");
  for (int i = 0; i < graph->vertices + 1; ++i)
    free(graph->adjacency_matrix[i]);
  free(graph->adjacency_matrix);
  free(graph->out_degrees);
  free(graph->in_degrees);
  graph = NULL;
}

void add_edge(Graph *graph, uint32_t beg, uint32_t end, uint32_t edges_count)
{
  ASSERT(graph != NULL, "graph is NULL");
  ASSERT(1 <= beg && beg <= graph->vertices, "beg is out of range");
  ASSERT(1 <= end && end <= graph->vertices, "end is out of range");
  ASSERT(beg != end, "self-loops are not allowed");
  if (!graph->adjacency_matrix[beg][end] && !graph->adjacency_matrix[end][beg]) graph->unique_edges++;
  graph->adjacency_matrix[beg][end] += edges_count;
  graph->out_degrees[beg]++;
  graph->in_degrees[end]++;
}

void remove_edge(Graph *graph, uint32_t beg, uint32_t end) {
  ASSERT(graph != NULL, "graph is NULL");
  ASSERT(1 <= beg && beg <= graph->vertices, "beg is out of range");
  ASSERT(1 <= end && end <= graph->vertices, "end is out of range");
  ASSERT(graph->adjacency_matrix[beg][end] != 0, "attempt to remove nonexistant edge");
  graph->adjacency_matrix[beg][end]--;
  if (!graph->adjacency_matrix[beg][end] && !graph->adjacency_matrix[end][beg]) graph->unique_edges--;
  graph->out_degrees[beg]--;
  graph->in_degrees[end]--;
}

GraphSize get_graph_size(const Graph *graph) {
  ASSERT(graph != NULL, "graph is NULL");
  GraphSize graphSize = {graph->vertices + graph->unique_edges, graph->vertices + graph->edges};
  return graphSize;
}

uint8_t graph_size_cmp(const GraphSize graphSize1, const GraphSize graphSize2) {
  if (graphSize1.vertices_plus_unique_edges > graphSize2.vertices_plus_unique_edges) return -1;
  if (graphSize1.vertices_plus_unique_edges < graphSize2.vertices_plus_unique_edges) return 1;
  if (graphSize1.vertices_plus_edges > graphSize2.vertices_plus_edges) return -1;
  if (graphSize1.vertices_plus_edges < graphSize2.vertices_plus_edges) return 1;
  return 0;
}

uint8_t graph_cmp(const Graph *graph1, const Graph *graph2) {
  ASSERT(graph1 != NULL, "graph1 is NULL");
  ASSERT(graph2 != NULL, "graph2 is NULL");
  GraphSize graphSize1 = get_graph_size(graph1);
  GraphSize graphSize2 = get_graph_size(graph2);

  return graph_size_cmp(graphSize1, graphSize2);
}

int determine_edges(uint32_t **adjacency_matrix, int first_vertex, int second_vertex) {
  return adjacency_matrix[first_vertex][second_vertex];
}

uint32_t get_max_graph_degree(const Graph* graph)
{
  uint32_t max_degree = 0;
  for (uint32_t v = 1; v < graph->vertices + 1; ++v)
    if (graph->out_degrees[v] > max_degree)
      max_degree = graph->out_degrees[v];
  for (uint32_t v = 1; v < graph->vertices + 1; ++v)
    if (graph->in_degrees[v] > max_degree)
      max_degree = graph->in_degrees[v];

  return max_degree;
}

uint32_t* get_graph_distribution(const Graph* graph)
{
  uint32_t size = (get_max_graph_degree(graph) + 1) * 2;
  uint32_t* distribution = NULL;
  CHECK(NULL == (distribution = calloc(size, sizeof(uint32_t))));
  for (uint32_t v = 1; v < graph->vertices + 1; ++v)
  {
    distribution[2 * graph->out_degrees[v]] += 1;
    distribution[2 * graph->in_degrees[v] + 1]++;
  }
  return distribution;
}

float graph_distance(const Graph* graph1, const Graph* graph2)
{
  uint32_t size1 = (get_max_graph_degree(graph1) + 1) * 2;
  uint32_t size2 = (get_max_graph_degree(graph2) + 1) * 2;

  uint32_t* distribution1 = get_graph_distribution(graph1);
  uint32_t* distribution2 = get_graph_distribution(graph2);

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

void print_edges(Graph* graph) {
  ASSERT(graph != NULL, "graph is NULL");
  for (int v = 1; v <= graph->vertices; ++v) {
    for (int u = 1; u <= graph->vertices; ++u) {
      if (v == u || !graph->adjacency_matrix[v][u]) continue;
      printf("%d -> %d: %d"ENDLINE, v, u, graph->adjacency_matrix[v][u]);
    }
  }
}

void store_clique(Bitset *clique, Bitset **cliques, uint32_t *num_of_cliques) {
  if (*num_of_cliques == 0 || count_set_bits(cliques[0]) == count_set_bits(clique)) {
    cliques[(*num_of_cliques)++] = copy_bitset(clique);
    return;
  }
  if (count_set_bits(cliques[0]) < count_set_bits(clique)) {
    while(*num_of_cliques > 0) {
      destroy_bitset(cliques[*num_of_cliques-1]);
      cliques[--(*num_of_cliques)] = NULL;
    }
    cliques[(*num_of_cliques)++] = copy_bitset(clique);
  }
}

Graph* find_complement_undirected_graph(Graph* graph) {
  Graph* complement_graph = (Graph*)calloc(1, sizeof(Graph));
  initialize_graph(complement_graph, graph -> vertices, 0);

  for (int v = 1; v <= graph -> vertices; v++)
  {
    for (int u = v + 1; u <= graph -> vertices; u++)
    {
      if (!(graph -> adjacency_matrix[v][u] && graph -> adjacency_matrix[u][v]))
      {
        complement_graph -> adjacency_matrix[v][u] = 1;
        complement_graph -> adjacency_matrix[u][v] = 1;
      }
    }  
  }

  return complement_graph;
}

Bitset** clique_aprox(Graph* graph, uint32_t* num_of_cliques) {
  int** exclude = (int**)calloc(graph -> vertices + 1, sizeof(int*));
  for (int i = 1; i <= graph -> vertices; i++)
  {
    exclude[i] = (int*)calloc(graph -> vertices + 1, sizeof(int));
  }

  int** cost = (int**)calloc(graph -> vertices + 1, sizeof(int *));
  for (int i = 1; i <= graph->vertices; i++)
  {
    cost[i] = (int*)calloc(graph -> vertices + 1, sizeof(int));
  }

  int** previous = (int**)calloc(graph -> vertices + 1, sizeof(int *));
  for (int i = 1; i <= graph->vertices; i++)
  {
    previous[i] = (int*)calloc(graph -> vertices + 1, sizeof(int));
  }

  for (int u = 1; u <= graph -> vertices; u++)
  {
    exclude[u][u] = 1;
    cost[u][1] = 1;

    for (int v = 1; v < graph -> vertices; v++)
    {
      if (graph -> adjacency_matrix[u][v]){
        exclude[u][v] = 1;
        cost[u][1]++;
      }
    }

    for (int round = 2; round <= graph -> vertices; round++)
    {
      cost[u][round] = INT_MAX;
    }
  }

  int cost_changed, last_round = 0;

  for (int round = 1; round <= graph -> vertices; round++)
  {
      for (int u = 1; u <= graph -> vertices; u++)
      {
        for (int v = 1; v <= graph -> vertices; v++)
        { 
          if (exclude[u][v] == 0)
          {
            if (cost[u][round] < INT_MAX)
            {
              int* v_exclude = exclude[u];
              v_exclude[v] = 1;
              int v_cost = cost[u][round] + 1;

              for (int j = 1; j <= graph -> vertices; j++)
              {
                if (graph -> adjacency_matrix[v][j])
                {
                  if (!v_exclude[j])
                  {
                    v_exclude[j] = 1;
                    v_cost++;
                  }
                }
              }

              if (cost[v][round + 1] > v_cost)
              {
                previous[v][round + 1] = u;
                cost[v][round + 1] = v_cost;
                cost_changed = 1;
              }
              
            }
          }
        }
      }

      if (cost_changed == 0) {
        last_round = round;
        break;
      }
      cost_changed = 0;
  }
  
  Bitset** clique_candidates = (Bitset**)calloc(graph -> vertices + 1, sizeof(Bitset*));
  for (int i = 1; i < graph -> vertices; i++)
  {
      clique_candidates[i] = create_bitset(graph -> vertices + 1);
  }
  
  for (int u = 1; u <= graph -> vertices; u++)
  {
    int t = u;
    (*num_of_cliques)++;
    while (t != 0)
    {
      printf("%d \n", t);
      set_bit(clique_candidates[u], t); 
      t = previous[t][last_round];
    }
  }
  
  return clique_candidates;
}

void BronKerbosch(Bitset *R, Bitset *P, Bitset *X, uint32_t vertices, Bitset **adjacency_matrix, Bitset **cliques, uint32_t *num_of_cliques) {
  ASSERT(R != NULL, "R is NULL.");
  ASSERT(P != NULL, "P is NULL.");
  ASSERT(X != NULL, "X is NULL.");
  ASSERT(adjacency_matrix != NULL, "adjacency_matrix is NULL.");
  ASSERT(cliques != NULL, "cliques is NULL.");
  ASSERT(num_of_cliques != NULL, "num_of_cliques is NULL.");
  // report R as maximal clique
  if (check_if_all_unset(P) && check_if_all_unset(X) && count_set_bits(R) >= 1) {
    store_clique(R, cliques, num_of_cliques);
  }

  for (int v = 1; v <= vertices; ++v) {
    // run for vertices set in P
    if (!get_bit(P, v)) continue;
  
    // R ⋃ {v}
    Bitset *newR = copy_bitset(R);
    set_bit(newR, v);
    // P ⋂ N(v)
    Bitset *newP = bitset_intersection(P, adjacency_matrix[v]);
    // X ⋂ N(v)
    Bitset *newX = bitset_intersection(X, adjacency_matrix[v]);
  
    BronKerbosch(newR, newP, newX, vertices, adjacency_matrix, cliques, num_of_cliques);
  
    // destroy bitsets used in past calculations
    destroy_bitset(newP);
    destroy_bitset(newR);
    destroy_bitset(newX);
  
    // P := P \ {v}
    unset_bit(P, v);
    // X := X ⋃ {v}
    set_bit(X, v);
  }
}

Bitset** construct_bitset_adjacency_matrix(Graph *graph) {
  ASSERT(graph != NULL, "graph is NULL");
  Bitset **bitset_adjacency_matrix = (Bitset**)calloc(graph->vertices + 1, sizeof(Bitset*));
  for (int v = 1; v <= graph->vertices; ++v) {
    bitset_adjacency_matrix[v] = create_bitset(graph->vertices + 1);
  }
  for (int v = 1; v <= graph->vertices; ++v) {
    for (int u = v + 1; u <= graph->vertices; ++u) {
      if (graph->adjacency_matrix[v][u] && graph->adjacency_matrix[u][v]) {
        set_bit(bitset_adjacency_matrix[v], u);
        set_bit(bitset_adjacency_matrix[u], v);
      }
    }
  }
  return bitset_adjacency_matrix;
}

void destroy_bitset_adjacency_matrix(Bitset** bitset_adjacency_matrix, int vertices) {
  for (int i = 1; i <= vertices; ++i) {
    destroy_bitset(bitset_adjacency_matrix[i]);
  }
  free(bitset_adjacency_matrix);
}

Graph* extract_clique(Graph *graph, Bitset *clique) {
  ASSERT(graph != NULL, "graph is NULL");
  ASSERT(graph != NULL, "clique is NULL");
  Graph *extracted_clique = (Graph*)calloc(1, sizeof(Graph));
  initialize_graph(extracted_clique, graph->vertices, 0);
  for (int v = 1; v <= graph->vertices; ++v) {
    if (!get_bit(clique, v)) continue;
    for (int u = v + 1; u <= graph->vertices; ++u) {
      if (!get_bit(clique, u)) continue;
      if (graph->adjacency_matrix[v][u]) {
        extracted_clique->adjacency_matrix[v][u] = graph->adjacency_matrix[v][u];
        extracted_clique->edges += graph->adjacency_matrix[v][u];
      }
      if (graph->adjacency_matrix[u][v]) {
        extracted_clique->adjacency_matrix[u][v] = graph->adjacency_matrix[u][v];
        extracted_clique->edges += graph->adjacency_matrix[u][v];
      }
      if (extracted_clique->adjacency_matrix[v][u] || extracted_clique->adjacency_matrix[u][v]) {
        extracted_clique->unique_edges++;
      }
    }
  }
  return extracted_clique;
}

uint32_t clique_get_max_p(Graph *clique) {
  ASSERT(clique != NULL, "clique is NULL");
  uint32_t p = clique->vertices * clique->vertices;
  for (int v = 1; v <= clique->vertices; ++v) {
    for (int u = v + 1; u <= clique->vertices; ++u) {
      if (!clique->adjacency_matrix[v][u] || !clique->adjacency_matrix[u][v]) continue;
      p = fmin(p, fmin(clique->adjacency_matrix[v][u], clique->adjacency_matrix[u][v]));
    }
  }
  return p;
}

uint8_t p_clique_cmp(Graph *clique1, uint32_t p1, Graph *clique2, uint32_t p2) {
  ASSERT(clique1 != NULL, "clique1 is NULL");
  ASSERT(clique2 != NULL, "clique2 is NULL");
  if (p1 > p2) return -1;
  if (p1 < p2) return 1;
  return graph_cmp(clique1, clique2);
}

Graph* get_max_clique(Graph *graph, bool aprox) {
  ASSERT(graph != NULL, "graph is NULL");
  Bitset **cliques = NULL;
  uint32_t num_of_cliques = 0;

  if (aprox)
  {
    Graph *complement_graph = find_complement_undirected_graph(graph);
    cliques = clique_aprox(complement_graph, &num_of_cliques);
    destroy_graph(complement_graph);
  }

  else 
  {
    cliques = (Bitset **)calloc(CLIQUES_BUFFER_SIZE, sizeof(Bitset *));
    Bitset *R = create_bitset(graph->vertices + 1);
    Bitset *P = create_bitset(graph->vertices + 1);
    set_all_bits(P);
    Bitset *X = create_bitset(graph->vertices + 1);
    Bitset **bitset_adjacency_matrix = construct_bitset_adjacency_matrix(graph);
    ASSERT(cliques != NULL, "Could not allocate memory for cliques.");

    BronKerbosch(R, P, X, graph->vertices, bitset_adjacency_matrix, cliques, &num_of_cliques);
    destroy_bitset(R);
    destroy_bitset(P);
    destroy_bitset(X);
    destroy_bitset_adjacency_matrix(bitset_adjacency_matrix, graph->vertices);

    if (num_of_cliques == 0)
    {
      free(cliques);
      return NULL;
    }
  }
  
  Graph **extracted_cliques = (Graph**)calloc(num_of_cliques, sizeof(Graph*));
  ASSERT(extracted_cliques != NULL, "Could not allocate memory for extracted_cliques.");
  uint32_t *clique_p = (uint32_t*)calloc(num_of_cliques, sizeof(uint32_t));
  ASSERT(clique_p != NULL, "Could not allocate memory for clique_p.");

  for (int i = 0; i < num_of_cliques; ++i) {
    extracted_cliques[i] = extract_clique(graph, cliques[i]);
    destroy_bitset(cliques[i]);
    clique_p[i] = clique_get_max_p(extracted_cliques[i]);
  }
  free(cliques);

  int max_clique_id = 0;

  for (int i = 1; i < num_of_cliques; ++i) {
    if (p_clique_cmp(extracted_cliques[max_clique_id],
      clique_p[max_clique_id], extracted_cliques[i], clique_p[i]) == 1) {
      max_clique_id = i;
    }
  }

  for (int i = 0; i < num_of_cliques; ++i) {
    if (i == max_clique_id) continue;
    destroy_graph(extracted_cliques[i]);
  }

  Graph *max_clique = (Graph*)calloc(1, sizeof(Graph));
  memcpy(max_clique, extracted_cliques[max_clique_id], sizeof(Graph));
  free(extracted_cliques[max_clique_id]);

  for (int v = 1; v <= max_clique->vertices; ++v) {
    for (int u = 1; u <= max_clique->vertices; ++u) {
      if (max_clique->adjacency_matrix[v][u]) {
        max_clique->adjacency_matrix[v][u] = clique_p[max_clique_id];
      }
    }
  }

  free(extracted_cliques);
  free(clique_p);

  return max_clique;
}