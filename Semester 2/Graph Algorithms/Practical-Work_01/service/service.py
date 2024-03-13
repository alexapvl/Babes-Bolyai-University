from domain.domain import Graph

class GraphError(ValueError):
    pass

class GraphService():
    def __init__(self, graph: Graph) -> None:
        self.graph = graph
        
    def add_vertex(self, i):
        """Adds a vertex to the graph if it does not already exist

        Args:
            i : "name" of the vertex 
        """
        if i in self.graph.vertices:
            raise GraphError("\nVertex already exists in the graph\n")
        self.graph.vertices.append(i)
        
    def add_edge(self, i, j, cost):
        """Adds an edge to the directed graph from i to j
        First it checks if the edge already exists, then if not, 
        it adds it to the graph along with the cost of that edge

        Args:
            i : first vertex (out)
            j : second vertex (in)
            cost : the cost of the edge
        """
        if (i, j) in self.graph.edges.keys():
            raise GraphError("\nEdge already exists in the graph\n")
        if type(cost) != int:
            raise GraphError("\nCost must be an integer\n")
        self.graph.edges[(i, j)] = cost
        self.graph.dout[i].append(j)
        self.graph.din[j].append(i)
        
        
    def is_vertex(self, i) -> bool:
        """Checks if a vertex exists in the graph

        Args:
            i : "name" of the vertex
        """
        if i in self.graph.vertices:
            return True
        return False
        
    def is_edge(self, i, j) -> bool:
        """Checks if an edge exists in the graph

        Args:
            i : first vertex (out)
            j : second vertex (in)
        """
        if (i, j) in self.graph.edges.keys():
            return True
        return False
        