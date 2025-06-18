package com.quiz.model;

public class SoftwareDeveloper {

  private int id;
  private String name;
  private String skills;

  public SoftwareDeveloper() {
  }

  public SoftwareDeveloper(String name) {
    this.name = name;
  }

  public SoftwareDeveloper(int id, String name, String skills) {
    this.id = id;
    this.name = name;
    this.skills = skills;
  }

  // Getters and setters
  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public String getSkills() {
    return skills;
  }

  public void setSkills(String skills) {
    this.skills = skills;
  }

  @Override
  public String toString() {
    return "User{" +
        "id=" + id +
        ", name='" + name + '\'' +
        ", skills=" + skills +
        '}';
  }
}
