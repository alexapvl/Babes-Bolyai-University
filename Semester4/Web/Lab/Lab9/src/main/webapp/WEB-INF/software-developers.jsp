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
        <h1>Software Developers</h1>
        <ul>
            <li><a href="projects">All Projects</a></li>
            <li><a href="my-projects?member=${user.name}">My Projects</a></li>
            <li><a href="manage-projects">Manage Projects</a></li>
            <li><a href="logout">Logout</a></li>
        </ul>
    </div>
    
    <div class="container">
        <h2>Welcome, ${user.name}!</h2>
        
        <div class="card">
            <div class="card-header">
                <h3>Software Developers</h3>
            </div>

           <!-- this form will call a frontend script to filter by skills -->
            <form action="software-developers">
              <input type="text" name="skill" placeholder="Search by skills">
              <button type="submit">Search</button>
            </form>
            <div class="card-body">
                <c:if test="${empty softwareDevelopers}">
                    <p>No developers available.</p>
                </c:if>
                
                <div class="quiz-list">
                    <c:forEach var="dev" items="${softwareDevelopers}">
                        <div class="quiz-item">
                            <h3>${dev.name}</h3>
                            <p>Skills: ${dev.skills}</p>
                        </div>
                    </c:forEach>
                </div>
            </div>
        </div>
    </div>
</body>
<script>
  const softwareDevelopers = [
    <c:forEach var="dev" items="${softwareDevelopers}" varStatus="status">
      {
        id: ${dev.id},
        name: "${dev.name}",
        skills: "${dev.skills}"
      }<c:if test="${!status.last}">,</c:if>
    </c:forEach>
  ];

  function filterDevelopers(skill) {
    const developerCards = document.querySelectorAll('.quiz-item');
    let visibleCount = 0;
    
    developerCards.forEach((card, index) => {
      const developer = softwareDevelopers[index];
      
      if (skill === '' || developer.skills.toLowerCase().includes(skill.toLowerCase())) {
        card.style.display = 'block';
        visibleCount++;
      } else {
        card.style.display = 'none';
      }
    });
    
    // Show/hide no results message
    let noResultsMsg = document.querySelector('.no-results');
    
    if (visibleCount === 0) {
      if (!noResultsMsg) {
        noResultsMsg = document.createElement('p');
        noResultsMsg.className = 'no-results';
        noResultsMsg.textContent = 'No developers found with the specified skill.';
        document.querySelector('.quiz-list').appendChild(noResultsMsg);
      }
    } else {
      if (noResultsMsg) {
        noResultsMsg.remove();
      }
    }
  }

  document.querySelector('form').addEventListener('submit', function(e) {
    e.preventDefault();
    const skill = document.querySelector('input[name="skill"]').value.trim();
    filterDevelopers(skill);
  });
</script>
</html> 