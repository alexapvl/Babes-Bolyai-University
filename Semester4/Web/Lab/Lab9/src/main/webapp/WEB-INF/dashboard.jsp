<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quiz App - Dashboard</title>
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
        <h2>Welcome, ${user.username}!</h2>
        
        <div class="card">
            <div class="card-header">
                <h3>Available Quizzes</h3>
            </div>
            <div class="card-body">
                <c:if test="${empty quizzes}">
                    <p>No quizzes available.</p>
                </c:if>
                
                <div class="quiz-list">
                    <c:forEach var="quiz" items="${quizzes}">
                        <div class="quiz-item">
                            <h3>${quiz.title}</h3>
                            <p>${quiz.description}</p>
                            <a href="quiz-config?quizId=${quiz.id}" class="btn">Start Quiz</a>
                        </div>
                    </c:forEach>
                </div>
            </div>
        </div>
        
        <div class="card">
            <div class="card-header">
                <h3>Your Quiz History</h3>
            </div>
            <div class="card-body">
                <c:if test="${empty attempts}">
                    <p>You haven't taken any quizzes yet.</p>
                </c:if>
                
                <c:if test="${not empty attempts}">
                    <table style="width:100%; border-collapse: collapse;">
                        <thead>
                            <tr>
                                <th style="text-align:left; padding:8px; border-bottom:1px solid #ddd;">Quiz</th>
                                <th style="text-align:center; padding:8px; border-bottom:1px solid #ddd;">Score</th>
                                <th style="text-align:center; padding:8px; border-bottom:1px solid #ddd;">Date</th>
                            </tr>
                        </thead>
                        <tbody>
                            <c:forEach var="attempt" items="${attempts}">
                                <tr>
                                    <td style="padding:8px; border-bottom:1px solid #ddd;">
                                        <c:forEach var="quiz" items="${quizzes}">
                                            <c:if test="${quiz.id eq attempt.quizId}">
                                                ${quiz.title}
                                            </c:if>
                                        </c:forEach>
                                    </td>
                                    <td style="text-align:center; padding:8px; border-bottom:1px solid #ddd;">
                                        ${attempt.score} / ${attempt.totalQuestions}
                                    </td>
                                    <td style="text-align:center; padding:8px; border-bottom:1px solid #ddd;">
                                        ${formattedDates[attempt.id]}
                                    </td>
                                </tr>
                            </c:forEach>
                        </tbody>
                    </table>
                </c:if>
            </div>
        </div>
    </div>
</body>
</html> 