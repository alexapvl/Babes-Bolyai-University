package com.quiz.servlet;

import com.quiz.dao.QuizAttemptDAO;
import com.quiz.dao.QuizDAO;
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
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DashboardServlet extends HttpServlet {

  private QuizDAO quizDAO = new QuizDAO();
  private QuizAttemptDAO quizAttemptDAO = new QuizAttemptDAO();

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Get user from session
    HttpSession session = request.getSession(false);
    User user = (User) session.getAttribute("user");

    // Get all available quizzes
    List<Quiz> quizzes = quizDAO.findAll();
    request.setAttribute("quizzes", quizzes);

    // Get user's quiz attempts
    List<QuizAttempt> attempts = quizAttemptDAO.findByUserId(user.getId());
    request.setAttribute("attempts", attempts);

    // Format dates for display in JSP
    Map<Integer, String> formattedDates = new HashMap<>();
    DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");

    for (QuizAttempt attempt : attempts) {
      if (attempt.getStartedAt() != null) {
        String formattedDate = attempt.getStartedAt().format(formatter);
        formattedDates.put(attempt.getId(), formattedDate);
      }
    }

    request.setAttribute("formattedDates", formattedDates);

    // Forward to dashboard page
    request.getRequestDispatcher("/WEB-INF/dashboard.jsp").forward(request, response);
  }
}