package com.quiz.servlet;

import com.quiz.dao.UserDAO;
import com.quiz.model.User;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.util.Optional;

public class LoginServlet extends HttpServlet {

  private UserDAO userDAO = new UserDAO();

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Check if user is already logged in
    HttpSession session = request.getSession(false);
    if (session != null && session.getAttribute("user") != null) {
      // User is already logged in, redirect to dashboard
      response.sendRedirect(request.getContextPath() + "/dashboard");
      return;
    }

    // Forward to login page
    request.getRequestDispatcher("/login.jsp").forward(request, response);
  }

  @Override
  protected void doPost(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    String username = request.getParameter("username");
    String password = request.getParameter("password");

    // Validate input
    if (username == null || username.trim().isEmpty() ||
        password == null || password.trim().isEmpty()) {

      request.setAttribute("error", "Username and password are required");
      request.getRequestDispatcher("/login.jsp").forward(request, response);
      return;
    }

    // Authenticate user
    if (userDAO.authenticate(username, password)) {
      // Get user details
      Optional<User> userOpt = userDAO.findByUsername(username);

      if (userOpt.isPresent()) {
        User user = userOpt.get();

        // Create session and add user
        HttpSession session = request.getSession(true);
        session.setAttribute("user", user);

        // Redirect to dashboard
        response.sendRedirect(request.getContextPath() + "/dashboard");
      } else {
        // This should not happen if authentication succeeded
        request.setAttribute("error", "An error occurred. Please try again.");
        request.getRequestDispatcher("/login.jsp").forward(request, response);
      }
    } else {
      // Authentication failed
      request.setAttribute("error", "Invalid username or password");
      request.getRequestDispatcher("/login.jsp").forward(request, response);
    }
  }
}