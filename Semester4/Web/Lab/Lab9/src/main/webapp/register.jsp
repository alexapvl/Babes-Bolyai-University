<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quiz App - Register</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="navbar">
        <h1>Quiz Application</h1>
    </div>
    
    <div class="container">
        <div class="card">
            <div class="card-header">
                <h2>Create Account</h2>
            </div>
            <div class="card-body">
                <c:if test="${not empty error}">
                    <div class="alert alert-danger">
                        ${error}
                    </div>
                </c:if>
                
                <form action="register" method="post" id="registerForm">
                    <div class="form-group">
                        <label for="username">Username:</label>
                        <input type="text" id="username" name="username" class="form-control" 
                               value="${param.username}" required minlength="3" maxlength="50">
                        <small>Username must be between 3 and 50 characters</small>
                    </div>
                    
                    <div class="form-group">
                        <label for="password">Password:</label>
                        <input type="password" id="password" name="password" class="form-control" 
                               required minlength="6">
                        <small>Password must be at least 6 characters long</small>
                    </div>
                    
                    <div class="form-group">
                        <label for="confirmPassword">Confirm Password:</label>
                        <input type="password" id="confirmPassword" name="confirmPassword" class="form-control" 
                               required minlength="6">
                        <small>Please confirm your password</small>
                    </div>
                    
                    <button type="submit" class="btn">Register</button>
                </form>
                
                <div style="margin-top: 20px; text-align: center;">
                    <p>Already have an account? <a href="login">Login here</a></p>
                </div>
            </div>
        </div>
    </div>
    
    <script>
        // Client-side validation
        document.getElementById('registerForm').addEventListener('submit', function(e) {
            const username = document.getElementById('username').value.trim();
            const password = document.getElementById('password').value;
            const confirmPassword = document.getElementById('confirmPassword').value;
            
            // Username validation
            if (username.length < 3 || username.length > 50) {
                e.preventDefault();
                alert('Username must be between 3 and 50 characters');
                return;
            }
            
            // Check for valid characters in username (alphanumeric and underscore)
            if (!/^[a-zA-Z0-9_]+$/.test(username)) {
                e.preventDefault();
                alert('Username can only contain letters, numbers, and underscores');
                return;
            }
            
            // Password validation
            if (password.length < 6) {
                e.preventDefault();
                alert('Password must be at least 6 characters long');
                return;
            }
            
            // Confirm password validation
            if (password !== confirmPassword) {
                e.preventDefault();
                alert('Passwords do not match');
                return;
            }
        });
        
        // Real-time password confirmation validation
        document.getElementById('confirmPassword').addEventListener('input', function() {
            const password = document.getElementById('password').value;
            const confirmPassword = this.value;
            
            if (confirmPassword && password !== confirmPassword) {
                this.setCustomValidity('Passwords do not match');
            } else {
                this.setCustomValidity('');
            }
        });
        
        // Real-time username validation
        document.getElementById('username').addEventListener('input', function() {
            const username = this.value.trim();
            
            if (username && !/^[a-zA-Z0-9_]+$/.test(username)) {
                this.setCustomValidity('Username can only contain letters, numbers, and underscores');
            } else {
                this.setCustomValidity('');
            }
        });
    </script>
</body>
</html> 