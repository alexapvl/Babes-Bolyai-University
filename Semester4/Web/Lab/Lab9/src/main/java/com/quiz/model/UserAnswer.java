package com.quiz.model;

import java.time.LocalDateTime;

public class UserAnswer {
  private int id;
  private int attemptId;
  private int questionId;
  private int answerOptionId;
  private boolean isCorrect;
  private LocalDateTime answeredAt;

  public UserAnswer() {
  }

  public UserAnswer(int attemptId, int questionId, int answerOptionId, boolean isCorrect) {
    this.attemptId = attemptId;
    this.questionId = questionId;
    this.answerOptionId = answerOptionId;
    this.isCorrect = isCorrect;
  }

  public UserAnswer(int id, int attemptId, int questionId, int answerOptionId,
      boolean isCorrect, LocalDateTime answeredAt) {
    this.id = id;
    this.attemptId = attemptId;
    this.questionId = questionId;
    this.answerOptionId = answerOptionId;
    this.isCorrect = isCorrect;
    this.answeredAt = answeredAt;
  }

  // Getters and setters
  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public int getAttemptId() {
    return attemptId;
  }

  public void setAttemptId(int attemptId) {
    this.attemptId = attemptId;
  }

  public int getQuestionId() {
    return questionId;
  }

  public void setQuestionId(int questionId) {
    this.questionId = questionId;
  }

  public int getAnswerOptionId() {
    return answerOptionId;
  }

  public void setAnswerOptionId(int answerOptionId) {
    this.answerOptionId = answerOptionId;
  }

  public boolean isCorrect() {
    return isCorrect;
  }

  public void setCorrect(boolean correct) {
    isCorrect = correct;
  }

  public LocalDateTime getAnsweredAt() {
    return answeredAt;
  }

  public void setAnsweredAt(LocalDateTime answeredAt) {
    this.answeredAt = answeredAt;
  }

  @Override
  public String toString() {
    return "UserAnswer{" +
        "id=" + id +
        ", attemptId=" + attemptId +
        ", questionId=" + questionId +
        ", answerOptionId=" + answerOptionId +
        ", isCorrect=" + isCorrect +
        ", answeredAt=" + answeredAt +
        '}';
  }
}