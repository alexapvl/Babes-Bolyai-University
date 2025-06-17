package com.quiz.model;

import java.time.LocalDateTime;

public class User {
  private int id;
  private String username;
  private LocalDateTime createdAt;

  public User() {
  }

  public User(String username) {
    this.username = username;
  }

  public User(int id, String username, LocalDateTime createdAt) {
    this.id = id;
    this.username = username;
    this.createdAt = createdAt;
  }

  // Getters and setters
  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public String getUsername() {
    return username;
  }

  public void setUsername(String username) {
    this.username = username;
  }

  public LocalDateTime getCreatedAt() {
    return createdAt;
  }

  public void setCreatedAt(LocalDateTime createdAt) {
    this.createdAt = createdAt;
  }

  @Override
  public String toString() {
    return "User{" +
        "id=" + id +
        ", username='" + username + '\'' +
        ", createdAt=" + createdAt +
        '}';
  }
}