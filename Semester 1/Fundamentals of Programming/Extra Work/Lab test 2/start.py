from repository import Repository
from services import Service
from ui import UI


if __name__ == "__main__":
    repo = Repository()
    repo.load_file()
    service = Service(repo)
    ui = UI(service)
    ui.start()
