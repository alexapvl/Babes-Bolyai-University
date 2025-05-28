package com.quiz.dao;

import com.quiz.model.Question;
import com.quiz.util.DatabaseUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class QuestionDAO {

  public List<Question> findByQuizId(int quizId) {
    List<Question> questions = new ArrayList<>();
    String sql = "SELECT * FROM questions WHERE quiz_id = ? ORDER BY question_order ASC";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, quizId);
      ResultSet rs = stmt.executeQuery();

      while (rs.next()) {
        Question question = new Question();
        question.setId(rs.getInt("id"));
        question.setQuizId(rs.getInt("quiz_id"));
        question.setQuestionText(rs.getString("question_text"));
        question.setQuestionOrder(rs.getInt("question_order"));
        question.setCreatedAt(rs.getTimestamp("created_at").toLocalDateTime());
        questions.add(question);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return questions;
  }

  public List<Question> findByQuizIdWithPagination(int quizId, int offset, int limit) {
    List<Question> questions = new ArrayList<>();
    String sql = "SELECT * FROM questions WHERE quiz_id = ? ORDER BY question_order ASC LIMIT ? OFFSET ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, quizId);
      stmt.setInt(2, limit);
      stmt.setInt(3, offset);
      ResultSet rs = stmt.executeQuery();

      while (rs.next()) {
        Question question = new Question();
        question.setId(rs.getInt("id"));
        question.setQuizId(rs.getInt("quiz_id"));
        question.setQuestionText(rs.getString("question_text"));
        question.setQuestionOrder(rs.getInt("question_order"));
        question.setCreatedAt(rs.getTimestamp("created_at").toLocalDateTime());
        questions.add(question);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return questions;
  }

  public int countQuestionsByQuizId(int quizId) {
    String sql = "SELECT COUNT(*) FROM questions WHERE quiz_id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, quizId);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        return rs.getInt(1);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return 0;
  }

  public Optional<Question> findById(int id) {
    String sql = "SELECT * FROM questions WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, id);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        Question question = new Question();
        question.setId(rs.getInt("id"));
        question.setQuizId(rs.getInt("quiz_id"));
        question.setQuestionText(rs.getString("question_text"));
        question.setQuestionOrder(rs.getInt("question_order"));
        question.setCreatedAt(rs.getTimestamp("created_at").toLocalDateTime());
        return Optional.of(question);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public boolean create(Question question) {
    String sql = "INSERT INTO questions (quiz_id, question_text, question_order) VALUES (?, ?, ?)";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

      stmt.setInt(1, question.getQuizId());
      stmt.setString(2, question.getQuestionText());
      stmt.setInt(3, question.getQuestionOrder());

      int affectedRows = stmt.executeUpdate();

      if (affectedRows > 0) {
        try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
          if (generatedKeys.next()) {
            question.setId(generatedKeys.getInt(1));
            return true;
          }
        }
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public boolean update(Question question) {
    String sql = "UPDATE questions SET quiz_id = ?, question_text = ?, question_order = ? WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, question.getQuizId());
      stmt.setString(2, question.getQuestionText());
      stmt.setInt(3, question.getQuestionOrder());
      stmt.setInt(4, question.getId());

      int affectedRows = stmt.executeUpdate();
      return affectedRows > 0;

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public boolean delete(int id) {
    String sql = "DELETE FROM questions WHERE id = ?";

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