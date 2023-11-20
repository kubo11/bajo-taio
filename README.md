# bajo-taio

## Building

To build project create build directory in project root directory and run **make** command.

## Running

Running program without any arguments will print usage message. \
The first argument is one of the following commands:
- size - print graph's size
- dist - print distance between two graphs
- cliq - find max clique for graph
- sub - find maximum common subgraph for two graphs
Than user can optionally pass paths to files containing graph data (1 path for size and cliq, 2 paths for dist and sub). If user hasn't passed any path they will be asked to pass graph data via console.
First user will be asked to pass number of graphs. Program accepts value of one or two.\
Usage:
```
bajo-taio {size|dist|cliq|sub} [path_to_first_graph] [path_to_second_graph]
```

## Data

Program accepts data in the following format:
- 1st line - number of vertices (n)
- 2nd line - number of edges (m)
- next m lines - pairs of vertex numbers separated by space, representing an edge

Data is stored in an adjacency matrix. Vertices are numbered from 1 to n. The size of matrix is n+1 by n+1 (cells for 0 are empty).