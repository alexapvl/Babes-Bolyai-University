package com.quiz.model;

import java.time.LocalDateTime;

public class User {
  private int id;
  private String username;
  private String passwordHash;
  private LocalDateTime createdAt;

  public User() {
  }

  public User(String username, String passwordHash) {
    this.username = username;
    this.passwordHash = passwordHash;
  }

  public User(int id, String username, String passwordHash, LocalDateTime createdAt) {
    this.id = id;
    this.username = username;
    this.passwordHash = passwordHash;
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

  public String getPasswordHash() {
    return passwordHash;
  }

  public void setPasswordHash(String passwordHash) {
    this.passwordHash = passwordHash;
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