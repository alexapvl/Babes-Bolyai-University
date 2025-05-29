package com.quiz.dao;

import com.quiz.model.Quiz;
import com.quiz.util.DatabaseUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class QuizDAO {

  public List<Quiz> findAll() {
    List<Quiz> quizzes = new ArrayList<>();
    String sql = "SELECT * FROM quizzes ORDER BY created_at DESC";

    try (Connection conn = DatabaseUtil.getConnection();
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery(sql)) {

      while (rs.next()) {
        Quiz quiz = new Quiz();
        quiz.setId(rs.getInt("id"));
        quiz.setTitle(rs.getString("title"));
        quiz.setDescription(rs.getString("description"));
        quiz.setCreatedAt(rs.getTimestamp("created_at").toLocalDateTime());
        quizzes.add(quiz);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return quizzes;
  }

  public Optional<Quiz> findById(int id) {
    String sql = "SELECT * FROM quizzes WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, id);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        Quiz quiz = new Quiz();
        quiz.setId(rs.getInt("id"));
        quiz.setTitle(rs.getString("title"));
        quiz.setDescription(rs.getString("description"));
        quiz.setCreatedAt(rs.getTimestamp("created_at").toLocalDateTime());
        return Optional.of(quiz);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public boolean create(Quiz quiz) {
    String sql = "INSERT INTO quizzes (title, description) VALUES (?, ?)";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

      stmt.setString(1, quiz.getTitle());
      stmt.setString(2, quiz.getDescription());

      int affectedRows = stmt.executeUpdate();

      if (affectedRows > 0) {
        try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
          if (generatedKeys.next()) {
            quiz.setId(generatedKeys.getInt(1));
            return true;
          }
        }
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public boolean update(Quiz quiz) {
    String sql = "UPDATE quizzes SET title = ?, description = ? WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setString(1, quiz.getTitle());
      stmt.setString(2, quiz.getDescription());
      stmt.setInt(3, quiz.getId());

      int affectedRows = stmt.executeUpdate();
      return affectedRows > 0;

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public boolean delete(int id) {
    String sql = "DELETE FROM quizzes WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, id);
      int affectedRows = stmt.executeUpdate();
      return affectedRows > 0;

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }
}