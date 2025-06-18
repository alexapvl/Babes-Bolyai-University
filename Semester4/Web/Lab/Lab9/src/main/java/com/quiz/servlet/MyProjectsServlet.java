package com.quiz.servlet;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.ArrayList;
import com.quiz.dao.ProjectDAO;
import com.quiz.model.Project;

public class MyProjectsServlet extends HttpServlet {

  private ProjectDAO projectDAO = new ProjectDAO();

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    ArrayList<Project> projects = projectDAO.findAllProjectsMemberIsPartOf(request.getParameter("member"));
    request.setAttribute("myProjects", projects);

    request.getRequestDispatcher("/WEB-INF/my-projects.jsp").forward(request, response);
  }
}
