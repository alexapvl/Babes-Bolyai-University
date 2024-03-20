from repository.repository import RepoError
from service.service import GraphService

class UIError(ValueError):
    pass

class UI():
    def __init__(self, service: GraphService):
        self.service = service
    
    def print_menu(self):
        print("1. Add vertex")
        print("2. Remove vertex")
        print("3. Add edge")
        print("4. Remove edge")
        print("5. Check if vertex exists")
        print("6. Check if edge exists")
        print("7. Generate random graph given the number of vertices and edges. The graph will be written to a separate file.")
        print("8. Print graph")
        print("0. Exit")
    
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
                    i = int(input("Enter vertex: "))
                    if self.service.is_vertex(i):
                        print(f"\nVertex {i} exists in the graph\n")
                    else:
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                elif command == "6":
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
                elif command == "7":
                    no_vertices = int(input("Enter number of vertices: "))
                    no_edges = int(input("Enter number of edges: "))
                    graph = self.service.generate_random_graph(no_vertices, no_edges)
                    self.service.write_randomly_generated_graph_to_file(graph, "text_files/random_graph.txt")
                elif command == "8":
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
                