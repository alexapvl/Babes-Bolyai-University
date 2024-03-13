from domain.domain import Graph
from service.service import GraphService
from ui.ui import UI

if __name__ == "__main__":
    graph = Graph(10)
    service = GraphService(graph)
    ui = UI(service)
    ui.start()  
    