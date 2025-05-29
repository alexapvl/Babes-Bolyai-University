package com.quiz.servlet;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

public class LogoutServlet extends HttpServlet {

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    // Get the current session
    HttpSession session = request.getSession(false);

    if (session != null) {
      // Invalidate the session
      session.invalidate();
    }

    // Redirect to login page
    response.sendRedirect(request.getContextPath() + "/login.jsp");
  }
}