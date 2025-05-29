package com.quiz.servlet;

import com.quiz.dao.QuestionDAO;
import com.quiz.dao.QuizDAO;
import com.quiz.model.Quiz;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.util.Optional;

public class QuizConfigServlet extends HttpServlet {

  private QuizDAO quizDAO = new QuizDAO();
  private QuestionDAO questionDAO = new QuestionDAO();

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Get quiz ID from request
    String quizIdParam = request.getParameter("quizId");

    if (quizIdParam == null || quizIdParam.trim().isEmpty()) {
      response.sendRedirect(request.getContextPath() + "/dashboard");
      return;
    }

    try {
      int quizId = Integer.parseInt(quizIdParam);

      // Get quiz details
      Optional<Quiz> quizOpt = quizDAO.findById(quizId);

      if (quizOpt.isPresent()) {
        Quiz quiz = quizOpt.get();
        request.setAttribute("quiz", quiz);

        // Get total number of questions
        int totalQuestions = questionDAO.countQuestionsByQuizId(quizId);
        request.setAttribute("totalQuestions", totalQuestions);

        // Forward to quiz configuration page
        request.getRequestDispatcher("/WEB-INF/quiz-config.jsp").forward(request, response);
      } else {
        // Quiz not found
        response.sendRedirect(request.getContextPath() + "/dashboard");
      }

    } catch (NumberFormatException e) {
      response.sendRedirect(request.getContextPath() + "/dashboard");
    }
  }

  @Override
  protected void doPost(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Get parameters from request
    String quizIdParam = request.getParameter("quizId");
    String questionsPerPageParam = request.getParameter("questionsPerPage");
    String totalQuestionsParam = request.getParameter("totalQuestions");

    // Validate input
    if (quizIdParam == null || quizIdParam.trim().isEmpty() ||
        questionsPerPageParam == null || questionsPerPageParam.trim().isEmpty() ||
        totalQuestionsParam == null || totalQuestionsParam.trim().isEmpty()) {

      response.sendRedirect(request.getContextPath() + "/dashboard");
      return;
    }

    try {
      int quizId = Integer.parseInt(quizIdParam);
      int questionsPerPage = Integer.parseInt(questionsPerPageParam);
      int totalQuestions = Integer.parseInt(totalQuestionsParam);

      // Validate values
      int maxQuestions = questionDAO.countQuestionsByQuizId(quizId);

      if (questionsPerPage <= 0) {
        questionsPerPage = 1;
      }

      if (totalQuestions <= 0 || totalQuestions > maxQuestions) {
        totalQuestions = maxQuestions;
      }

      // Store quiz configuration in session
      HttpSession session = request.getSession();
      session.setAttribute("quizId", quizId);
      session.setAttribute("questionsPerPage", questionsPerPage);
      session.setAttribute("totalQuestions", totalQuestions);
      session.setAttribute("currentPage", 1);

      // Redirect to quiz page
      response.sendRedirect(request.getContextPath() + "/quiz");

    } catch (NumberFormatException e) {
      response.sendRedirect(request.getContextPath() + "/dashboard");
    }
  }
}