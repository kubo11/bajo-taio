"""
Arguments:
    1 - number of vertices
    2 - number of edges
    3 - number of unique edges (must be less than or equal to number of edges)
    4 - (optional) number of graphs

Examples:
        
    generate one graph with 10 vertices and 10 unique edges (simple graph)
    >>> python generator.py 10 10 10        

    generate three graphs with 100 vertices, 250 unique edges and 500 edges in total
    >>> python generator.py 100 500 250 3
    
"""
import numpy as np
import sys


def generate_edges(M, n_edges, unique):
    n_vertices = M.shape[0]

    if unique:
        w = np.ones((n_vertices, n_vertices)) - np.eye(n_vertices)
    else:
        w = M
    w = w.flatten()
    p = w / np.sum(w)
    idx = np.random.choice(np.arange(n_vertices * n_vertices), size=n_edges, replace=not unique, p=p)
    idx = np.unravel_index(idx, shape=(n_vertices, n_vertices))
    E = np.zeros(shape=(n_vertices, n_vertices))
    if unique:
        E[idx[0], idx[1]] = 1
    else:
        for beg, end in zip(*idx):
            E[beg, end] += 1
    return E


def generate_example(n_vertices, n_edges, n_unique_edges = None, save_path = None):
    assert(n_unique_edges is None or n_unique_edges <= n_edges)
    M = np.zeros(shape=(n_vertices, n_vertices))
    if n_unique_edges is not None:
        M += _generate_edges(M, n_unique_edges, True)
        n_edges -= n_unique_edges
    M += _generate_edges(M, n_edges, False)
    M = M.astype(np.int32)

    if save_path is not None:
        with open(save_path, 'w') as f:
            f.write('1\n')
            f.write(f'{n_vertices}\n')
            for i in range(n_vertices):
                for j in range(n_vertices):
                    f.write(f'{M[i][j]} ')
                f.write('\n')
    
    return M


def main():
    args = sys.argv
    assert(len(args) - 1 == 3 or len(args) - 1 == 4)

    if len(args) - 1 == 4:
        n_graphs = int(args[-1])
        args = args[:-1]
    else:
        n_graphs = 1
    for i in range(n_graphs):
        n, m, unique_m = args[1:]
        generate_example(int(n), int(m), int(unique_m), f'./example_{n}_{m}_{unique_m}_{i}.txt')


if __name__ == '__main__':
    main()
    
