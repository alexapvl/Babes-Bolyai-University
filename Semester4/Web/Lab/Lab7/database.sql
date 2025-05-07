CREATE DATABASE IF NOT EXISTS personal_library;

USE personal_library;

CREATE TABLE IF NOT EXISTS books (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    author VARCHAR(255) NOT NULL,
    genre VARCHAR(100),
    pages INT,
    lent_to VARCHAR(255) DEFAULT NULL, -- Stores the name of the person the book is lent to, NULL if available
    lent_date DATE DEFAULT NULL       -- Stores the date the book was lent
);