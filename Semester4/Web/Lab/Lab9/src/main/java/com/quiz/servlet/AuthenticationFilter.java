package com.quiz.servlet;

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

public class AuthenticationFilter implements Filter {

  @Override
  public void init(FilterConfig filterConfig) throws ServletException {
    // Initialization code if needed
  }

  @Override
  public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
      throws IOException, ServletException {

    HttpServletRequest httpRequest = (HttpServletRequest) request;
    HttpServletResponse httpResponse = (HttpServletResponse) response;
    HttpSession session = httpRequest.getSession(false);

    String requestURI = httpRequest.getRequestURI();
    String contextPath = httpRequest.getContextPath();

    // Allow access to login and register pages without authentication
    if (requestURI.equals(contextPath + "/login") ||
        requestURI.equals(contextPath + "/register") ||
        requestURI.equals(contextPath + "/login.jsp") ||
        requestURI.equals(contextPath + "/register.jsp")) {
      chain.doFilter(request, response);
      return;
    }

    boolean isLoggedIn = (session != null && session.getAttribute("user") != null);

    if (isLoggedIn) {
      // User is logged in, continue with the request
      chain.doFilter(request, response);
    } else {
      // User is not logged in, redirect to login page
      httpResponse.sendRedirect(httpRequest.getContextPath() + "/login.jsp");
    }
  }

  @Override
  public void destroy() {
    // Cleanup code if needed
  }
}