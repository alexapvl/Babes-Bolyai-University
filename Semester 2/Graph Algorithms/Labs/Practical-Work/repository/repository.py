import random
from collections import deque
from domain.domain import Graph, WolfGoatCabbageGraph

class RepoError(ValueError):
    pass

class Repository():
    def __init__(self, graph: Graph):
        self.graph = graph
        self.wgcGrapf = WolfGoatCabbageGraph()
        
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
        for inbound in self.graph.din[i]:
            self.graph.dout[inbound].remove(i)
        for outbound in self.graph.dout[i]:
            self.graph.din[outbound].remove(i)
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
        self.graph.numberOfEdges = len(self.graph.edges.keys())

    def remove_edge(self, i, j):
        if not self.is_edge(i, j):
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
        if not (self.is_vertex(i) and self.is_vertex(j)):
            return False
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
    
    def clear_graph(self):
        self.graph.vertices = []
        self.graph.edges = {}
        self.graph.din = {}
        self.graph.dout = {}
        self.graph.numberOfEdges = 0
    
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

    def shortest_path_between_two_vertices_forward_breath_first_search(self, start: int, end: int) -> list:
        '''
        This function calculates the shortest path between two vertices in a graph using a forward breadth-first search approach. 
        It verifies the existence of the provided vertices within the graph and returns the starting vertex if both vertices are the same.
        To keep track of visited vertices, it utilizes a set, and employs a queue to maintain the current vertex and its associated path. 
        Each vertex encountered during the search is marked as visited.
        The algorithm then explores the neighbors of the current vertex, checking if any neighbor matches the end vertex; 
        in such case, it returns the path. If a neighbor has not been visited before, it's added to the queue and marked as visited.
        If the end vertex is not reached after exploring all possible paths, the function returns None.
        '''
        if not ((self.is_vertex(start) and self.is_vertex(end))):
            raise RepoError("\nVertices do not exist in the graph\n")
        if start == end:
            return [start]
        visited = set()
        queue = deque([(start, [start])])
        while queue:
            current, path = queue.popleft()
            visited.add(current) # since we are using a set, we will not check for duplicates
            for neighbor in self.get_outbounds_of_vertex(current):
                if neighbor == end:
                    return path + [neighbor]
                if neighbor not in visited:
                    queue.append((neighbor, path + [neighbor]))
                    visited.add(neighbor)
        return None

    def shortest_path_wgc(self):
        '''
        Bonus problem: Wolf, Goat, Cabbage -> BFS algorithm
        Uses operations on bits to represent the state of the wolf, goat, and cabbage.
        0 means the object is on the left side of the river, 1 means the object is on the right side of the river.
        '''
        start = self.wgcGrapf.initial_state()
        end = self.wgcGrapf.final_state()
        q = [start]
        pred = {start: None}
        while len(q) > 0:
            x = q.pop(0)
            if x == end:
                break
            for y in self.get_outbounds_of_vertex(x):
                if y not in pred:
                    pred[y] = x
                    q.append(y)
        path = []
        while x is not None:
            path.append(x)
            x = pred[x]
        path.reverse()
        return path

    @property
    def graph(self) -> Graph:
        return self.__graph
    
    @graph.setter
    def graph(self, value: Graph):
        self.__graph = value
    
    def lowest_cost_walk_floyd_warshall(self, start: int, end: int) -> tuple:
        '''
        This function calculates the lowest cost walk between two vertices in a graph using the Floyd-Warshall algorithm, provided that
        the given graph has no negative cost cycles.
        It verifies the existence of the provided vertices within the graph and returns the starting vertex if both vertices are the same.
        The algorithm initializes a matrix of costs, where the cost of an edge is stored in the corresponding cell.
        It then iterates through the matrix, updating the cost of a path if a shorter path is found.
        The function returns a tuple consisting of a list with the intermediate matrices(as tuples), the path and the cost of the walk.
        '''
        if not ((self.is_vertex(start) and self.is_vertex(end))):
            raise RepoError("\nVertices do not exist in the graph\n")
        
        no_vertices = self.number_of_vertices()
        cost_matrix = [[float('inf')] * no_vertices for _ in range(no_vertices)] # initialize the matrix of costs
        for i in range(no_vertices):
            cost_matrix[i][i] = 0
        next_vertex = [["None"] * no_vertices for _ in range(no_vertices)] # initialize the matrix for the lowest cost path between two vertices
        intermediate_matrices = []
        
        for u in self.graph.vertices:
            for v in self.get_outbounds_of_vertex(u):
                cost_matrix[u][v] = self.graph.edges[(u, v)]
                next_vertex[u][v] = v
                
        cost_copy_matrix = [row.copy() for row in cost_matrix]
        next_vertex_copy_matrix = [row.copy() for row in next_vertex]
        intermediate_matrices.append((cost_copy_matrix, next_vertex_copy_matrix))
    
        for k in self.graph.vertices:
            for i in self.graph.vertices:
                for j in self.graph.vertices:
                    if k != i and k != j and i != j:
                        if cost_matrix[i][j] > cost_matrix[i][k] + cost_matrix[k][j]:
                            cost_matrix[i][j] = cost_matrix[i][k] + cost_matrix[k][j]
                            next_vertex[i][j] = next_vertex[i][k]
            cost_copy_matrix = [row.copy() for row in cost_matrix]
            next_vertex_copy_matrix = [row.copy() for row in next_vertex]
            intermediate_matrices.append((cost_copy_matrix, next_vertex_copy_matrix))
        
        if cost_matrix[start][end] == float('inf'):
            return intermediate_matrices, [], float('inf')
        path = [start]
        cost = cost_matrix[start][end]
        while start != end:
            start = next_vertex[start][end]
            path.append(start)
        
        return intermediate_matrices, path, cost
        