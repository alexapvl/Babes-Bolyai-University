# Quiz Application

A web-based quiz application built with JSP/Servlet technology that allows users to take quizzes with configurable parameters.

## Features

- User registration and authentication (login/logout)
- Quiz selection from available quizzes
- Configurable number of questions per quiz
- Configurable number of questions per page (pagination)
- Results page showing correct/incorrect answers
- Tracking of user's best scores
- Responsive design
- Input validation for all forms
- Password hashing for security

## Prerequisites

- Java Development Kit (JDK) 17 or higher
- Apache Maven
- MySQL Database
- Apache Tomcat 9 or higher

## Setup Instructions

### 1. Database Setup

1. Create a MySQL database by running the SQL script:

```bash
mysql -u root -p < database_schema.sql
```

2. Update database connection settings in `src/main/java/com/quiz/util/DatabaseUtil.java` if needed:

```java
private static final String DB_URL = "jdbc:mysql://localhost:3306/quiz_app?useSSL=false&serverTimezone=UTC";
private static final String DB_USERNAME = "root";
private static final String DB_PASSWORD = ""; // Change this to your MySQL root password
```

### 2. Build the Application

```bash
mvn clean package
```

### 3. Deploy to Tomcat

1. Copy the generated WAR file from `target/quiz-app.war` to your Tomcat's webapps directory.
2. Start Tomcat server.
3. Access the application at: http://localhost:8080/quiz-app/

### 4. Run with Maven (Alternative)

You can also run the application directly using Maven:

```bash
mvn tomcat7:run
```

Then access the application at: http://localhost:8080/quiz/

## Getting Started

### User Registration

1. Access the application homepage
2. Click "Register here" on the login page
3. Fill in the registration form with:
   - Username (3-50 characters, alphanumeric and underscores only)
   - Password (minimum 6 characters)
   - Confirm password
4. Submit the form to create your account

### User Login

You can log in using either:

- Your newly created account credentials
- The default demo account:
  - Username: admin
  - Password: admin123

## Project Structure

- `src/main/java/com/quiz/model/` - Model classes
- `src/main/java/com/quiz/dao/` - Data Access Objects
- `src/main/java/com/quiz/servlet/` - Servlet controllers
- `src/main/java/com/quiz/util/` - Utility classes
- `src/main/webapp/` - JSP views and web resources
- `src/main/webapp/WEB-INF/` - Protected JSP views
- `src/main/webapp/css/` - CSS stylesheets

## Security Features

- Password hashing using BCrypt
- Session-based authentication
- Input validation and sanitization
- Protection against common web vulnerabilities
- Secure password requirements

## License

This project is open source and available under the [MIT License](LICENSE).
