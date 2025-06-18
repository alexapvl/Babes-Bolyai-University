package com.quiz.servlet;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Optional;
import com.quiz.dao.ProjectDAO;
import com.quiz.dao.SoftwareDeveloperDAO;
import com.quiz.model.Project;
import com.quiz.model.SoftwareDeveloper;

public class ManageProjectsServlet extends HttpServlet {
  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {
    request.getRequestDispatcher("/WEB-INF/manage-projects.jsp").forward(request, response);
  }

  private ProjectDAO projectDAO = new ProjectDAO();
  private SoftwareDeveloperDAO softwareDeveloperDAO = new SoftwareDeveloperDAO();

  @Override
  protected void doPost(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {
    // will receive a developer name and a list of project names (string with comma
    // separated values)
    String developerName = request.getParameter("developerName");
    String projectNames = request.getParameter("projectNames");

    // check if the developer exists, if not, ignore the request
    Optional<SoftwareDeveloper> developer = softwareDeveloperDAO.findByUsername(developerName);
    if (developer.isEmpty()) {
      request.setAttribute("error", "Developer not found");
      request.getRequestDispatcher("/WEB-INF/manage-projects.jsp").forward(request, response);
      return;
    }

    // split the project names into an array
    String[] projectNamesArray = projectNames.split(",");

    // for loop through all the projects and check
    // 1. if projects exists, if not create it
    // 2. if the developer is a member of the project, if not add it
    // 3. if the developer is not a member of the project, add it to the project
    // members string and update the db

    for (String projectName : projectNamesArray) {
      // check if the project exists, if not, create it
      Optional<Project> project = projectDAO.findByName(projectName);
      if (project.isEmpty()) {
        Project newProject = new Project(developer.get().getId(), projectName, "", "");
        projectDAO.create(newProject);
      }

      project = projectDAO.findByName(projectName);

      // check if the developer is a member of the project, if not add it
      if (!project.get().getMembers().contains(developer.get().getName())) {
        if (project.get().getMembers().isEmpty()) {
          project.get().setMembers(developer.get().getName());
        } else {
          project.get().setMembers(project.get().getMembers() + "," + developer.get().getName());
        }
        projectDAO.update(project.get());
      }
    }

    request.setAttribute("success", "Projects updated successfully");
    request.getRequestDispatcher("/WEB-INF/manage-projects.jsp").forward(request, response);
  }

}
