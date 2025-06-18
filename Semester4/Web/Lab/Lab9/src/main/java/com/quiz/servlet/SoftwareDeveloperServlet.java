package com.quiz.servlet;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.ArrayList;
import com.quiz.dao.SoftwareDeveloperDAO;
import com.quiz.model.SoftwareDeveloper;

public class SoftwareDeveloperServlet extends HttpServlet {
  private SoftwareDeveloperDAO softwareDeveloperDAO = new SoftwareDeveloperDAO();

  @Override
  protected void doGet(HttpServletRequest request, HttpServletResponse response)
      throws ServletException, IOException {

    ArrayList<SoftwareDeveloper> softwareDevelopers = softwareDeveloperDAO.findAll();
    request.setAttribute("softwareDevelopers", softwareDevelopers);

    request.getRequestDispatcher("/WEB-INF/software-developers.jsp").forward(request, response);
  }
}
