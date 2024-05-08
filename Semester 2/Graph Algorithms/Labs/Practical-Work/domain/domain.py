class Graph():
    def __init__(self, no_vertices=0, no_edges=0) -> None:
        self.vertices = [] # list of vertices
        self.edges = {} # (i, j): cost | where the tuple (i, j) is the key
        self.din = {} # inbound
        self.dout = {} # outbound
        self.numberOfEdges = no_edges # number of edges
        for i in range(no_vertices):
            # add each vertex to the list of vertices and create the inbounds and outbounds
            self.vertices.append(i)
            self.din[i] = []
            self.dout[i] = []
        
    def get_cost(self, i: int, j: int) -> int:
        '''
        Returns the cost of the edge (i, j)
        '''
        return self.edges[(i, j)]

    @property
    def vertices(self) -> list:
        return self.__vertices
    
    @vertices.setter
    def vertices(self, value: list):
        self.__vertices = value
    
    @property
    def edges(self) -> dict:
        return self.__edges
    
    @edges.setter
    def edges(self, value: dict):
        self.__edges = value
        
    @property
    def din(self) -> dict:
        return self.__din
    
    @din.setter
    def din(self, value: dict):
        self.__din = value
        
    @property
    def dout(self) -> dict:
        return self.__dout
    
    @dout.setter
    def dout(self, value: dict):
        self.__dout = value
        
    @property
    def numberOfEdges(self) -> int:
        return self.__numberOfEdges
    
    @numberOfEdges.setter
    def numberOfEdges(self, value: int):
        self.__numberOfEdges = value
    
    def __str__(self) -> str:
        string = ""
        # outbounds
        string += "Outbounds:\n"
        for i in self.vertices:
            string += f"{i} -> {self.dout[i]}\n"
        string += "\n"
        # inbounds
        string += "Inbounds:\n"
        for i in self.vertices:
            string += f"{i} <- {self.din[i]}\n"
        string += "\n"
        # edges
        string += "Edges:\n"
        if self.edges == {}:
            string += "No edges\n"
        else:
            for edge in self.edges.keys():
                string += f"Edge: {edge[0]} -> {edge[1]} Cost: {self.edges[edge]}\n"
        return string

class VertexWolfGoatCabbage:
    '''
    Implemented for the bonus problem at lab 2
    Vertex class for the Wolf, Goat, Cabbage problem
    The vertex is represented as a 4-bit number, where the i-th bit represents the i-th object
    The least significant bit is the boat
    The second least significant bit is the cabbage
    The third least significant bit is the goat
    The most significant bit is the wolf
    '''
    def __init__(self, v):
        self.__v = v
    def __repr__(self):
        sides = ([], [])
        for i in range(4):
            name = ["wolf", "goat", "cabbage", "boat"][i]
            value = self.get_side(i)
            sides[value].append(name)
        return f"({','.join(sides[0])}|{','.join(sides[1])})"
    def __str__(self):
        return self.__repr__()
    def __eq__(self, other):
        if not isinstance(other, VertexWolfGoatCabbage):
            return False
        return self.__v == other.__v
    def __hash__(self):
        return self.__v
    def get_side(self, i):
        return 0 if ((self.__v & (1 << i)) == 0) else 1
    def is_valid(self):
        return self.get_side(3) == self.get_side(1) or (
            self.get_side(1) != self.get_side(0) and self.get_side(1) != self.get_side(2))
        
    def parse_neighbors(self):
        for i in range(4):
            v = self.__v
            if self.get_side(i) == self.get_side(3):
                v ^= (1 << 3)
                if i != 3:
                    v ^= (1 << i)
                vertex = VertexWolfGoatCabbage(v)
                if vertex.is_valid():
                    yield vertex

class WolfGoatCabbageGraph:
    '''
    Implemented for the bonus problem at lab 2
    '''
    def dout(self, x: VertexWolfGoatCabbage):
        '''Returns something iterable that contains all the outbound neighbors of vertex x
            Precondition: x is a valid vertex of the graph.
        '''
        return x.parse_neighbors()
    def din(self, x: VertexWolfGoatCabbage):
        '''Returns something iterable that contains all the inbound neighbors of vertex x
            Precondition: x is a valid vertex of the graph.
        '''
        return x.parse_neighbors()
    def initial_state(self):
        '''
        Returns the initial state of the graph
        '''
        return VertexWolfGoatCabbage(0)
    def final_state(self):
        '''
        Returns the final state of the graph
        '''
        return VertexWolfGoatCabbage(15)
