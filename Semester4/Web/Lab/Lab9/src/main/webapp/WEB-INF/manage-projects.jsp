<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Project Manager - Projects</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="navbar">
        <h1>Manage Projects</h1>
        <ul>
            <li><a href="projects">All Projects</a></li>
            <li><a href="my-projects?member=${user.name}">My Projects</a></li>
            <li><a href="software-developers">Software Developers</a></li>
            <li><a href="logout">Logout</a></li>
        </ul>
    </div>
    
    <div class="container">
        <h2>Welcome, ${user.name}!</h2>
        
        <form action="manage-projects" method="post">
            <label for="developerName">Developer Name</label>
            <input type="text" id="developerName" name="developerName" required>
            <label for="projectNames">Project Names</label>
            <input type="text" id="projectNames" name="projectNames" required>
            <button type="submit">Add Developer to Projects</button>
        </form>
        <c:if test="${not empty success}">
            <p>${success}</p>
        </c:if>
        <c:if test="${not empty error}">
            <p>${error}</p>
        </c:if>
    </div>
</body>
</html> 