from domain.domain import WolfGoatCabbageGraph, VertexWolfGoatCabbage
from repository.repository import RepoError
from service.service import GraphService
from tabulate import tabulate

class UIError(ValueError):
    pass

class UI():
    def __init__(self, service: GraphService):
        self.service = service
    
    def start(self):
        while(True): # will read a file until a valid file is given
            try:
                self.service.fileName = "textFiles/" + input("Enter file name: ") + ".txt"
                self.service.read_file()
                break
            except FileNotFoundError:
                print("File does not exist")
        while(True):
            self.print_menu()
            command = input(">> ")
            if command == "1":
                self.start_lab1()
            elif command == "2":
                self.start_lab2()
            elif command == "3":
                self.start_lab3()
            elif command == "4":
                self.start_lab4()
            elif command == "0":
                break
            else:
                print("\nInvalid command\n")
    
    def print_menu(self):
        print("\n----- Choose lab number")
        print("1. Lab 1")
        print("2. Lab 2")
        print("3. Lab 3")
        print("4. Lab 4")
        print("0. Exit\n")
    
    def print_lab1_menu(self):
        print("\n----- Edit the graph")
        print("1. Add vertex")
        print("2. Remove vertex")
        print("3. Add edge")
        print("4. Remove edge")
        print("5. Update cost of an edge")
        print("\n----- Check the graph")
        print("6. Check if vertex exists")
        print("7. Check if edge exists")
        print("8. In degree of vertex")
        print("9. Out degree of vertex")
        print("10. Number of vertices")
        print("11. Number of edges")
        print("\n----- Print")
        print("12. Print vertices")
        print("13. Print inbounds for specific vertex")
        print("14. Print outbounds for specific vertex")
        print("15. Print graph")
        print("\n----- Other")
        print("16. Create a copy of the current graph and write it into a separate file")
        print("17. Generate random graph given the number of vertices and edges. The graph will be written to a separate file.")
        print("18. Clear the graph")
        print("0. Back\n")
    
    def print_lab2_menu(self):
        print("\n----- Choose the algorithm")
        print("1. Breadth-first search")
        print("2. Wolf, goat and cabbadge problem(bonus)")
        print("0. Back\n")

    def print_lab3_menu(self):
        print("\n----- Choose the algorithm")
        print("1. Floyd-Warshall algorithm")
        print("2. Find the number of distinct minimum cost walks between a pair of vertices(bonus 1)")
        print("3. Find the number of possible paths between two vertices(bonus 2)")
        print("0. Back\n")

    def print_lab4_menu(self):
        print("\n----- Choose the algorithm")
        print("1. Topological sort")
        print("0. Back\n")

    def askToSave(self):
        choice = input("Do you want to save changes to a separate file? y/n >> ")
        choice = choice.lower()
        newFileName = self.service.fileName[:-4] + "-copy.txt"
        if choice == "y":
            self.service.write_given_graph_to_file(self.service.repo.graph, newFileName)

    def start_lab1(self):
        while(True):
            try:
                self.print_lab1_menu()
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
                    i = int(input("Enter vertex: "))
                    if not self.service.is_vertex(i):
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                    print(f"\nIn degree of vertex {i} is {self.service.in_degree_of_vertex(i)}\n")
                elif command == "9":
                    i = int(input("Enter vertex: "))
                    if not self.service.is_vertex(i):
                        raise UIError(f"\nVertex {i} does not exist in the graph\n")
                    print(f"\nOut degree of vertex {i} is {self.service.out_degree_of_vertex(i)}\n")
                elif command == "10":
                    print(f"\nThe graph has {self.service.number_of_vertices()} vertices\n")
                elif command == "11":
                    print(f"\nThe graph has {self.service.number_of_edges()} edges\n")
                elif command == "12":
                    print("Vertices: ", end="")
                    for vertex in self.service.get_vertices():
                        print(vertex, end=" | ")
                    print()
                elif command == "13":
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
                elif command == "14":
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
                elif command == "15":
                    print(self.service.repo.graph)
                elif command == "16":
                    copyGraph = self.service.copy_graph()
                    name_of_file = input("Enter the name of the file: ")
                    name_of_file = "textFiles/" + name_of_file + ".txt"
                    self.service.write_given_graph_to_file(copyGraph, name_of_file)
                    print("\nCopy of the graph was succesfully written to the file\n")
                elif command == "17":
                    no_vertices = int(input("Enter number of vertices: "))
                    no_edges = int(input("Enter number of edges: "))
                    file_name = input("Enter file name: ")
                    actual_file_path = "textFiles/" + file_name + ".txt"
                    if no_edges > no_vertices ** 2:
                        # create the file with the name and write the error message in it
                        self.service.write_given_graph_to_file(None, file_name)
                    else:
                        graph = self.service.generate_random_graph(no_vertices, no_edges)
                        self.service.write_given_graph_to_file(graph, actual_file_path)
                        print(f"\nRandom graph was succesfully written to the file with the name: {file_name}\n")
                elif command == "18":
                    self.service.clear_graph()
                    print("\nGraph was cleared\n")
                elif command == "0":
                    self.askToSave()
                    break
                else:
                    raise UIError("\nInvalid command\n")
            except UIError as ue:
                print(ue)
            except RepoError as ge:
                print(ge)
            except ValueError as ve:
                print("\nInvalid input\n")

    def start_lab2(self):
        while(True):
            try:
                self.print_lab2_menu()
                command = input(">> ")
                if command == "1":
                    start = int(input("Enter start vertex: "))
                    end = int(input("Enter end vertex: "))
                    path = self.service.shortest_path_between_two_vertices_forward_breath_first_search(start, end)
                    if path is None:
                        print("\nThere is no path between the two vertices\n")
                    else:
                        print("\nLength of the path: ", len(path) - 1) # length of the path is the number of edges
                        print("Path: ", end="")
                        for vertex in path[:-1]:
                            print(vertex, end=" -> ")
                        print(path[-1])
                elif command == "2":
                    path = self.service.shortest_path_wgc()
                    print("\nLength of the path: ", len(path) - 1) # length of the path is the number of edges
                    print("", end="")
                    for vertex in path[:-1]:
                        print(vertex, end=" -> ")
                    print(path[-1])
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

    def start_lab3(self):
        while(True):
            try:
                self.print_lab3_menu()
                command = input(">> ")
                if command == "1":
                    start = int(input("Enter start vertex: "))
                    end = int(input("Enter end vertex: "))
                    intermediate_matrices, path, cost = self.service.lowest_cost_walk_floyd_warshall(start, end)
                    if cost == float('inf'):
                        print("\nThere is no path between the two vertices\n")
                    else:
                        print("\nLength of the path: ", len(path) - 1)
                        print("Cost of the path: ", cost)
                        print("Path: ", end="")
                        for vertex in path[:-1]:
                            print(vertex, end=" -> ")
                        print(path[-1])
                        row_and_col_headers = [f"Vertex {vertex}" for vertex in self.service.get_vertices()]
                        print("\nInitial matrices D and P:")
                        print(f"-----D-----")
                        print(tabulate(intermediate_matrices[0][0], headers=row_and_col_headers, showindex=row_and_col_headers, tablefmt="fancy_grid"))
                        print(f"-----P-----")
                        print(tabulate(intermediate_matrices[0][1], headers=row_and_col_headers, showindex=row_and_col_headers, tablefmt="fancy_grid"))
                        index = 0
                        for matrix in intermediate_matrices[1:]:
                            print(f"\nIntermediate matrices D{index} and P{index}:")
                            print(f"k = {index} -> using vertex {index} as an intermediate vertex")
                            print(f"-----D{index}-----")
                            print(tabulate(matrix[0], headers=row_and_col_headers, showindex=row_and_col_headers, tablefmt="fancy_grid"))
                            print(f"-----P{index}-----")
                            print(tabulate(matrix[1], headers=row_and_col_headers, showindex=row_and_col_headers, tablefmt="fancy_grid"))
                            index += 1
                    final_matrix = intermediate_matrices[-1][0]
                    while(True):
                        stop = input("\nDo you want to stop? y/n >> ")
                        if stop == "y":
                            break
                        start = int(input("Enter start vertex: "))
                        end = int(input("Enter end vertex: "))
                        print(f"\nMinimum cost of the path between {start} and {end}: {final_matrix[start][end]}\n")
                elif command == "2":
                    start = int(input("Enter start vertex: "))
                    end = int(input("Enter end vertex: "))
                    number_of_paths = self.service.find_min_cost_paths(start, end);
                    print(f"\nNumber of minimum cost paths between {start} and {end}: {number_of_paths}")
                elif command == "3":
                    start = int(input("Enter start vertex: "))
                    end = int(input("Enter end vertex: "))
                    number_of_paths = self.service.find_all_possible_paths(start, end);
                    print(f"\nNumber of possible paths between {start} and {end}: {number_of_paths}")
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

    def start_lab4(self):
        while(True):
            try:
                self.print_lab4_menu()
                command = input(">> ")
                if command == "1":
                    topological_order = self.service.topological_sort()
                    if topological_order is None:
                        print("\nGraph is not a DAG\n")
                    else:
                        print("\nTopological order: ", end="")
                        for vertex in topological_order:
                            print(vertex, end=" | ")
                        print()
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
            