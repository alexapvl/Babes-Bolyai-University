from domain.domain import Graph
from repository.repository import Repository
from service.service import GraphService
from ui.ui import UI

if __name__ == "__main__":
    graph = Graph()
    repo = Repository(graph)
    service = GraphService(repo, "")
    ui = UI(service)
    ui.start()  # start the program
    