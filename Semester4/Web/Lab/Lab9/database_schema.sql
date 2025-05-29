-- Quiz Application Database Schema
-- Run this script in your MySQL database

CREATE DATABASE IF NOT EXISTS quiz_app;
USE quiz_app;

-- Users table for authentication
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Quizzes table
CREATE TABLE quizzes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Questions table
CREATE TABLE questions (
    id INT AUTO_INCREMENT PRIMARY KEY,
    quiz_id INT NOT NULL,
    question_text TEXT NOT NULL,
    question_order INT DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (quiz_id) REFERENCES quizzes(id) ON DELETE CASCADE
);

-- Answer options table
CREATE TABLE answer_options (
    id INT AUTO_INCREMENT PRIMARY KEY,
    question_id INT NOT NULL,
    option_text VARCHAR(500) NOT NULL,
    is_correct BOOLEAN DEFAULT FALSE,
    option_order INT DEFAULT 0,
    FOREIGN KEY (question_id) REFERENCES questions(id) ON DELETE CASCADE
);

-- User quiz attempts table
CREATE TABLE quiz_attempts (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    quiz_id INT NOT NULL,
    score INT DEFAULT 0,
    total_questions INT DEFAULT 0,
    started_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    completed_at TIMESTAMP NULL,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (quiz_id) REFERENCES quizzes(id) ON DELETE CASCADE
);

-- User answers for each attempt
CREATE TABLE user_answers (
    id INT AUTO_INCREMENT PRIMARY KEY,
    attempt_id INT NOT NULL,
    question_id INT NOT NULL,
    answer_option_id INT NOT NULL,
    is_correct BOOLEAN DEFAULT FALSE,
    answered_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (attempt_id) REFERENCES quiz_attempts(id) ON DELETE CASCADE,
    FOREIGN KEY (question_id) REFERENCES questions(id) ON DELETE CASCADE,
    FOREIGN KEY (answer_option_id) REFERENCES answer_options(id) ON DELETE CASCADE
);

-- Insert sample data
-- Sample quiz
INSERT INTO quizzes (title, description) VALUES 
('Java Programming Basics', 'Test your knowledge of basic Java programming concepts');

-- Sample questions
INSERT INTO questions (quiz_id, question_text, question_order) VALUES 
(1, 'What is the correct way to declare a variable in Java?', 1),
(1, 'Which keyword is used to create a class in Java?', 2),
(1, 'What is the default value of a boolean variable in Java?', 3),
(1, 'Which method is the entry point of a Java application?', 4),
(1, 'What is inheritance in Java?', 5);

-- Sample answer options
-- Question 1
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES 
(1, 'int x = 5;', TRUE, 1),
(1, 'variable int x = 5;', FALSE, 2),
(1, 'int x := 5;', FALSE, 3),
(1, 'declare int x = 5;', FALSE, 4);

-- Question 2
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES 
(2, 'class', TRUE, 1),
(2, 'Class', FALSE, 2),
(2, 'object', FALSE, 3),
(2, 'new', FALSE, 4);

-- Question 3
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES 
(3, 'false', TRUE, 1),
(3, 'true', FALSE, 2),
(3, 'null', FALSE, 3),
(3, '0', FALSE, 4);

-- Question 4
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES 
(4, 'main()', TRUE, 1),
(4, 'start()', FALSE, 2),
(4, 'run()', FALSE, 3),
(4, 'init()', FALSE, 4);

-- Question 5
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES 
(5, 'A mechanism where one class acquires properties of another class', TRUE, 1),
(5, 'A way to create multiple instances of a class', FALSE, 2),
(5, 'A method to hide implementation details', FALSE, 3),
(5, 'A technique to handle exceptions', FALSE, 4);

-- Create a default admin user (password: admin123)
INSERT INTO users (username, password_hash) VALUES 
('admin', '$2a$12$kLGkqS0XtaW.jvqD/rWzfOrQR7C8z8VQ8PqJ1YzjKhY8Jz6EXWVG6'); 