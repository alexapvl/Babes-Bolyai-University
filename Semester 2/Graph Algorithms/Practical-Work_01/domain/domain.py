class Graph():
    def __init__(self, n=0) -> None:
        self.vertices = []
        self.edges = {} # (i, j): cost | where the tuple (i, j) is the key
        self.din = {}
        self.dout = {}
        for i in range(1, n+1):
            self.vertices.append(i)
            self.din[i] = []
            self.dout[i] = []
    
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
    