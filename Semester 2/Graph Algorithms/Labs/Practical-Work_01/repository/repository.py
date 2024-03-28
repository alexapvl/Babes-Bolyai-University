import random
from domain.domain import Graph

class RepoError(ValueError):
    pass

class Repository():
    def __init__(self, graph: Graph):
        self.graph = graph
        
    def add_vertex(self, i):
        if i in self.graph.vertices:
            raise RepoError("\nVertex already exists in the graph\n")
        self.graph.vertices.append(i)
        self.graph.din[i] = []
        self.graph.dout[i] = []

    def remove_vertex(self, i):
        if i not in self.graph.vertices:
            raise RepoError("\nVertex does not exist in the graph\n")
        # delete keys from the dictionaries
        del self.graph.din[i]
        del self.graph.dout[i]
        for edge in list(self.graph.edges.keys()):
            if edge[0] == i or edge[1] == i:
                del self.graph.edges[edge]
                self.graph.numberOfEdges -= 1
        self.graph.vertices.remove(i)
        
    def add_edge(self, i, j, cost):
        if (i, j) in self.graph.edges.keys():
            raise RepoError(f"\nEdge {i} -> {j} already exists in the graph\n")
        if type(cost) != int:
            raise RepoError("\nCost must be an integer\n")
        self.graph.edges[(i, j)] = cost
        self.graph.dout[i].append(j)
        self.graph.din[j].append(i)
        if (len(self.graph.edges) - 1) == self.graph.numberOfEdges:
            self.graph.numberOfEdges += 1

    def remove_edge(self, i, j):
        if (i, j) not in self.graph.edges.keys():
            raise RepoError(f"\nEdge {i} -> {j} does not exist in the graph\n")
        del self.graph.edges[(i, j)]
        self.graph.dout[i].remove(j)
        self.graph.din[j].remove(i)
        self.graph.numberOfEdges -= 1
        
    def is_vertex(self, i) -> bool:
        if i in self.graph.vertices:
            return True
        return False
        
    def is_edge(self, i, j) -> bool:
        if (i, j) in self.graph.edges.keys():
            return True
        return False

    def get_isolated_vertices(self) -> list:
        isolated = []
        for i in self.graph.vertices:
            if len(self.graph.din[i]) == 0 and len(self.graph.dout[i]) == 0:
                isolated.append(i)
        return isolated

    def copy_graph(self) -> Graph:
        new_graph = Graph(len(self.graph.vertices), self.graph.numberOfEdges)
        new_graph.vertices = self.graph.vertices.copy()
        new_graph.edges = self.graph.edges.copy()
        new_graph.din = self.graph.din.copy()
        new_graph.dout = self.graph.dout.copy()
        return new_graph

    def generate_random_graph(self, no_vertices: int, no_edges: int) -> Graph:
        graph = Graph(no_vertices, no_edges)
        while no_edges > 0:
            i = random.choice(range(no_vertices))
            j = random.choice(range(no_vertices))
            if (i, j) not in graph.edges.keys():
                cost = random.choice(range(1, 100))
                graph.din[i].append(j)
                graph.dout[j].append(i)
                graph.edges[(i, j)] = cost
                no_edges -= 1
        return graph
    
    def get_vertices(self) -> list:
        return self.graph.vertices
    
    def update_edge_cost(self, i: int, j: int, cost: int):
        self.graph.edges[(i, j)] = cost

    def in_degree_of_vertex(self, i: int) -> int:
        return len(self.graph.din[i])

    def out_degree_of_vertex(self, i: int) -> int:
        return len(self.graph.dout[i])

    def number_of_vertices(self) -> int:
        return len(self.graph.vertices)

    def number_of_edges(self) -> int:
        return self.graph.numberOfEdges

    def get_vertices(self) -> list:
        return self.graph.vertices

    def get_inbounds_of_vertex(self, i: int) -> list:
        return self.graph.din[i]

    def get_outbounds_of_vertex(self, i: int) -> list:
        return self.graph.dout[i]

    @property
    def graph(self) -> Graph:
        return self.__graph
    
    @graph.setter
    def graph(self, value: Graph):
        self.__graph = value
        