package com.quiz.servlet;

import com.quiz.dao.SoftwareDeveloperDAO;
import com.quiz.model.SoftwareDeveloper;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.util.Optional;

public class LoginServlet extends HttpServlet {

  private SoftwareDeveloperDAO softwareDeveloperDAO = new SoftwareDeveloperDAO();

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Check if user is already logged in
    HttpSession session = request.getSession(false);
    if (session != null && session.getAttribute("user") != null) {
      // User is already logged in, redirect to dashboard
      response.sendRedirect(request.getContextPath() + "/projects");
      return;
    }

    // Forward to login page
    request.getRequestDispatcher("/login.jsp").forward(request, response);
  }

  @Override
  protected void doPost(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    String username = request.getParameter("username");

    // Validate input
    if (username == null || username.trim().isEmpty()) {

      request.setAttribute("error", "Username is required");
      request.getRequestDispatcher("/login.jsp").forward(request, response);
      return;
    }

    // Authenticate user
    if (softwareDeveloperDAO.authenticate(username)) {
      // Get user details
      Optional<SoftwareDeveloper> softwareDeveloperOpt = softwareDeveloperDAO.findByUsername(username);

      if (softwareDeveloperOpt.isPresent()) {
        SoftwareDeveloper softwareDeveloper = softwareDeveloperOpt.get();

        // Create session and add user
        HttpSession session = request.getSession(true);
        session.setAttribute("user", softwareDeveloper);

        // Redirect to dashboard
        response.sendRedirect(request.getContextPath() + "/projects");
      } else {
        // This should not happen if authentication succeeded
        request.setAttribute("error", "An error occurred. Please try again.");
        request.getRequestDispatcher("/login.jsp").forward(request, response);
      }
    } else {
      // Authentication failed
      request.setAttribute("error", "Invalid username");
      request.getRequestDispatcher("/login.jsp").forward(request, response);
    }
  }
}