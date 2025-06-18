package com.quiz.model;

public class Project {
  private int id;
  private int projectManagerId;
  private String name;
  private String description;
  private String members;

  public Project() {
  }

  public Project(int projectManagerId, String name, String description, String members) {
    this.projectManagerId = projectManagerId;
    this.name = name;
    this.description = description;
    this.members = members;
  }

  public Project(int id, int projectManagerId, String name, String description, String members) {
    this.id = id;
    this.projectManagerId = projectManagerId;
    this.name = name;
    this.description = description;
    this.members = members;
  }

  // Getters and setters
  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public int getProjectManagerId() {
    return projectManagerId;
  }

  public void setProjectManagerId(int projectManagerId) {
    this.projectManagerId = projectManagerId;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public String getDescription() {
    return description;
  }

  public void setDescription(String description) {
    this.description = description;
  }

  public String getMembers() {
    return members;
  }

  public void setMembers(String members) {
    this.members = members;
  }
}
