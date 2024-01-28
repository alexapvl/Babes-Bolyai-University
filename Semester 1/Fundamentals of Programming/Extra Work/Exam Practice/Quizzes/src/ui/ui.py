from Quizzes.src.domain.question import (Question)
from Quizzes.src.service.controller import Service, RepoError


class UIError(ValueError):
    pass

class UI:
    def __init__(self, service: Service):
        self.__service = service

    @staticmethod
    def print_question(question: Question):
        print(question.text, "?")
        print(question.answers[0])
        print(question.answers[1])
        print(question.answers[2])

    @staticmethod
    def point(question: Question):
        if question.difficulty == "easy":
            return 1
        elif question.difficulty == "medium":
            return 2
        else:
            return 3

    def start(self):
        Service.generate_100_questions()
        while True:
            try:
                command = input(">>>")
                params = command.split()
                if params[0] == "add":
                    # adds a question to the master questions file
                    q_params = params[1].split(";")
                    question = Question(q_params[0], q_params[1], q_params[2], q_params[3], q_params[4], q_params[5], q_params[6])
                    self.__service.add_question(question)
                elif params[0] == "create":
                    # creates a quiz
                    self.__service.create_quiz(params[1], int(params[2]), params[3])
                elif params[0] == "start":
                    grade = 0
                    total_possible_points = 0
                    file = self.__service.start_quiz(params[1])
                    questions = file.readlines()
                    for question in questions:
                        params = question.split(";")
                        q = Question(params[0], params[1], params[2], params[3], params[4], params[5], Service.fix_quiz(params[6]))
                        UI.print_question(q)
                        answer = input("Answer: ")
                        point = self.point(q)
                        total_possible_points += point
                        if answer == q.correct_ans:
                            grade += point
                    print(f"Score: {total_possible_points}/{grade} ({float((grade/total_possible_points) * 100)}%)")
                    file.close()
                elif params[0] == "exit":
                    print("Bye!")
                    break
                else:
                    raise UIError("Invalid command")
            except UIError as uie:
                print(uie)
            except RepoError as re:
                print(re)

if __name__ == '__main__':
    service = Service()
    ui = UI(service)
    ui.start()
