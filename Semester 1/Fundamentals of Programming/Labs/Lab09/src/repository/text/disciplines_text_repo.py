from src.domain.discipline import Discipline
from src.repository.memory.disciplines_memo_repo import DisciplinesMemoRepo

class DisciplinesTextRepo(DisciplinesMemoRepo):
    def __init__(self, file_path: str):
        super().__init__()
        self.file_path = file_path

    # |----------------------FILE METHODS----------------------|

    def load_repo(self):
        try:
            with open(self.file_path, "r") as file:
                lines = file.readlines()
                self.disciplines = [DisciplinesTextRepo.unformat(line) for line in lines]
        except FileNotFoundError:
            self.disciplines = []

    def append_discipline_to_repo(self, discipline: Discipline):
        if not isinstance(discipline, Discipline):
            raise ValueError("Invalid discipline, must be of type Discipline")
        with open(self.file_path, "a") as file:
            file.write(DisciplinesTextRepo.format(discipline) + "\n")

    def overwrite_repo(self):
        with open(self.file_path, "w") as file:
            for discipline in self.disciplines:
                file.write(DisciplinesTextRepo.format(discipline) + "\n")

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

    @classmethod
    def format(cls, discipline: Discipline):
        """
        Creates a line of text from a discipline
        :param discipline: Discipline to be converted
        :return: String containing the discipline's ID and name(ID|name)
        """
        if not isinstance(discipline, Discipline):
            raise ValueError("Invalid discipline, must be of type Discipline")
        return discipline.id + "|" + discipline.name

    @classmethod
    def unformat(cls, line: str):
        """
        Creates a discipline from a line of text
        First splits the line by "|" to get the ID and name
        :param line: Line of text from the file
        :return: Discipline created from the line of text
        """
        if not isinstance(line, str):
            raise ValueError("Invalid line, must be of type str")
        line = line.strip()
        discipline = line.split("|")
        return Discipline(discipline[0], discipline[1])

    @property
    def file_path(self):
        return self.__file_path

    @file_path.setter
    def file_path(self, file_path):
        if not isinstance(file_path, str):
            raise ValueError("Invalid file path, must be of type str")
        self.__file_path = file_path
