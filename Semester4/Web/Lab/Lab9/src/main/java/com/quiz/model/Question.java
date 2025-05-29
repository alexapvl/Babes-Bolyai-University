package com.quiz.model;

import java.time.LocalDateTime;
import java.util.List;

public class Question {
  private int id;
  private int quizId;
  private String questionText;
  private int questionOrder;
  private LocalDateTime createdAt;
  private List<AnswerOption> answerOptions;

  public Question() {
  }

  public Question(int quizId, String questionText, int questionOrder) {
    this.quizId = quizId;
    this.questionText = questionText;
    this.questionOrder = questionOrder;
  }

  public Question(int id, int quizId, String questionText, int questionOrder, LocalDateTime createdAt) {
    this.id = id;
    this.quizId = quizId;
    this.questionText = questionText;
    this.questionOrder = questionOrder;
    this.createdAt = createdAt;
  }

  // Getters and setters
  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public int getQuizId() {
    return quizId;
  }

  public void setQuizId(int quizId) {
    this.quizId = quizId;
  }

  public String getQuestionText() {
    return questionText;
  }

  public void setQuestionText(String questionText) {
    this.questionText = questionText;
  }

  public int getQuestionOrder() {
    return questionOrder;
  }

  public void setQuestionOrder(int questionOrder) {
    this.questionOrder = questionOrder;
  }

  public LocalDateTime getCreatedAt() {
    return createdAt;
  }

  public void setCreatedAt(LocalDateTime createdAt) {
    this.createdAt = createdAt;
  }

  public List<AnswerOption> getAnswerOptions() {
    return answerOptions;
  }

  public void setAnswerOptions(List<AnswerOption> answerOptions) {
    this.answerOptions = answerOptions;
  }

  @Override
  public String toString() {
    return "Question{" +
        "id=" + id +
        ", quizId=" + quizId +
        ", questionText='" + questionText + '\'' +
        ", questionOrder=" + questionOrder +
        ", createdAt=" + createdAt +
        '}';
  }
}