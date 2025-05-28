package com.quiz.model;

import java.time.LocalDateTime;
import java.util.List;

public class Quiz {
  private int id;
  private String title;
  private String description;
  private LocalDateTime createdAt;
  private List<Question> questions;

  public Quiz() {
  }

  public Quiz(String title, String description) {
    this.title = title;
    this.description = description;
  }

  public Quiz(int id, String title, String description, LocalDateTime createdAt) {
    this.id = id;
    this.title = title;
    this.description = description;
    this.createdAt = createdAt;
  }

  // Getters and setters
  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public String getTitle() {
    return title;
  }

  public void setTitle(String title) {
    this.title = title;
  }

  public String getDescription() {
    return description;
  }

  public void setDescription(String description) {
    this.description = description;
  }

  public LocalDateTime getCreatedAt() {
    return createdAt;
  }

  public void setCreatedAt(LocalDateTime createdAt) {
    this.createdAt = createdAt;
  }

  public List<Question> getQuestions() {
    return questions;
  }

  public void setQuestions(List<Question> questions) {
    this.questions = questions;
  }

  @Override
  public String toString() {
    return "Quiz{" +
        "id=" + id +
        ", title='" + title + '\'' +
        ", description='" + description + '\'' +
        ", createdAt=" + createdAt +
        '}';
  }
}