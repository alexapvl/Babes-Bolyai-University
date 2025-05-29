package com.quiz.dao;

import com.quiz.model.UserAnswer;
import com.quiz.util.DatabaseUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class UserAnswerDAO {

  public List<UserAnswer> findByAttemptId(int attemptId) {
    List<UserAnswer> answers = new ArrayList<>();
    String sql = "SELECT * FROM user_answers WHERE attempt_id = ? ORDER BY answered_at ASC";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, attemptId);
      ResultSet rs = stmt.executeQuery();

      while (rs.next()) {
        UserAnswer answer = new UserAnswer();
        answer.setId(rs.getInt("id"));
        answer.setAttemptId(rs.getInt("attempt_id"));
        answer.setQuestionId(rs.getInt("question_id"));
        answer.setAnswerOptionId(rs.getInt("answer_option_id"));
        answer.setCorrect(rs.getBoolean("is_correct"));
        answer.setAnsweredAt(rs.getTimestamp("answered_at").toLocalDateTime());
        answers.add(answer);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return answers;
  }

  public Optional<UserAnswer> findByAttemptAndQuestion(int attemptId, int questionId) {
    String sql = "SELECT * FROM user_answers WHERE attempt_id = ? AND question_id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, attemptId);
      stmt.setInt(2, questionId);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        UserAnswer answer = new UserAnswer();
        answer.setId(rs.getInt("id"));
        answer.setAttemptId(rs.getInt("attempt_id"));
        answer.setQuestionId(rs.getInt("question_id"));
        answer.setAnswerOptionId(rs.getInt("answer_option_id"));
        answer.setCorrect(rs.getBoolean("is_correct"));
        answer.setAnsweredAt(rs.getTimestamp("answered_at").toLocalDateTime());
        return Optional.of(answer);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public boolean create(UserAnswer answer) {
    String sql = "INSERT INTO user_answers (attempt_id, question_id, answer_option_id, is_correct) VALUES (?, ?, ?, ?)";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

      stmt.setInt(1, answer.getAttemptId());
      stmt.setInt(2, answer.getQuestionId());
      stmt.setInt(3, answer.getAnswerOptionId());
      stmt.setBoolean(4, answer.isCorrect());

      int affectedRows = stmt.executeUpdate();

      if (affectedRows > 0) {
        try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
          if (generatedKeys.next()) {
            answer.setId(generatedKeys.getInt(1));
            return true;
          }
        }
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public int countCorrectAnswersByAttemptId(int attemptId) {
    String sql = "SELECT COUNT(*) FROM user_answers WHERE attempt_id = ? AND is_correct = TRUE";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, attemptId);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        return rs.getInt(1);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return 0;
  }

  public int countIncorrectAnswersByAttemptId(int attemptId) {
    String sql = "SELECT COUNT(*) FROM user_answers WHERE attempt_id = ? AND is_correct = FALSE";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, attemptId);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        return rs.getInt(1);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return 0;
  }
}