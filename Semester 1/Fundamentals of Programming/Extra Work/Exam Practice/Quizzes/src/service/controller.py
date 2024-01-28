from Quizzes.src.domain.question import Question
from Quizzes.src.domain.quiz import Quiz
from random import choice, shuffle, randint


class RepoError(ValueError):
    pass

class Service:
    def __init__(self):
        pass

    @staticmethod
    def random_difficulty() -> str:
        """
        :return: a random difficulty under the form of a string
        """
        random = ["easy", "medium", "hard"]
        return choice(random)

    @staticmethod
    def load_master_quizzes() -> tuple:
        """
        Open a file and read all questions, create the question list
        :return: a tuple of lists consisting of texts, answers, and difficulties
        """
        questions = []
        with open("../repo/master_question_list.txt", "r") as f:
            lines = f.readlines()
            for line in lines:
                params = line.split(";")
                params[6] = Service.fix_quiz(params[6])
                question = Question(params[0], params[1], params[2], params[3], params[4], params[5], params[6])
                questions.append(question)
            f.close()
        texts = []
        answers = []
        for q in questions:
            texts.append(q.text)
            answers.append(q.answers)
        return texts, answers

    @staticmethod
    def generate_100_questions():
        """
        Generates 100 pseudo-random questions from the master question file and overwrites the contents of itself
        :return: the list of questions
        """
        questions = []
        texts, answers = Service.load_master_quizzes()
        for id in range(1, 101, 1):
            text = choice(texts)
            answer = choice(answers)
            correct_answer = choice(answer)
            difficulty = Service.random_difficulty()
            questions.append(Question(str(id), text, answer[0], answer[1], answer[2], correct_answer, difficulty))
        with open("../repo/master_question_list.txt", "w") as f:
            for q in questions:
                f.write(str(q))
                f.write("\n")  # append new line
        f.close()

    @staticmethod
    def add_question(question: Question):
        """
        adds a question to the master question list
        """
        with open("../repo/master_question_list.txt", "a") as f:
            f.write(str(question))
            f.write("\n")  # append new line
        f.close()

    @staticmethod
    def fix_quiz(string: str):
        if "\n" in string:
            return string[:-1]
        else:
            return string

    @staticmethod
    def create_quiz(diff: str, number_of_questions: int, file: str):
        """
        Creates a quiz with the given difficulty and number of questions and creates a quiz with the given file name
        First, it takes the minimum number of questions of the desired difficulty and then the rest of the questions, at random difficulty
        Then, it creates the quiz and writes it in the file
        :param number_of_questions: Number of questions in the string
        :param diff: Difficulty of the quiz
        :param file: File name to be created
        """
        difficulties = {"easy": [], "medium": [], "hard": []}
        with open("../repo/master_question_list.txt", "r") as master:
            lines = master.readlines()
            for line in lines:
                params = line.split(";")
                params[6] = Service.fix_quiz(params[6])
                question = Question(params[0], params[1], params[2], params[3], params[4], params[5], params[6])
                difficulties[params[6]].append(question)
        master.close()

        if len(difficulties[diff]) < (number_of_questions // 2) + 1:
            raise RepoError(f"Not enough {diff} questions in master file")

        questions = []

        shuffle(difficulties[diff])  # randomise order
        for _ in range((number_of_questions // 2) + 1):
            q = difficulties[diff].pop()
            questions.append(q)

        while len(questions) < number_of_questions:
            random_number = randint(0, 2)
            if random_number == 0:
                # choose an easy question
                random_easy_question = choice(difficulties["easy"])
                questions.append(random_easy_question)
                difficulties["easy"].remove(random_easy_question)
            elif random_number == 1:
                # choose a medium question
                random_medium_question = choice(difficulties["medium"])
                questions.append(random_medium_question)
                difficulties["medium"].remove(random_medium_question)
            else:
                # choose a hard question
                random_hard_question = choice(difficulties["hard"])
                questions.append(random_hard_question)
                difficulties["hard"].remove(random_hard_question)

        quiz = Quiz(diff, number_of_questions, file, questions)
        with open(f"../{file}", "w") as f:
            for question in quiz.questions:
                f.write(str(question))
                f.write("\n")
        f.close()

    @staticmethod
    def start_quiz(file: str):
        """
        Starts the quiz
        :param file: File name
        :return: The file descriptor
        """
        f = open(f"../{file}", "r")
        return f

    @staticmethod
    def write_quiz_into_file(file: str, quiz: Quiz):
        with open(file, "w") as f:
            for question in quiz.questions:
                f.write(str(question))
                f.write("\n")
        f.close()

if __name__ == '__main__':
    service = Service()
    service.create_quiz("easy", 6, "test.txt")