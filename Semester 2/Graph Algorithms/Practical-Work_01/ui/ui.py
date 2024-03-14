from service.service import GraphService, GraphError

class UIError(ValueError):
    pass

class UI():
    def __init__(self, service: GraphService):
        self.service = service
    
    def print_menu(self):
        print("1. Add vertex")
        print("2. Add edge")
        print("3. Check if vertex exists")
        print("4. Check if edge exists")
        print("5. Print graph")
        print("0. Exit")
    
    def start(self):
        while(True):
            try:
                self.print_menu()
                command = input(">> ")
                if command == "1":
                    i = int(input("Enter vertex: "))
                    self.service.add_vertex(i)
                elif command == "2":
                    i = int(input("Enter first vertex: "))
                    j = int(input("Enter second vertex: "))
                    cost = int(input("Enter cost: "))
                    self.service.add_edge(i, j, cost)
                elif command == "3":
                    i = int(input("Enter vertex: "))
                    if self.service.is_vertex(i):
                        print(f"\nVertex {i} exists in the graph\n")
                    else:
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                elif command == "4":
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
                elif command == "5":
                    print(self.service.graph)
                elif command == "0":
                    break
                else:
                    raise UIError("\nInvalid command\n")
            except UIError as ue:
                print(ue)
            except GraphError as ge:
                print(ge)
            except ValueError as ve:
                print("\nInvalid input\n")
                