<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quiz App - Configure Quiz</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="navbar">
        <h1>Quiz Application</h1>
        <ul>
            <li><a href="dashboard">Dashboard</a></li>
            <li><a href="logout">Logout</a></li>
        </ul>
    </div>
    
    <div class="container">
        <div class="card">
            <div class="card-header">
                <h2>Configure Quiz: ${quiz.title}</h2>
                <p>${quiz.description}</p>
            </div>
            <div class="card-body">
                <form action="quiz-config" method="post">
                    <input type="hidden" name="quizId" value="${quiz.id}">
                    
                    <div class="form-group">
                        <label for="totalQuestions">Number of Questions:</label>
                        <input type="number" id="totalQuestions" name="totalQuestions" class="form-control" 
                               min="1" max="${totalQuestions}" value="${totalQuestions}" required>
                        <small>Maximum: ${totalQuestions} questions</small>
                    </div>
                    
                    <div class="form-group">
                        <label for="questionsPerPage">Questions Per Page:</label>
                        <input type="number" id="questionsPerPage" name="questionsPerPage" class="form-control" 
                               min="1" max="10" value="1" required>
                        <small>Recommended: 1-5 questions per page</small>
                    </div>
                    
                    <div style="display: flex; justify-content: space-between;">
                        <a href="dashboard" class="btn btn-danger">Cancel</a>
                        <button type="submit" class="btn">Start Quiz</button>
                    </div>
                </form>
            </div>
        </div>
    </div>
    
    <script>
        // Validate form inputs
        document.querySelector('form').addEventListener('submit', function(e) {
            const totalQuestions = parseInt(document.getElementById('totalQuestions').value);
            const questionsPerPage = parseInt(document.getElementById('questionsPerPage').value);
            const maxQuestions = parseInt(${totalQuestions});
            
            if (totalQuestions <= 0 || totalQuestions > maxQuestions) {
                e.preventDefault();
                alert('Please enter a valid number of questions (1-' + maxQuestions + ')');
            }
            
            if (questionsPerPage <= 0) {
                e.preventDefault();
                alert('Please enter at least 1 question per page');
            }
        });
    </script>
</body>
</html> 