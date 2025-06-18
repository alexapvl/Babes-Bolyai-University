package com.quiz.dao;

import com.quiz.model.Project;
import com.quiz.util.DatabaseUtil;
import java.util.ArrayList;

import java.sql.*;
import java.util.Optional;

public class ProjectDAO {
  public ArrayList<Project> findAll() {
    String sql = "SELECT * FROM project";

    try (Connection conn = DatabaseUtil.getConnection();
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery(sql)) {

      ArrayList<Project> projects = new ArrayList<>();

      while (rs.next()) {
        try {
          Project project = new Project();
          project.setId(rs.getInt("id"));
          project.setProjectManagerId(rs.getInt("projectManagerId"));
          project.setName(rs.getString("name"));
          project.setDescription(rs.getString("description"));
          project.setMembers(rs.getString("members"));
          projects.add(project);
        } catch (SQLException e) {
          e.printStackTrace();
        }
      }

      return projects;

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return null;
  }

  public Optional<Project> findByName(String name) {
    String sql = "SELECT * FROM project WHERE name = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {
      stmt.setString(1, name);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        Project project = new Project();
        project.setId(rs.getInt("id"));
        project.setProjectManagerId(rs.getInt("projectManagerId"));
        project.setName(rs.getString("name"));
        project.setDescription(rs.getString("description"));
        project.setMembers(rs.getString("members"));
        return Optional.of(project);
      }
    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public ArrayList<Project> findAllProjectsMemberIsPartOf(String member) {
    String sql = "SELECT * FROM project WHERE members LIKE ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {
      stmt.setString(1, "%" + member + "%");
      ResultSet rs = stmt.executeQuery();

      ArrayList<Project> projects = new ArrayList<>();

      while (rs.next()) {
        try {
          Project project = new Project();
          project.setId(rs.getInt("id"));
          project.setProjectManagerId(rs.getInt("projectManagerId"));
          project.setName(rs.getString("name"));
          project.setDescription(rs.getString("description"));
          project.setMembers(rs.getString("members"));
          projects.add(project);
        } catch (SQLException e) {
          e.printStackTrace();
        }
      }

      return projects;

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return null;
  }

  public boolean create(Project project) {
    String sql = "INSERT INTO project (projectManagerId, name, description, members) VALUES (?, ?, ?, ?)";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {
      stmt.setInt(1, project.getProjectManagerId());
      stmt.setString(2, project.getName());
      stmt.setString(3, project.getDescription());
      stmt.setString(4, project.getMembers());
      stmt.executeUpdate();
      return true;
    } catch (SQLException e) {
      e.printStackTrace();
    }
    return false;
  }

  public boolean update(Project project) {
    String sql = "UPDATE project SET name = ?, description = ?, members = ? WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {
      stmt.setString(1, project.getName());
      stmt.setString(2, project.getDescription());
      stmt.setString(3, project.getMembers());
      stmt.setInt(4, project.getId());
      stmt.executeUpdate();
      return true;
    } catch (SQLException e) {
      e.printStackTrace();
    }
    return false;
  }
}
