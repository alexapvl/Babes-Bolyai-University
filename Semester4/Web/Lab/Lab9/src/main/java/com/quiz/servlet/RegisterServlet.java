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
import java.util.regex.Pattern;

public class RegisterServlet extends HttpServlet {

  private UserDAO userDAO = new UserDAO();
  private static final Pattern USERNAME_PATTERN = Pattern.compile("^[a-zA-Z0-9_]+$");

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

    // Forward to register page
    request.getRequestDispatcher("/register.jsp").forward(request, response);
  }

  @Override
  protected void doPost(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    String username = request.getParameter("username");
    String password = request.getParameter("password");
    String confirmPassword = request.getParameter("confirmPassword");

    // Validate input
    if (username == null || username.trim().isEmpty()) {
      request.setAttribute("error", "Username is required");
      request.getRequestDispatcher("/register.jsp").forward(request, response);
      return;
    }

    // Trim username
    username = username.trim();

    if (password == null || password.trim().isEmpty()) {
      request.setAttribute("error", "Password is required");
      request.getRequestDispatcher("/register.jsp").forward(request, response);
      return;
    }

    if (confirmPassword == null || !password.equals(confirmPassword)) {
      request.setAttribute("error", "Passwords do not match");
      request.getRequestDispatcher("/register.jsp").forward(request, response);
      return;
    }

    // Validate username length and format
    if (username.length() < 3 || username.length() > 50) {
      request.setAttribute("error", "Username must be between 3 and 50 characters");
      request.getRequestDispatcher("/register.jsp").forward(request, response);
      return;
    }

    // Validate username format (only alphanumeric and underscore)
    if (!USERNAME_PATTERN.matcher(username).matches()) {
      request.setAttribute("error", "Username can only contain letters, numbers, and underscores");
      request.getRequestDispatcher("/register.jsp").forward(request, response);
      return;
    }

    // Validate password strength
    if (password.length() < 6) {
      request.setAttribute("error", "Password must be at least 6 characters long");
      request.getRequestDispatcher("/register.jsp").forward(request, response);
      return;
    }

    // Check if username already exists
    Optional<User> existingUser = userDAO.findByUsername(username);
    if (existingUser.isPresent()) {
      request.setAttribute("error", "Username already exists. Please choose a different username.");
      request.getRequestDispatcher("/register.jsp").forward(request, response);
      return;
    }

    // Create new user
    String hashedPassword = userDAO.hashPassword(password);
    User newUser = new User(username, hashedPassword);

    if (userDAO.create(newUser)) {
      // Registration successful
      request.setAttribute("success", "Registration successful! You can now log in with your credentials.");
      request.getRequestDispatcher("/login.jsp").forward(request, response);
    } else {
      // Registration failed
      request.setAttribute("error", "Registration failed. Please try again later.");
      request.getRequestDispatcher("/register.jsp").forward(request, response);
    }
  }
}

