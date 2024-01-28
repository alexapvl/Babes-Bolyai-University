class Question:
    def __init__(self, id: str, text: str, ans1: str, ans2: str, ans3: str, correct_ans: str, difficulty: str):
        self.id = id
        self.text = text
        self.answers = [ans1, ans2, ans3]
        self.correct_ans = correct_ans
        self.difficulty = difficulty

    def __str__(self):
        ans1 = self.answers[0]
        ans2 = self.answers[1]
        ans3 = self.answers[2]
        return str(self.id) + ";" + self.text + ";" + ans1 + ";" + ans2 + ";" + ans3 + ";" + self.correct_ans + ";" + self.difficulty

    @property
    def id(self):
        return self.__id

    @id.setter
    def id(self, id: str):
        self.__id = id

    @property
    def text(self):
        return self.__text

    @text.setter
    def text(self, text: str):
        self.__text = text

    @property
    def answers(self):
        return self.__answers

    @answers.setter
    def answers(self, answers: list):
        self.__answers = answers

    @property
    def difficulty(self):
        return self.__difficulty

    @difficulty.setter
    def difficulty(self, diff: str):
        self.__difficulty = diff
