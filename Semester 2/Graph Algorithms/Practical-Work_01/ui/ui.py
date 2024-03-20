from repository.repository import RepoError
from service.service import GraphService

class UIError(ValueError):
    pass

class UI():
    def __init__(self, service: GraphService):
        self.service = service
    
    def print_menu(self):
        print("\n1. Add vertex")
        print("2. Remove vertex")
        print("3. Add edge")
        print("4. Remove edge")
        print("5. Update cost of an edge")
        print("6. Check if vertex exists")
        print("7. Check if edge exists")
        print("8. Create a copy of the current graph and write it into a separate file")
        print("9. Generate random graph given the number of vertices and edges. The graph will be written to a separate file.")
        print("10. In degree of vertex")
        print("11. Out degree of vertex")
        print("12. Number of vertices")
        print("13. Print vertices")
        print("14. Print inbounds for specific vertex")
        print("15. Print outbounds for specific vertex")
        print("16. Print graph")
        print("0. Exit\n")
    
    def start(self):
        self.service.read_file()
        while(True):
            try:
                self.print_menu()
                command = input(">> ")
                if command == "1":
                    i = int(input("Enter vertex: "))
                    self.service.add_vertex(i)
                elif command == "2":
                    i = int(input("Enter vertex: "))
                    self.service.remove_vertex(i)
                elif command == "3":
                    i = int(input("Enter first vertex: "))
                    j = int(input("Enter second vertex: "))
                    cost = int(input("Enter cost: "))
                    self.service.add_edge(i, j, cost)
                elif command == "4":
                    i = int(input("Enter first vertex: "))
                    j = int(input("Enter second vertex: "))
                    self.service.remove_edge(i, j)
                elif command == "5":
                    i = int(input("Enter first vertex: "))
                    j = int(input("Enter second vertex: "))
                    if not self.service.is_edge(i, j):
                        raise UIError(f"\nEdge from {i} to {j} does not exist in the graph\n")
                    cost = int(input("Enter new cost: "))
                    self.service.update_edge_cost(i, j, cost)
                elif command == "6":
                    i = int(input("Enter vertex: "))
                    if self.service.is_vertex(i):
                        print(f"\nVertex {i} exists in the graph\n")
                    else:
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                elif command == "7":
                    i = int(input("Enter first vertex: "))
                    if not self.service.is_vertex(i):
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                    j = int(input("Enter second vertex: "))
                    if not self.service.is_vertex(j):
                        raise UIError(f"\nVertex {j} does not exist in the graph\n")
                    if self.service.is_edge(i, j):
                        print(f"\nEdge from {i} to {j} exists in the graph\n")
                    else:
                        raise UIError(f"\nEdge from {i} to {j} does not exist in the graph\n")
                elif command == "8":
                    copyGraph = self.service.copy_graph()
                    self.service.write_given_graph_to_file(copyGraph, "text_files/copy_graph.txt")
                    print("\nCopy of the graph was succesfully written to the file\n")
                elif command == "9":
                    no_vertices = int(input("Enter number of vertices: "))
                    no_edges = int(input("Enter number of edges: "))
                    graph = self.service.generate_random_graph(no_vertices, no_edges)
                    self.service.write_given_graph_to_file(graph, "text_files/random_graph.txt")
                    print("\nRandom graph was succesfully written to the file\n")
                elif command == "10":
                    i = int(input("Enter vertex: "))
                    if not self.service.is_vertex(i):
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                    print(f"\nIn degree of vertex {i} is {self.service.in_degree_of_vertex(i)}\n")
                elif command == "11":
                    i = int(input("Enter vertex: "))
                    if not self.service.is_vertex(i):
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                    print(f"\nOut degree of vertex {i} is {self.service.out_degree_of_vertex(i)}\n")
                elif command == "12":
                    print(f"\nThe graph has {self.service.number_of_vertices()} vertices\n")
                elif command == "13":
                    print("Vertices: ", end="")
                    for vertex in self.service.get_vertices():
                        print(vertex, end=" | ")
                    print()
                elif command == "14":
                    i = int(input("Enter vertex: "))
                    if not self.service.is_vertex(i):
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                    if len(self.service.get_inbounds_of_vertex(i)) == 0:
                        print(f"\nVertex {i} has no inbounds\n")
                    else:    
                        print(f"\nInbounds for vertex {i}: ", end="")
                        for vertex in self.service.get_inbounds_of_vertex(i):
                            print(vertex, end=" | ")
                    print()
                elif command == "15":
                    i = int(input("Enter vertex: "))
                    if not self.service.is_vertex(i):
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                    if len(self.service.get_outbounds_of_vertex(i)) == 0:
                        print(f"\nVertex {i} has no outbounds\n")
                    else:    
                        print(f"\nOutbounds for vertex {i}: ", end="")
                        for vertex in self.service.get_outbounds_of_vertex(i):
                            print(vertex, end=" | ")
                    print()
                elif command == "16":
                    print(self.service.repo.graph)
                elif command == "0":
                    break
                else:
                    raise UIError("\nInvalid command\n")
            except UIError as ue:
                print(ue)
            except RepoError as ge:
                print(ge)
            except ValueError as ve:
                print("\nInvalid input\n")
                