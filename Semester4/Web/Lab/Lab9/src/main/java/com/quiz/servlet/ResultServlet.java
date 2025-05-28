package com.quiz.servlet;

import com.quiz.dao.QuizAttemptDAO;
import com.quiz.dao.QuizDAO;
import com.quiz.dao.UserAnswerDAO;
import com.quiz.model.Quiz;
import com.quiz.model.QuizAttempt;
import com.quiz.model.User;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.time.format.DateTimeFormatter;
import java.util.Optional;

public class ResultServlet extends HttpServlet {

  private QuizDAO quizDAO = new QuizDAO();
  private QuizAttemptDAO quizAttemptDAO = new QuizAttemptDAO();
  private UserAnswerDAO userAnswerDAO = new UserAnswerDAO();

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Get quiz configuration from session
    HttpSession session = request.getSession();
    Integer quizId = (Integer) session.getAttribute("quizId");
    Integer attemptId = (Integer) session.getAttribute("attemptId");
    User user = (User) session.getAttribute("user");

    // Validate session attributes
    if (quizId == null || attemptId == null || user == null) {
      response.sendRedirect(request.getContextPath() + "/dashboard");
      return;
    }

    // Get quiz details
    Optional<Quiz> quizOpt = quizDAO.findById(quizId);
    Optional<QuizAttempt> attemptOpt = quizAttemptDAO.findById(attemptId);

    if (quizOpt.isPresent() && attemptOpt.isPresent()) {
      Quiz quiz = quizOpt.get();
      QuizAttempt attempt = attemptOpt.get();

      request.setAttribute("quiz", quiz);
      request.setAttribute("attempt", attempt);

      // Get correct and incorrect answers count
      int correctAnswers = userAnswerDAO.countCorrectAnswersByAttemptId(attemptId);
      int incorrectAnswers = userAnswerDAO.countIncorrectAnswersByAttemptId(attemptId);

      request.setAttribute("correctAnswers", correctAnswers);
      request.setAttribute("incorrectAnswers", incorrectAnswers);

      // Get user's best result for this quiz
      Optional<QuizAttempt> bestAttemptOpt = quizAttemptDAO.findBestAttemptByUserAndQuiz(user.getId(), quizId);

      if (bestAttemptOpt.isPresent()) {
        QuizAttempt bestAttempt = bestAttemptOpt.get();
        request.setAttribute("bestAttempt", bestAttempt);

        // Format the date for display
        if (bestAttempt.getCompletedAt() != null) {
          String formattedDate = bestAttempt.getCompletedAt().format(DateTimeFormatter.ofPattern("yyyy-MM-dd"));
          request.setAttribute("bestAttemptDate", formattedDate);
        }
      }

      // Clear quiz session attributes
      session.removeAttribute("quizId");
      session.removeAttribute("questionsPerPage");
      session.removeAttribute("totalQuestions");
      session.removeAttribute("currentPage");
      session.removeAttribute("attemptId");

      // Forward to results page
      request.getRequestDispatcher("/WEB-INF/result.jsp").forward(request, response);
    } else {
      // Quiz or attempt not found
      response.sendRedirect(request.getContextPath() + "/dashboard");
    }
  }
}