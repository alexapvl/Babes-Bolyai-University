-- this is just for reference, we will use Entity Framework to create the database

CREATE DATABASE IF NOT EXISTS web_practice_asp_1;

USE web_practice_asp_1;

-- Create users table for authentication (simple version)
CREATE TABLE IF NOT EXISTS softwareDeveloper (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    age INT NULL,
    skills VARCHAR(255) NULL
);

-- Create books table (modified to include user ownership)
CREATE TABLE IF NOT EXISTS project (
    id INT AUTO_INCREMENT PRIMARY KEY,
    projectManagerId INT NOT NULL,
    FOREIGN KEY (projectManagerId) REFERENCES softwareDeveloper(id),
    name VARCHAR(255) NOT NULL,
    description VARCHAR(255) NULL,
    members VARCHAR(255) NULL
);