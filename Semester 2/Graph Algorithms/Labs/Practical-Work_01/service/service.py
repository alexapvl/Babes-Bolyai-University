from domain.domain import Graph
from repository.repository import Repository, RepoError

class GraphService():
    def __init__(self, repository: Repository , file_name: str) -> None:
        self.repo = repository
        self.fileName = file_name
        
    def read_file(self):
        """
        Reads a graph from a file and stores it in the repository
        Args:
            file_name : the name of the file
        """
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
                else: # isolated vertices are written alone on a line just to mark them, they are already added when the graph is created
                    continue
                
    def write_file(self):
        """
        Writes the whole graph to the file, overwriting the previous content
        Args:
            file_name : the name of the file
        """
        with open(self.fileName, "w") as file:
            file.write(f"{len(self.repo.graph.vertices)} {self.repo.graph.numberOfEdges}\n")
            for edge in self.repo.graph.edges.keys():
                file.write(f"{edge[0]} {edge[1]} {self.repo.graph.edges[edge]}\n")
            for isolatedVertex in self.get_isolated_vertices():
                file.write(f"{isolatedVertex}\n")
    
    def write_given_graph_to_file(self, graph: Graph, file_name: str):
        """
        Writes a randomly generated graph to a file
        Args:
            graph : the graph to be written
            file_name : the name of the file
        """
        if graph is None:
            with open(file_name, "w") as file:
                file.write("The graph with the given number of vertices and edged is impossile to create.\nThe number of edges is greater than the maximum number of edges possible.\n")
            return
        with open(file_name, "w") as file:
            file.write(f"{len(graph.vertices)} {graph.numberOfEdges}\n")
            for edge in graph.edges.keys():
                file.write(f"{edge[0]} {edge[1]} {graph.edges[edge]}\n")
            for isolatedVertex in graph.vertices:
                if len(graph.din[isolatedVertex]) == 0 and len(graph.dout[isolatedVertex]) == 0:
                    file.write(f"{isolatedVertex}\n")

    def add_vertex(self, i):
        """
        Adds a vertex to the graph if it does not already exist
        Args:
            i : "name" of the vertex 
        """
        self.repo.add_vertex(i)
        self.write_file()

    def remove_vertex(self, i):
        """
        Removes a vertex from the graph if it exists
        Args:
            i : "name" of the vertex
        """
        self.repo.remove_vertex(i)
        self.write_file()
    
    def add_edge(self, i, j, cost):
        """
        Adds an edge to the directed graph from i to j
        First it checks if the edge already exists, then if not, 
        it adds it to the graph along with the cost of that edge
        Args:
            i : first vertex (out)
            j : second vertex (in)
            cost : the cost of the edge
        """
        self.repo.add_edge(i, j, cost)
        self.write_file()
    
    def remove_edge(self, i, j):
        """
        Removes an edge from the graph
        Args:
            i : first vertex (out)
            j : second vertex (in)
        """
        self.repo.remove_edge(i, j)
        self.write_file()
    
    def is_vertex(self, i) -> bool:
        """
        Checks if a vertex exists in the graph
        Args:
            i : "name" of the vertex
        """
        return self.repo.is_vertex(i)

    def is_edge(self, i, j) -> bool:
        """
        Checks if an edge exists in the graph
        Args:
            i : first vertex (out)
            j : second vertex (in)
        """
        return self.repo.is_edge(i, j)

    def get_isolated_vertices(self) -> list:
        """
        Returns a list of isolated vertices
        """
        return self.repo.get_isolated_vertices()
    
    def copy_graph(self) -> Graph:
        """
        Returns a copy of the current graph
        """
        return self.repo.copy_graph()

    def generate_random_graph(self, no_vertices: int, no_edges: int) -> Graph:
        """
        Generates a random graph with a given number of vertices and edges
        Args:
            no_vertices : number of vertices
            no_edges : number of edges
        """
        return self.repo.generate_random_graph(no_vertices, no_edges)

    def get_vertices(self) -> list:
        """
        Returns a list of vertices
        """
        return self.repo.get_vertices()

    def update_edge_cost(self, i: int, j: int, cost: int):
        """
        Updates the cost of an edge
        Args:
            i : first vertex (out)
            j : second vertex (in)
            cost : the new cost
        """
        self.repo.update_edge_cost(i, j, cost)
        self.write_file()
    
    def in_degree_of_vertex(self, i: int) -> int:
        """
        Returns the in-degree of a vertex
        Args:
            i : "name" of the vertex
        """
        return self.repo.in_degree_of_vertex(i)

    def out_degree_of_vertex(self, i: int) -> int:
        """
        Returns the out-degree of a vertex
        Args:
            i : "name" of the vertex
        """
        return self.repo.out_degree_of_vertex(i)

    def number_of_vertices(self) -> int:
        """
        Returns the number of vertices in the graph
        """
        return self.repo.number_of_vertices()
    
    def number_of_edges(self) -> int:
        """
        Returns the number of edges in the graph
        """
        return self.repo.number_of_edges()

    def get_vertices(self) -> list:
        """
        Returns a list of vertices
        """
        return self.repo.get_vertices()

    def get_inbounds_of_vertex(self, i: int) -> list:
        """
        Returns a list of inbounds of a vertex
        Args:
            i : "name" of the vertex
        """
        return self.repo.get_inbounds_of_vertex(i)

    def get_outbounds_of_vertex(self, i: int) -> list:
        """
        Returns a list of outbounds of a vertex
        Args:
            i : "name" of the vertex
        """
        return self.repo.get_outbounds_of_vertex(i)
        
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
