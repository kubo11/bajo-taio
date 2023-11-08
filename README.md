# bajo-taio

## Building

To build project create build directory in project root directory and run **make** command.

## Running

Running program without any arguments will set it into console mode and user will be asked to supply graphs data via keyboard. 
First user will be asked to type a number of graphs. Program accepts a value of one or two.\
Running program with one or two arguments will set it into file mode. Program will attempt to open input files and read graphs data. \
Usage:
```
./bajo-taio [path_to_first_graph] [path_to_second_graph]
```

## Data

Program accepts data in the following format:
- 1st line - number of vertices (n)
- 2nd line - number of edges (m)
- next m lines - pairs of vertex numbers separated by space, representing an edge

Data is stored in an adjacency matrix. Vertices are numbered from 1 to n. The size of matrix is n+1 by n+1 (cells for 0 are empty).