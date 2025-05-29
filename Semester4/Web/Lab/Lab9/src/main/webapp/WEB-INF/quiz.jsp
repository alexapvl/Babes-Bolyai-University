<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quiz App - ${quiz.title}</title>
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
                <h2>${quiz.title} - Page ${currentPage} of ${totalPages}</h2>
                <div class="progress-bar" style="background-color: #f0f0f0; border-radius: 4px; height: 20px; margin-top: 10px;">
                    <div style="background-color: #4CAF50; height: 100%; width: ${(currentPage / totalPages) * 100}%; border-radius: 4px;"></div>
                </div>
            </div>
            <div class="card-body">
                <form action="quiz" method="post" id="quizForm">
                    <c:forEach var="question" items="${questions}" varStatus="status">
                        <div class="question">
                            <h3>${status.index + 1}. ${question.questionText}</h3>
                            <div class="options">
                                <c:forEach var="option" items="${question.answerOptions}">
                                    <label class="option">
                                        <input type="radio" name="question_${question.id}" value="${option.id}" required>
                                        ${option.optionText}
                                    </label>
                                </c:forEach>
                            </div>
                        </div>
                    </c:forEach>
                    
                    <div class="pagination">
                        <c:if test="${currentPage > 1}">
                            <a href="javascript:history.back()" class="btn">Previous</a>
                        </c:if>
                        <c:if test="${currentPage == 1}">
                            <a href="dashboard" class="btn btn-danger">Cancel Quiz</a>
                        </c:if>
                        
                        <button type="submit" class="btn">
                            <c:choose>
                                <c:when test="${currentPage == totalPages}">Finish Quiz</c:when>
                                <c:otherwise>Next</c:otherwise>
                            </c:choose>
                        </button>
                    </div>
                </form>
            </div>
        </div>
    </div>
    
    <script>
        // Confirm before leaving the page
        window.addEventListener('beforeunload', function(e) {
            // Cancel the event
            e.preventDefault();
            // Chrome requires returnValue to be set
            e.returnValue = '';
        });
        
        // Don't show confirmation when submitting the form
        document.getElementById('quizForm').addEventListener('submit', function() {
            window.removeEventListener('beforeunload', function() {});
        });
    </script>
</body>
</html> 