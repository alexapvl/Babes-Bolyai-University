CREATE DATABASE IF NOT EXISTS personal_library;

USE personal_library;

-- Create users table for authentication (simple version)
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);

-- Create books table (modified to include user ownership)
CREATE TABLE IF NOT EXISTS books (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    author VARCHAR(255) NOT NULL,
    genre VARCHAR(100),
    pages INT,
    lent_to VARCHAR(255) DEFAULT NULL, -- Stores the name of the person the book is lent to, NULL if available
    lent_date DATE DEFAULT NULL,      -- Stores the date the book was lent
    user_id INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

-- Insert a test user (password: "password" - plain text as requested)
INSERT INTO users (username, password) VALUES 
('testuser', 'password');