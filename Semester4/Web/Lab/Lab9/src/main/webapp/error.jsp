<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" isErrorPage="true"%>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quiz App - Error</title>
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
                <h2>Error</h2>
            </div>
            <div class="card-body">
                <div class="alert alert-danger">
                    <p>An error has occurred. Please try again later.</p>
                    <p>Error code: <%= response.getStatus() %></p>
                    <% if (exception != null) { %>
                        <p>Error message: <%= exception.getMessage() %></p>
                    <% } %>
                </div>
                
                <a href="dashboard" class="btn">Return to Dashboard</a>
            </div>
        </div>
    </div>
</body>
</html> 