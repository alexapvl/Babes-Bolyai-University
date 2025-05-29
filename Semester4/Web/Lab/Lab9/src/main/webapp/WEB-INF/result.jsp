<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quiz App - Results</title>
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
                <h2>Quiz Results: ${quiz.title}</h2>
            </div>
            <div class="card-body">
                <div class="result-summary">
                    <h3>Your Score: ${attempt.score} / ${attempt.totalQuestions}</h3>
                    <c:set var="percentage" value="${(attempt.score / attempt.totalQuestions) * 100}" />
                    <div style="width: 100%; background-color: #f0f0f0; border-radius: 4px; height: 30px; margin: 20px 0;">
                        <div style="background-color: #4CAF50; height: 100%; width: ${percentage}%; border-radius: 4px; text-align: center; line-height: 30px; color: white; font-weight: bold;">
                            <c:if test="${attempt.score > 0}">
                                <fmt:formatNumber value="${percentage}" maxFractionDigits="0" />%
                            </c:if>
                        </div>
                    </div>
                </div>
                
                <div class="result-detail">
                    <div class="result-box correct">
                        <h3>Correct Answers</h3>
                        <p class="large-text">${correctAnswers}</p>
                    </div>
                    
                    <div class="result-box incorrect">
                        <h3>Incorrect Answers</h3>
                        <p class="large-text">${incorrectAnswers}</p>
                    </div>
                    
                    <c:if test="${not empty bestAttempt}">
                        <div class="result-box best-score">
                            <h3>Your Best Score</h3>
                            <p class="large-text">${bestAttempt.score} / ${bestAttempt.totalQuestions}</p>
                            <c:if test="${not empty bestAttemptDate}">
                                <p>${bestAttemptDate}</p>
                            </c:if>
                        </div>
                    </c:if>
                </div>
                
                <div style="text-align: center; margin-top: 30px;">
                    <a href="quiz-config?quizId=${quiz.id}" class="btn">Try Again</a>
                    <a href="dashboard" class="btn">Back to Dashboard</a>
                </div>
            </div>
        </div>
    </div>
    
    <style>
        .large-text {
            font-size: 2em;
            font-weight: bold;
            margin: 10px 0;
        }
    </style>
</body>
</html> 