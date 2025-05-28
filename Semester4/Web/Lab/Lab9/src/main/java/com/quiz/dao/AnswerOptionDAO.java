package com.quiz.dao;

import com.quiz.model.AnswerOption;
import com.quiz.util.DatabaseUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class AnswerOptionDAO {

  public List<AnswerOption> findByQuestionId(int questionId) {
    List<AnswerOption> options = new ArrayList<>();
    String sql = "SELECT * FROM answer_options WHERE question_id = ? ORDER BY option_order ASC";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, questionId);
      ResultSet rs = stmt.executeQuery();

      while (rs.next()) {
        AnswerOption option = new AnswerOption();
        option.setId(rs.getInt("id"));
        option.setQuestionId(rs.getInt("question_id"));
        option.setOptionText(rs.getString("option_text"));
        option.setCorrect(rs.getBoolean("is_correct"));
        option.setOptionOrder(rs.getInt("option_order"));
        options.add(option);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return options;
  }

  public Optional<AnswerOption> findById(int id) {
    String sql = "SELECT * FROM answer_options WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, id);
      ResultSet rs = stmt.executeQuery();

      if (rs.next()) {
        AnswerOption option = new AnswerOption();
        option.setId(rs.getInt("id"));
        option.setQuestionId(rs.getInt("question_id"));
        option.setOptionText(rs.getString("option_text"));
        option.setCorrect(rs.getBoolean("is_correct"));
        option.setOptionOrder(rs.getInt("option_order"));
        return Optional.of(option);
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return Optional.empty();
  }

  public boolean create(AnswerOption option) {
    String sql = "INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES (?, ?, ?, ?)";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

      stmt.setInt(1, option.getQuestionId());
      stmt.setString(2, option.getOptionText());
      stmt.setBoolean(3, option.isCorrect());
      stmt.setInt(4, option.getOptionOrder());

      int affectedRows = stmt.executeUpdate();

      if (affectedRows > 0) {
        try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
          if (generatedKeys.next()) {
            option.setId(generatedKeys.getInt(1));
            return true;
          }
        }
      }

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public boolean update(AnswerOption option) {
    String sql = "UPDATE answer_options SET question_id = ?, option_text = ?, is_correct = ?, option_order = ? WHERE id = ?";

    try (Connection conn = DatabaseUtil.getConnection();
        PreparedStatement stmt = conn.prepareStatement(sql)) {

      stmt.setInt(1, option.getQuestionId());
      stmt.setString(2, option.getOptionText());
      stmt.setBoolean(3, option.isCorrect());
      stmt.setInt(4, option.getOptionOrder());
      stmt.setInt(5, option.getId());

      int affectedRows = stmt.executeUpdate();
      return affectedRows > 0;

    } catch (SQLException e) {
      e.printStackTrace();
    }

    return false;
  }

  public boolean delete(int id) {
    String sql = "DELETE FROM answer_options WHERE id = ?";

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