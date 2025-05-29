package com.quiz.model;

import java.time.LocalDateTime;
import java.util.List;

public class QuizAttempt {
  private int id;
  private int userId;
  private int quizId;
  private int score;
  private int totalQuestions;
  private LocalDateTime startedAt;
  private LocalDateTime completedAt;
  private List<UserAnswer> userAnswers;

  public QuizAttempt() {
  }

  public QuizAttempt(int userId, int quizId, int totalQuestions) {
    this.userId = userId;
    this.quizId = quizId;
    this.totalQuestions = totalQuestions;
    this.score = 0;
  }

  public QuizAttempt(int id, int userId, int quizId, int score, int totalQuestions,
      LocalDateTime startedAt, LocalDateTime completedAt) {
    this.id = id;
    this.userId = userId;
    this.quizId = quizId;
    this.score = score;
    this.totalQuestions = totalQuestions;
    this.startedAt = startedAt;
    this.completedAt = completedAt;
  }

  // Getters and setters
  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public int getUserId() {
    return userId;
  }

  public void setUserId(int userId) {
    this.userId = userId;
  }

  public int getQuizId() {
    return quizId;
  }

  public void setQuizId(int quizId) {
    this.quizId = quizId;
  }

  public int getScore() {
    return score;
  }

  public void setScore(int score) {
    this.score = score;
  }

  public int getTotalQuestions() {
    return totalQuestions;
  }

  public void setTotalQuestions(int totalQuestions) {
    this.totalQuestions = totalQuestions;
  }

  public LocalDateTime getStartedAt() {
    return startedAt;
  }

  public void setStartedAt(LocalDateTime startedAt) {
    this.startedAt = startedAt;
  }

  public LocalDateTime getCompletedAt() {
    return completedAt;
  }

  public void setCompletedAt(LocalDateTime completedAt) {
    this.completedAt = completedAt;
  }

  public List<UserAnswer> getUserAnswers() {
    return userAnswers;
  }

  public void setUserAnswers(List<UserAnswer> userAnswers) {
    this.userAnswers = userAnswers;
  }

  @Override
  public String toString() {
    return "QuizAttempt{" +
        "id=" + id +
        ", userId=" + userId +
        ", quizId=" + quizId +
        ", score=" + score +
        ", totalQuestions=" + totalQuestions +
        ", startedAt=" + startedAt +
        ", completedAt=" + completedAt +
        '}';
  }
}