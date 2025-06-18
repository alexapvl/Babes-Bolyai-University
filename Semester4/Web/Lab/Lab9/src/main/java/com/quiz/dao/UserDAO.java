package com.quiz.dao;

import com.quiz.model.User;
import com.quiz.util.DatabaseUtil;

import java.sql.*;
import java.util.Optional;

public class UserDAO {

  public Optional<User> findByUsername(String username) {
    String sql = "SELECT * FROM users WHERE username = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setString(1, username);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        User user = new User();
        user.setId(rs.getInt("id"));
        user.setUsername(rs.getString("username"));
        user.setCreatedAt(rs.getTimestamp("created_at").toLocalDateTime());
        return Optional.of(user);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public Optional<User> findById(int id) {
    String sql = "SELECT * FROM users WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, id);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        User user = new User();
        user.setId(rs.getInt("id"));
        user.setUsername(rs.getString("username"));
        user.setCreatedAt(rs.getTimestamp("created_at").toLocalDateTime());
        return Optional.of(user);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public boolean create(User user) {
    String sql = "INSERT INTO users (username) VALUES (?)";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

      stmt.setString(1, user.getUsername());

      int affectedRows = stmt.executeUpdate();

      if (affectedRows > 0) {
        try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
          if (generatedKeys.next()) {
            user.setId(generatedKeys.getInt(1));
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
    Optional<User> userOpt = findByUsername(username);

    if (userOpt.isPresent()) {
      return true;
    } else {
      User user = new User();
      user.setUsername(username);
      create(user);
      return true;
    }

    // return false;
  }

  // public String hashPassword(String plainPassword) {
  // return BCrypt.hashpw(plainPassword, BCrypt.gensalt(12));
  // }
}