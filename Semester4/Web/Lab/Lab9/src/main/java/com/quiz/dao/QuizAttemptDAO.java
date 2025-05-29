package com.quiz.dao;

import com.quiz.model.QuizAttempt;
import com.quiz.util.DatabaseUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class QuizAttemptDAO {

  public List<QuizAttempt> findByUserId(int userId) {
    List<QuizAttempt> attempts = new ArrayList<>();
    String sql = "SELECT * FROM quiz_attempts WHERE user_id = ? ORDER BY started_at DESC";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, userId);
      ResultSet rs = stmt.executeQuery();

      while (rs.next()) {
        QuizAttempt attempt = new QuizAttempt();
        attempt.setId(rs.getInt("id"));
        attempt.setUserId(rs.getInt("user_id"));
        attempt.setQuizId(rs.getInt("quiz_id"));
        attempt.setScore(rs.getInt("score"));
        attempt.setTotalQuestions(rs.getInt("total_questions"));
        attempt.setStartedAt(rs.getTimestamp("started_at").toLocalDateTime());

        Timestamp completedAt = rs.getTimestamp("completed_at");
        if (completedAt != null) {
          attempt.setCompletedAt(completedAt.toLocalDateTime());
        }

        attempts.add(attempt);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return attempts;
  }

  public List<QuizAttempt> findByQuizId(int quizId) {
    List<QuizAttempt> attempts = new ArrayList<>();
    String sql = "SELECT * FROM quiz_attempts WHERE quiz_id = ? ORDER BY started_at DESC";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, quizId);
      ResultSet rs = stmt.executeQuery();

      while (rs.next()) {
        QuizAttempt attempt = new QuizAttempt();
        attempt.setId(rs.getInt("id"));
        attempt.setUserId(rs.getInt("user_id"));
        attempt.setQuizId(rs.getInt("quiz_id"));
        attempt.setScore(rs.getInt("score"));
        attempt.setTotalQuestions(rs.getInt("total_questions"));
        attempt.setStartedAt(rs.getTimestamp("started_at").toLocalDateTime());

        Timestamp completedAt = rs.getTimestamp("completed_at");
        if (completedAt != null) {
          attempt.setCompletedAt(completedAt.toLocalDateTime());
        }

        attempts.add(attempt);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return attempts;
  }

  public Optional<QuizAttempt> findById(int id) {
    String sql = "SELECT * FROM quiz_attempts WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, id);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        QuizAttempt attempt = new QuizAttempt();
        attempt.setId(rs.getInt("id"));
        attempt.setUserId(rs.getInt("user_id"));
        attempt.setQuizId(rs.getInt("quiz_id"));
        attempt.setScore(rs.getInt("score"));
        attempt.setTotalQuestions(rs.getInt("total_questions"));
        attempt.setStartedAt(rs.getTimestamp("started_at").toLocalDateTime());

        Timestamp completedAt = rs.getTimestamp("completed_at");
        if (completedAt != null) {
          attempt.setCompletedAt(completedAt.toLocalDateTime());
        }

        return Optional.of(attempt);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public Optional<QuizAttempt> findBestAttemptByUserAndQuiz(int userId, int quizId) {
    String sql = "SELECT * FROM quiz_attempts WHERE user_id = ? AND quiz_id = ? AND completed_at IS NOT NULL ORDER BY score DESC LIMIT 1";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, userId);
      stmt.setInt(2, quizId);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        QuizAttempt attempt = new QuizAttempt();
        attempt.setId(rs.getInt("id"));
        attempt.setUserId(rs.getInt("user_id"));
        attempt.setQuizId(rs.getInt("quiz_id"));
        attempt.setScore(rs.getInt("score"));
        attempt.setTotalQuestions(rs.getInt("total_questions"));
        attempt.setStartedAt(rs.getTimestamp("started_at").toLocalDateTime());

        Timestamp completedAt = rs.getTimestamp("completed_at");
        if (completedAt != null) {
          attempt.setCompletedAt(completedAt.toLocalDateTime());
        }

        return Optional.of(attempt);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public boolean create(QuizAttempt attempt) {
    String sql = "INSERT INTO quiz_attempts (user_id, quiz_id, total_questions) VALUES (?, ?, ?)";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

      stmt.setInt(1, attempt.getUserId());
      stmt.setInt(2, attempt.getQuizId());
      stmt.setInt(3, attempt.getTotalQuestions());

      int affectedRows = stmt.executeUpdate();

      if (affectedRows > 0) {
        try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
          if (generatedKeys.next()) {
            attempt.setId(generatedKeys.getInt(1));
            return true;
          }
        }
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public boolean updateScore(int attemptId, int score) {
    String sql = "UPDATE quiz_attempts SET score = ?, completed_at = CURRENT_TIMESTAMP WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, score);
      stmt.setInt(2, attemptId);

      int affectedRows = stmt.executeUpdate();
      return affectedRows > 0;

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }
}