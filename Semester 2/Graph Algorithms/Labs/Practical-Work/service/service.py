from domain.domain import Graph
from repository.repository import Repository

class GraphService():
    def __init__(self, repository: Repository , file_name: str) -> None:
        self.repo = repository
        self.fileName = file_name
        
    def read_file(self):
        '''
        Reads a graph from a file and stores it in the repository
        Args:
            file_name : the name of the file
        '''
        with open(self.fileName, "r") as file:
            params = file.readline().split()
            numberOfVertices = int(params[0])
            numberOfEdges = int(params[1])
            self.repo.graph = Graph(numberOfVertices, numberOfEdges)
            for line in file:
                params = line.split()
                if len(params) == 3:
                    i = int(params[0])
                    j = int(params[1])
                    cost = int(params[2])
                    self.repo.add_edge(i, j, cost)
                elif len(params) == 1:
                    i = int(params[0])
                    self.repo.add_vertex(i)
                else: # isolated vertices are written alone on a line just to mark them, they are already added when the graph is created
                    continue
                
    def write_file(self):
        '''
        Writes the whole graph to the file, overwriting the previous content
        Args:
            file_name : the name of the file
        '''
        with open(self.fileName, "w") as file:
            file.write(f"{len(self.repo.graph.vertices)} {self.repo.graph.numberOfEdges}\n")
            for edge in self.repo.graph.edges.keys():
                file.write(f"{edge[0]} {edge[1]} {self.repo.graph.edges[edge]}\n")
            for isolatedVertex in self.get_isolated_vertices():
                file.write(f"{isolatedVertex}\n")
    
    def write_given_graph_to_file(self, graph: Graph, file_name: str):
        '''
        Writes a randomly generated graph to a file
        Args:
            graph : the graph to be written
            file_name : the name of the file
        '''
        file_name = "textFiles/" + file_name + ".txt"
        if graph is None:
            with open(file_name, "w") as file:
                file.write("The graph with the given number of vertices and edged is impossile to create.\nThe number of edges is greater than the maximum number of edges possible.\n")
            return

        with open(file_name, "w") as file:
            # create the file if it does not exist
            pass

        with open(file_name, "w") as file:
            file.write(f"{len(graph.vertices)} {graph.numberOfEdges}\n")
            for edge in graph.edges.keys():
                file.write(f"{edge[0]} {edge[1]} {graph.edges[edge]}\n")
            for isolatedVertex in graph.vertices:
                if len(graph.din[isolatedVertex]) == 0 and len(graph.dout[isolatedVertex]) == 0:
                    file.write(f"{isolatedVertex}\n")

    def add_vertex(self, i):
        '''
        Adds a vertex to the graph if it does not already exist
        Args:
            i : "name" of the vertex 
        Preconditions:
            The vertex must not already exist in the graph
        '''
        self.repo.add_vertex(i)

    def remove_vertex(self, i):
        '''
        Removes a vertex from the graph if it exists
        Args:
            i : "name" of the vertex
        Preconditions:
            The vertex must exist in the graph
        '''
        self.repo.remove_vertex(i)
    
    def add_edge(self, i, j, cost):
        '''
        Adds an edge to the directed graph from i to j
        First it checks if the edge already exists, then if not, 
        it adds it to the graph along with the cost of that edge
        Args:
            i : first vertex (out)
            j : second vertex (in)
            cost : the cost of the edge
        Preconditions:
            The vertices must exist in the graph
        '''
        self.repo.add_edge(i, j, cost)
    
    def remove_edge(self, i, j):
        '''
        Removes an edge from the graph
        Args:
            i : first vertex (out)
            j : second vertex (in)
        Preconditions:
            The edge must exist in the graph
        '''
        self.repo.remove_edge(i, j)
    
    def is_vertex(self, i) -> bool:
        '''
        Checks if a vertex exists in the graph
        Args:
            i : "name" of the vertex
        Preconditions:
            The vertex must exist in the graph
        '''
        return self.repo.is_vertex(i)

    def is_edge(self, i, j) -> bool:
        '''
        Checks if an edge exists in the graph
        Args:
            i : first vertex (out)
            j : second vertex (in)
        Preconditions:
            The edge must exist in the graph
        '''
        return self.repo.is_edge(i, j)

    def get_isolated_vertices(self) -> list:
        '''
        Returns a list of isolated vertices
        '''
        return self.repo.get_isolated_vertices()
    
    def copy_graph(self) -> Graph:
        '''
        Returns a copy of the current graph
        '''
        return self.repo.copy_graph()

    def generate_random_graph(self, no_vertices: int, no_edges: int) -> Graph:
        '''
        Generates a random graph with a given number of vertices and edges
        Args:
            no_vertices : number of vertices
            no_edges : number of edges
        Preconditions:
            The number of edges must be less than or equal to the maximum number of edges possible
        '''
        return self.repo.generate_random_graph(no_vertices, no_edges)
    
    def generate_complete_symetric_graph(self, no_vertices: int) -> Graph:
        return self.repo.generate_complete_symetric_graph(no_vertices)
    
    def generate_complete_asymetric_graph(self, no_vertices: int) -> Graph:
        return self.repo.generate_complete_asymetric_graph(no_vertices)
    
    def clear_graph(self):
        '''
        Clears the graph
        '''
        self.repo.clear_graph()

    def get_vertices(self) -> list:
        '''
        Returns a list of vertices
        '''
        return self.repo.get_vertices()

    def update_edge_cost(self, i: int, j: int, cost: int):
        '''
        Updates the cost of an edge
        Args:
            i : first vertex (out)
            j : second vertex (in)
            cost : the new cost
        Preconditions:
            The edge must exist in the graph
        '''
        self.repo.update_edge_cost(i, j, cost)
    
    def in_degree_of_vertex(self, i: int) -> int:
        '''
        Returns the in-degree of a vertex
        Args:
            i : "name" of the vertex
        Preconditions:
            The vertex must exist in the graph
        '''
        return self.repo.in_degree_of_vertex(i)

    def out_degree_of_vertex(self, i: int) -> int:
        '''
        Returns the out-degree of a vertex
        Args:
            i : "name" of the vertex
        Preconditions:
            The vertex must exist in the graph
        '''
        return self.repo.out_degree_of_vertex(i)

    def number_of_vertices(self) -> int:
        '''
        Returns the number of vertices in the graph
        '''
        return self.repo.number_of_vertices()
    
    def number_of_edges(self) -> int:
        '''
        Returns the number of edges in the graph
        '''
        return self.repo.number_of_edges()

    def get_vertices(self) -> list:
        '''
        Returns a list of vertices
        '''
        return self.repo.get_vertices()

    def get_inbounds_of_vertex(self, i: int) -> list:
        '''
        Returns a list of inbounds of a vertex
        Args:
            i : "name" of the vertex
        Preconditions:
            The vertex must exist in the graph
        '''
        return self.repo.get_inbounds_of_vertex(i)

    def get_outbounds_of_vertex(self, i: int) -> list:
        '''
        Returns a list of outbounds of a vertex
        Args:
            i : "name" of the vertex
        Preconditions:
            The vertex must exist in the graph
        '''
        return self.repo.get_outbounds_of_vertex(i)

    #--- lab 2 ---#
    
    def shortest_path_between_two_vertices_forward_breath_first_search(self, start: int, end: int) -> list:
        '''
        Returns the shortest path between two vertices using forward breath first search
        Args:
            start : the starting vertex
            end : the ending vertex
        Preconditions:
            Both vertices must exist in the graph
        '''
        return self.repo.shortest_path_between_two_vertices_forward_breath_first_search(start, end)

    def shortest_path_wgc(self):
        '''
        Returns the shortest path in the wolf, goat and cabbage problem
        '''
        return self.repo.shortest_path_wgc()
    
    #--- lab 3 ---#
    
    def lowest_cost_walk_floyd_warshall(self, start: int, end: int) -> tuple:
        '''
        Returns the lowest cost walk between two vertices using Floyd-Warshall algorithm
        Args:
            start : the starting vertex
            end : the ending vertex
        Preconditions:
            Both vertices must exist in the graph
        '''
        return self.repo.lowest_cost_walk_floyd_warshall(start, end)
    
    def find_min_cost_paths(self, source: int, target: int) -> int:
        '''
        Returns the number of minimum cost paths between two vertices
        Args:
            source : the starting vertex
            target : the ending vertex
        Preconditions:
            Both vertices must exist in the graph
        '''
        return self.repo.find_min_cost_paths(source, target)

    def find_all_possible_paths(self, start: int, target: int) -> int:
        '''
        Returns the number of all possible paths between two vertices
        Args:
            start : the starting vertex
            target : the ending vertex
        Preconditions:
            Both vertices must exist in the graph
        '''
        return self.repo.find_all_possible_paths(start, target)

    #--- lab 4 ---#
    
    def topological_sort(self) -> list:
        '''
        Returns a topological sort of the graph
        '''
        return self.repo.topological_sort()

    def count_paths(self, start: int, end: int) -> int:
        '''
        Returns the number of paths between two vertices
        Args:
            start : the starting vertex
            target : the ending vertex
        Preconditions:
            Both vertices must exist in the graph
        '''
        return self.repo.count_paths(start, end)

    def count_lowest_cost_paths(self, start: int, end: int) -> int:
        '''
        Returns the number of lowest cost paths between two vertices
        Args:
            start : the starting vertex
            target : the ending vertex
        Preconditions:
            Both vertices must exist in the graph
        '''
        return self.repo.count_lowest_cost_paths(start, end)

    #--- lab 5 ---#
    
    def TSP_brute_force(self) -> tuple:
        '''
        Returns the shortest path in the Travelling Salesman Problem using brute force
        Args:
            start : the starting vertex
            target : the ending vertex
        Preconditions:
            The graph is complete (symetric or asymetric)
        '''
        return self.repo.TSP_brute_force()

    def TSP_greedy(self) -> tuple:
        '''
        Returns the shortest path in the Travelling Salesman Problem using greedy algorithm
        Args:
            start : the starting vertex
            target : the ending vertex
        Preconditions:
            The graph is complete (symetric or asymetric)
        '''
        return self.repo.TSP_greedy()

        
    @property
    def repo(self) -> Repository:
        return self.__repo
    
    @repo.setter
    def repo(self, value: Repository):
        self.__repo = value
        
    @property
    def fileName(self) -> str:
        return self.__fileName

    @fileName.setter
    def fileName(self, value: str):
        self.__fileName = value
