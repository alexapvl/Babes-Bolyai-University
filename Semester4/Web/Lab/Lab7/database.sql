CREATE DATABASE IF NOT EXISTS web_exam;

USE web_exam;

-- Create users table for authentication (simple version)
CREATE TABLE IF NOT EXISTS User (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS Product (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    price DECIMAL(10,2) NOT NULL
);

CREATE TABLE IF NOT EXISTS Orders (
    id INT AUTO_INCREMENT PRIMARY KEY,
    userId INT NOT NULL,
    FOREIGN KEY (userId) REFERENCES User(id),
    totalPrice DECIMAL(10,2) NOT NULL
);

CREATE TABLE IF NOT EXISTS OrderItem (
    id INT AUTO_INCREMENT PRIMARY KEY,
    orderId INT NOT NULL,
    FOREIGN KEY (orderId) REFERENCES Orders(id),
    productId INT NOT NULL,
    FOREIGN KEY (productId) REFERENCES Product(id)
);

-- INSERT INTO User (username) values ('testuser');

INSERT INTO Product (name, price) VALUES ('BOOK-Math', 15.2);
INSERT INTO Product (name, price) VALUES ('BOOK-Science', 12.3);
INSERT INTO Product (name, price) VALUES ('TOY-Car', 20.7);