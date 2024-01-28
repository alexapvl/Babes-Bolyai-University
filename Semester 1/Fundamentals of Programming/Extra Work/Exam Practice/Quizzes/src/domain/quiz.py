
class Quiz:
    # a quiz has difficulty, no. of questions, source file
    def __init__(self, difficulty: str, no_q: int, file: str, questions: list):
        self.difficulty = difficulty
        self.no_q = no_q
        self.file = file
        self.questions = questions

    @property
    def difficulty(self):
        return self.__difficulty

    @difficulty.setter
    def difficulty(self, difficulty: str):
        self.__difficulty = difficulty

    @property
    def no_q(self):
        return self.__no_q

    @no_q.setter
    def no_q(self, number: int):
        self.__no_q = number

    @property
    def file(self):
        return self.__file

    @file.setter
    def file(self, file: str):
        self.__file = file

    @property
    def questions(self):
        return self.__questions

    @questions.setter
    def questions(self, questions: list):
        self.__questions = questions
