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
    