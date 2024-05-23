import random
import itertools
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

    def generate_complete_symetric_graph(self, no_vertices: int) -> Graph:
        graph = Graph(no_vertices, 0)
        for i in range(no_vertices):
            for j in range(i + 1, no_vertices):
                cost = random.choice(range(1, 100))
                graph.din[i].append(j)
                graph.dout[j].append(i)
                graph.edges[(i, j)] = cost
                graph.din[j].append(i)
                graph.dout[i].append(j)
                graph.edges[(j, i)] = cost
                graph.numberOfEdges += 2
        return graph
    
    def generate_complete_asymetric_graph(self, no_vertices: int) -> Graph:
        graph = Graph(no_vertices, 0)
        for i in range(no_vertices):
            for j in range(no_vertices):
                if i != j:
                    cost = random.choice(range(1, 100))
                    graph.din[i].append(j)
                    graph.dout[j].append(i)
                    graph.edges[(i, j)] = cost
                    graph.numberOfEdges += 1
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

    @property
    def graph(self) -> Graph:
        return self.__graph
    
    @graph.setter
    def graph(self, value: Graph):
        self.__graph = value

    #--- lab 2 ---#

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

    #--- lab 3 ---#
    
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
    
    def find_min_cost_paths(self, source: int, target: int) -> int:
        '''
        This function calculates the number of minimum cost paths between two vertices in a graph.
        It verifies the existence of the provided vertices within the graph and returns the starting vertex if both vertices are the same.
        The algorithm initializes a matrix of costs, where the cost of an edge is stored in the corresponding cell.
        It then iterates through the matrix, updating the cost of a path if a shorter path is found.
        The function returns a tuple consisting of the number of minimum cost paths and a list with the paths.
        '''
        n = self.number_of_vertices()
        cost_matrix = [[float('inf')] * n for _ in range(n)]
        num_paths = [[0] * n for _ in range(n)]

        for u in range(n):
            for v in range(n):
                if self.is_edge(u, v):
                    cost_matrix[u][v] = self.graph.edges[(u, v)] 
                    num_paths[u][v] = 1

        for k in range(n):
            for i in range(n):
                for j in range(n):
                    if cost_matrix[i][k] + cost_matrix[k][j] < cost_matrix[i][j]:
                        cost_matrix[i][j] = cost_matrix[i][k] + cost_matrix[k][j]
                        num_paths[i][j] = num_paths[i][k] * num_paths[k][j]
                    elif cost_matrix[i][k] + cost_matrix[k][j] == cost_matrix[i][j]:
                        num_paths[i][j] += num_paths[i][k] * num_paths[k][j]

        return num_paths[source][target]

    def find_all_possible_paths(self, source: int, target: int) -> int:
        '''
        This function calculates the number of possible paths between two vertices in a graph.
        It verifies the existence of the provided vertices within the graph and returns the starting vertex if both vertices are the same.
        The algorithm initializes a matrix of costs, where the cost of an edge is stored in the corresponding cell.
        It then iterates through the matrix, updating the cost of a path if a shorter path is found.
        The function returns the number of possible paths between the two vertices.
        '''
        n = self.number_of_vertices()
        num_paths = [[0] * n for _ in range(n)]

        for u in range(n):
            for v in range(n):
                if self.is_edge(u, v):
                    num_paths[u][v] = 1

        for k in range(n):
            for i in range(n):
                for j in range(n):
                    num_paths[i][j] += num_paths[i][k] * num_paths[k][j]

        return num_paths[source][target] 
    
    #--- lab 4 ---#

    def TopoSortDFS(self, vertex: int, sorted: list, fullyProcessed: set, inProcess: set) -> bool:
        '''
        This function performs a depth-first search on the graph, topologically sorting the vertices.
        The function returns True if the graph is acyclic, False otherwise.
        A graph is DAG if and ony if there exists a topological sort of the vertices.
        '''
        inProcess.add(vertex)
        for neighbor in self.get_inbounds_of_vertex(vertex):
            if neighbor in inProcess:
                return False
            if neighbor not in fullyProcessed:
                if not self.TopoSortDFS(neighbor, sorted, fullyProcessed, inProcess):
                    return False
        inProcess.remove(vertex)
        sorted.append(vertex)
        fullyProcessed.add(vertex)
        return True

    def topological_sort(self) -> list:
        '''
        This function performs a topological sort on the graph.
        It verifies the existence of the provided vertices within the graph.
        The function returns a list with the topologically sorted vertices.
        '''
        sorted = []
        fullyProcessed = set()
        inProcess = set()
        for vertex in self.get_vertices():
            if vertex not in fullyProcessed:
                if not self.TopoSortDFS(vertex, sorted, fullyProcessed, inProcess):
                    return None
        return sorted
    
    def count_paths(self, start: int, end: int) -> int:
        '''
        This function calculates the number of paths between two vertices in a graph.
        It verifies the existence of the provided vertices within the graph and returns the starting vertex if both vertices are the same.
        The function returns the number of paths between the two vertices.
        '''
        if not ((self.is_vertex(start) and self.is_vertex(end))):
            raise RepoError("\nVertices do not exist in the graph\n")

        if start == end:
            return 1

        sorted_vertices = self.topological_sort()
        if sorted_vertices is None:
            raise RepoError("\nGraph contains a cycle\n")

        num_paths = [0] * self.number_of_vertices()
        num_paths[start] = 1

        for vertex in sorted_vertices:
            for neighbor in self.get_outbounds_of_vertex(vertex):
                num_paths[neighbor] += num_paths[vertex]

        return num_paths[end]

    def count_lowest_cost_paths(self, start: int, end: int) -> tuple:
        '''
        This function calculates the number of lowest cost paths between two vertices in a graph.
        It verifies the existence of the provided vertices within the graph and returns the starting vertex if both vertices are the same.
        The function returns a tuple consisting of the number of lowest cost paths and the lowest cost.
        '''
        if not ((self.is_vertex(start) and self.is_vertex(end))):
            raise RepoError("\nVertices do not exist in the graph\n")
        
        if start == end:
            return 1, 0

        sorted_vertices = self.topological_sort()
        if sorted_vertices is None:
            raise RepoError("\nGraph contains a cycle\n")

        num_paths = [0] * self.number_of_vertices()
        lowest_cost = [float('inf')] * self.number_of_vertices()
        num_paths[start] = 1
        lowest_cost[start] = 0

        for vertex in sorted_vertices:
            for neighbor in self.get_outbounds_of_vertex(vertex):
                cost = self.graph.get_cost(vertex, neighbor)
                if lowest_cost[vertex] + cost < lowest_cost[neighbor]:
                    lowest_cost[neighbor] = lowest_cost[vertex] + cost
                    num_paths[neighbor] = num_paths[vertex]
                elif lowest_cost[vertex] + cost == lowest_cost[neighbor]:
                    num_paths[neighbor] += num_paths[vertex]

        return num_paths[end], lowest_cost[end]

    #--- lab 5 ---#
    # brute force method
    def calculate_cycle_cost(self, cycle: list) -> int:
        '''
        This function calculates the cost of a cycle in a graph, provided that the graph is complete.
        So, there will be no need to check if the edge exists.
        Also, the cycle will always be valid.
        The function returns the cost of the cycle.
        '''
        cost = 0
        n = len(cycle)
        for i in range(n):
            cost += self.graph.get_cost(cycle[i], cycle[(i + 1) % n])
        return cost
    
    def TSP_brute_force(self) -> tuple:
        '''
        Travelling salesman problem - brute force method
        This function solves the TSP using a brute force method
        The function returns a tuple consisting of the minimum cost and the cycle which 
        generated that mininum cost
        '''
        if (self.number_of_edges() != self.number_of_vertices() * (self.number_of_vertices() - 1)):
            raise RepoError("\nGraph is not complete\n")
        vertices = self.get_vertices()
        min_cycle = []
        min_cost = float('inf')
        for permutation in itertools.permutations(vertices):
            current_cost = self.calculate_cycle_cost(permutation)
            if current_cost < min_cost:
                min_cost = current_cost
                min_cycle = permutation
        return min_cost, min_cycle

    # greedy method
    def TSP_greedy(self) -> tuple:
        '''
        Travelling salesman problem - greedy method
        This function solves the TSP using a greedy method
        The function returns a tuple consisting of the minimum cost and the cycle which
        NOTE: that the greedy method does not always return the optimal solution!
        It just returns a solution which is close to the optimal one in a much smaller time frame
        '''
        n = self.number_of_vertices()
        final_cost = float('inf')
        final_cycle = []
        for start in range(n): 
            # takes into consideration all possible starting vertices 
            # to get a better approximation
            visited = [False] * n
            cycle = [start]
            visited[start] = True
            total_cost = 0
            current = start
            for _ in range(n - 1): 
                # to ensure that we visit all other vertices exactly once
                next_vertex = None
                min_cost = float('inf')
                for v in range(n):
                    if not visited[v] and self.graph.get_cost(current, v) < min_cost:
                        next_vertex = v
                        min_cost = self.graph.get_cost(current, v)
                cycle.append(next_vertex)
                visited[next_vertex] = True
                total_cost += min_cost
                current = next_vertex
            cycle.append(cycle[0])
            total_cost += self.graph.get_cost(cycle[-2], cycle[-1])
            if total_cost < final_cost:
                final_cost = total_cost
                final_cycle = cycle
        return final_cost, final_cycle                
        