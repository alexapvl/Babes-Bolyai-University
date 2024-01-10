from src.domain.discipline import Discipline
from src.repository.memory.disciplines_memo_repo import DisciplinesMemoRepo
import pickle

class DisciplinesBinaryRepo(DisciplinesMemoRepo):
    def __init__(self, file_path: str):
        super().__init__()
        self.file_path = file_path

    # |----------------------FILE METHODS----------------------|

    def load_repo(self):
        try:
            with open(self.file_path, "rb") as file:
                while True:
                    discipline = pickle.load(file)
                    self.disciplines.append(discipline)
        except FileNotFoundError:
            self.disciplines = []
        except EOFError:
            pass

    def append_discipline_to_repo(self, discipline: Discipline):
        if not isinstance(discipline, Discipline):
            raise ValueError("Invalid discipline, must be of type Discipline")
        with open(self.file_path, "ab") as file:
            pickle.dump(discipline, file)

    def overwrite_repo(self):
        with open(self.file_path, "wb") as file:
            for discipline in self.disciplines:
                pickle.dump(discipline, file)

    # |--------------------REPOSITORY METHODS------------------|

    def add_discipline(self, discipline: Discipline):
        super().add_discipline(discipline)
        self.append_discipline_to_repo(discipline)

    def remove_discipline(self, discipline: Discipline):
        super().remove_discipline(discipline)
        self.overwrite_repo()

    def update_discipline(self, discipline: Discipline, new_discipline: Discipline):
        super().update_discipline(discipline, new_discipline)
        self.overwrite_repo()

    @property
    def file_path(self):
        return self.__file_path

    @file_path.setter
    def file_path(self, file_path):
        self.__file_path = file_path
