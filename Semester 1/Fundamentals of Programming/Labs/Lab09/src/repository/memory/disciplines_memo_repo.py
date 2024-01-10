from src.domain.discipline import Discipline

class DisciplinesMemoRepo:
    def __init__(self):
        self.disciplines = []

    def add_discipline(self, discipline: Discipline):
        """
        Adds a discipline to the list of disciplines
        :param discipline: The discipline to be added
        """
        if discipline in self.disciplines:
            raise ValueError("Discipline already exists")
        self.disciplines.append(discipline)

    def remove_discipline(self, discipline: Discipline):
        """
        Removes a discipline from the list of disciplines by ID(__eq__ method)
        :param discipline: Discipline to be removed
        """
        if discipline not in self.disciplines:
            raise ValueError("Discipline doesn't exist")
        self.disciplines.remove(discipline)

    def search_discipline_by_id(self, discipline_id: str):
        """
        Searches for a discipline by ID
        :param discipline_id: The ID of the discipline
        :return: The discipline with the given ID, if it exists
        """
        if not Discipline.validate_id(discipline_id):
            raise ValueError("Invalid ID, must contain only digits")
        for discipline in self.disciplines:
            if discipline.id == discipline_id:
                return discipline
        raise ValueError("Discipline with given ID doesn't exist")

    def search_discipline_by_name(self, name: str):
        """
        Searches for a discipline by name
        :param name: The name of the discipline
        :return: The discipline with the given name, if it exists
        """
        matching_disciplines = [discipline for discipline in self.disciplines if name.lower() in discipline.name.lower()]
        if len(matching_disciplines) == 0:
            raise ValueError("Discipline with given name doesn't exist")
        return matching_disciplines

    def update_discipline(self, discipline: Discipline, new_discipline: Discipline):
        """
        Updates a discipline
        :param discipline: Discipline to be updated
        :param new_discipline: New discipline
        """
        if discipline not in self.disciplines:
            raise ValueError("Discipline doesn't exist")
        for disc in self.disciplines:
            if disc == discipline:
                disc.update_discipline(new_discipline)
                break

    @property
    def disciplines(self):
        return self.__disciplines

    @disciplines.setter
    def disciplines(self, disciplines: list):
        self.__disciplines = disciplines

    @classmethod
    def generate_20_random_disciplines(cls):
        """
        Generates 20 random disciplines
        :return: A list of 20 random disciplines
        """
        disciplines_generated_list = []
        while len(disciplines_generated_list) < 20:
            discipline = Discipline.random_discipline()
            if discipline not in disciplines_generated_list:
                disciplines_generated_list.append(discipline)
        return disciplines_generated_list
