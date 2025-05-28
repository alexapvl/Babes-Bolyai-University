package com.quiz.servlet;

import com.quiz.dao.*;
import com.quiz.model.*;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class QuizServlet extends HttpServlet {

  private QuizDAO quizDAO = new QuizDAO();
  private QuestionDAO questionDAO = new QuestionDAO();
  private AnswerOptionDAO answerOptionDAO = new AnswerOptionDAO();
  private QuizAttemptDAO quizAttemptDAO = new QuizAttemptDAO();
  private UserAnswerDAO userAnswerDAO = new UserAnswerDAO();

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Get quiz configuration from session
    HttpSession session = request.getSession();
    Integer quizId = (Integer) session.getAttribute("quizId");
    Integer questionsPerPage = (Integer) session.getAttribute("questionsPerPage");
    Integer totalQuestions = (Integer) session.getAttribute("totalQuestions");
    Integer currentPage = (Integer) session.getAttribute("currentPage");
    Integer attemptId = (Integer) session.getAttribute("attemptId");

    // Validate session attributes
    if (quizId == null || questionsPerPage == null || totalQuestions == null || currentPage == null) {
      response.sendRedirect(request.getContextPath() + "/dashboard");
      return;
    }

    // Calculate pagination
    int offset = (currentPage - 1) * questionsPerPage;
    int limit = Math.min(questionsPerPage, totalQuestions - offset);

    // If we've reached the end of the quiz
    if (limit <= 0) {
      response.sendRedirect(request.getContextPath() + "/result");
      return;
    }

    // Get quiz details
    Optional<Quiz> quizOpt = quizDAO.findById(quizId);

    if (quizOpt.isPresent()) {
      Quiz quiz = quizOpt.get();
      request.setAttribute("quiz", quiz);

      // Get questions for current page
      List<Question> questions = questionDAO.findByQuizIdWithPagination(quizId, offset, limit);

      // Load answer options for each question
      for (Question question : questions) {
        List<AnswerOption> options = answerOptionDAO.findByQuestionId(question.getId());
        question.setAnswerOptions(options);
      }

      request.setAttribute("questions", questions);

      // Calculate pagination information
      int totalPages = (int) Math.ceil((double) totalQuestions / questionsPerPage);
      request.setAttribute("currentPage", currentPage);
      request.setAttribute("totalPages", totalPages);

      // Forward to quiz page
      request.getRequestDispatcher("/WEB-INF/quiz.jsp").forward(request, response);
    } else {
      // Quiz not found
      response.sendRedirect(request.getContextPath() + "/dashboard");
    }
  }

  @Override
  protected void doPost(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Get quiz configuration from session
    HttpSession session = request.getSession();
    Integer quizId = (Integer) session.getAttribute("quizId");
    Integer questionsPerPage = (Integer) session.getAttribute("questionsPerPage");
    Integer totalQuestions = (Integer) session.getAttribute("totalQuestions");
    Integer currentPage = (Integer) session.getAttribute("currentPage");

    // Validate session attributes
    if (quizId == null || questionsPerPage == null || totalQuestions == null || currentPage == null) {
      response.sendRedirect(request.getContextPath() + "/dashboard");
      return;
    }

    // Get user from session
    User user = (User) session.getAttribute("user");

    // Create quiz attempt if not exists
    Integer attemptId = (Integer) session.getAttribute("attemptId");
    if (attemptId == null) {
      QuizAttempt attempt = new QuizAttempt(user.getId(), quizId, totalQuestions);
      if (quizAttemptDAO.create(attempt)) {
        attemptId = attempt.getId();
        session.setAttribute("attemptId", attemptId);
      } else {
        // Error creating attempt
        response.sendRedirect(request.getContextPath() + "/dashboard");
        return;
      }
    }

    // Calculate pagination
    int offset = (currentPage - 1) * questionsPerPage;
    int limit = Math.min(questionsPerPage, totalQuestions - offset);

    // Get questions for current page
    List<Question> questions = questionDAO.findByQuizIdWithPagination(quizId, offset, limit);

    // Process answers
    for (Question question : questions) {
      String answerParam = request.getParameter("question_" + question.getId());

      if (answerParam != null && !answerParam.trim().isEmpty()) {
        try {
          int answerOptionId = Integer.parseInt(answerParam);

          // Check if answer is correct
          Optional<AnswerOption> optionOpt = answerOptionDAO.findById(answerOptionId);

          if (optionOpt.isPresent()) {
            AnswerOption option = optionOpt.get();

            // Save user answer
            UserAnswer userAnswer = new UserAnswer(attemptId, question.getId(), answerOptionId, option.isCorrect());
            userAnswerDAO.create(userAnswer);
          }
        } catch (NumberFormatException e) {
          // Invalid answer ID, ignore
        }
      }
    }

    // Move to next page or finish quiz
    currentPage++;
    session.setAttribute("currentPage", currentPage);

    // Check if we've reached the end of the quiz
    int nextOffset = (currentPage - 1) * questionsPerPage;
    int nextLimit = Math.min(questionsPerPage, totalQuestions - nextOffset);

    if (nextLimit <= 0) {
      // Calculate score
      int correctAnswers = userAnswerDAO.countCorrectAnswersByAttemptId(attemptId);
      quizAttemptDAO.updateScore(attemptId, correctAnswers);

      // Redirect to results page
      response.sendRedirect(request.getContextPath() + "/result");
    } else {
      // Redirect to next page of questions
      response.sendRedirect(request.getContextPath() + "/quiz");
    }
  }
}