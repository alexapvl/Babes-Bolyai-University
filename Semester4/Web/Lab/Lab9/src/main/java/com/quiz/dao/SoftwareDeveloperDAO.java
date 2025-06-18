package com.quiz.dao;

import com.quiz.model.SoftwareDeveloper;
import com.quiz.util.DatabaseUtil;
import java.util.ArrayList;
import java.sql.*;
import java.util.Optional;

public class SoftwareDeveloperDAO {

  public ArrayList<SoftwareDeveloper> findAll() {
    String sql = "SELECT * FROM softwareDeveloper";

    try (Connection conn = DatabaseUtil.getConnection();
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery(sql)) {

      ArrayList<SoftwareDeveloper> softwareDevelopers = new ArrayList<>();

      while (rs.next()) {
        SoftwareDeveloper softwareDeveloper = new SoftwareDeveloper();
        softwareDeveloper.setId(rs.getInt("id"));
        softwareDeveloper.setName(rs.getString("name"));
        softwareDeveloper.setSkills(rs.getString("skills"));
        softwareDevelopers.add(softwareDeveloper);
      }

      return softwareDevelopers;

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return null;
  }

  public Optional<SoftwareDeveloper> findByUsername(String username) {
    String sql = "SELECT * FROM softwareDeveloper WHERE name = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setString(1, username);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        SoftwareDeveloper softwareDeveloper = new SoftwareDeveloper();
        softwareDeveloper.setId(rs.getInt("id"));
        softwareDeveloper.setName(rs.getString("name"));
        softwareDeveloper.setSkills(rs.getString("skills"));
        return Optional.of(softwareDeveloper);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public Optional<SoftwareDeveloper> findById(int id) {
    String sql = "SELECT * FROM softwareDeveloper WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, id);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        SoftwareDeveloper softwareDeveloper = new SoftwareDeveloper();
        softwareDeveloper.setId(rs.getInt("id"));
        softwareDeveloper.setName(rs.getString("name"));
        softwareDeveloper.setSkills(rs.getString("skills"));
        return Optional.of(softwareDeveloper);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public boolean create(SoftwareDeveloper softwareDeveloper) {
    String sql = "INSERT INTO softwareDeveloper (name, skills) VALUES (?, ?)";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

      stmt.setString(1, softwareDeveloper.getName());
      stmt.setString(2, softwareDeveloper.getSkills());

      int affectedRows = stmt.executeUpdate();

      if (affectedRows > 0) {
        try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
          if (generatedKeys.next()) {
            softwareDeveloper.setId(generatedKeys.getInt(1));
            return true;
          }
        }
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public boolean authenticate(String username) {
    Optional<SoftwareDeveloper> softwareDeveloperOpt = findByUsername(username);

    if (softwareDeveloperOpt.isPresent()) {
      return true;
    } else {
      SoftwareDeveloper softwareDeveloper = new SoftwareDeveloper();
      softwareDeveloper.setName(username);
      create(softwareDeveloper);
      return true;
    }

    // return false;
  }
}
