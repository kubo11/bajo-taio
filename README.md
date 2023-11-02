# bajo-taio

## Building

To build project create build directory in project root directory and run **make** command.

## Running

Running program without any arguments will set it into console mode and user will be asked to supply graph data via keyboard. \
Running program with one argument will set it into file mode. Program will attempt to open input file and read graph data. \
Usage:
```
./bajo-taio [path]
```

## Data

Program accepts data in the following format:
- 1st line - number of vertices (n)
- 2nd line - number of edges (m)
- next m lines - pairs of vertex numbers separated by space, representing an edge

Data is stored in an adjacency matrix. Vertices are numbered from 1 to n. The size of matrix is n+1 by n+1 (cells for 0 are empty).