# Web Application Template Guide

## Adapting Quiz App Structure to Different Domains

This guide provides step-by-step instructions for adapting your Java Servlet + JSP + MySQL web application from one domain (e.g., Quiz App) to another (e.g., Software Developer/Project Management System).

---

## Project Structure Overview

```
project-root/
├── pom.xml                     # Maven configuration
├── database_schema.sql         # Database schema definition
├── seed.sql                   # Sample data (optional)
└── src/main/
    ├── java/com/[domain]/
    │   ├── model/             # Data models (POJOs)
    │   ├── dao/               # Database Access Objects
    │   ├── servlet/           # HTTP request handlers
    │   └── util/              # Utility classes
    └── webapp/
        ├── WEB-INF/
        │   ├── web.xml        # Servlet configuration
        │   └── *.jsp          # Protected JSP pages
        ├── *.jsp              # Public JSP pages
        └── css/               # Stylesheets
```

---

## Step-by-Step Adaptation Process

### Step 1: Update Project Configuration (5 minutes)

#### 1.1 Update `pom.xml`

- [ ] Change `<groupId>` from `com.quiz` to `com.[new-domain]`
- [ ] Change `<artifactId>` from `quiz-app` to `[new-domain]-app`
- [ ] Update `<finalName>` to match new domain
- [ ] Update Tomcat plugin `<path>` if needed

#### 1.2 Update Package Structure

- [ ] Rename package from `com.quiz` to `com.[new-domain]` in all Java files
- [ ] Use IDE's "Refactor > Rename Package" feature for efficiency

---

### Step 2: Design New Database Schema (10-15 minutes)

#### 2.1 Analyze Requirements

Based on your exam problem statement, identify:

- [ ] Main entities (e.g., SoftwareDeveloper, Project)
- [ ] Relationships between entities
- [ ] Required fields for each entity
- [ ] Foreign key relationships

#### 2.2 Create New `database_schema.sql`

```sql
-- Template structure:
CREATE DATABASE IF NOT EXISTS [new_domain]_app;
USE [new_domain]_app;

-- Replace with your entities
CREATE TABLE [entity1] (
    id INT AUTO_INCREMENT PRIMARY KEY,
    [field1] [TYPE] [CONSTRAINTS],
    [field2] [TYPE] [CONSTRAINTS],
    -- ... other fields
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE [entity2] (
    id INT AUTO_INCREMENT PRIMARY KEY,
    [foreign_key_id] INT NOT NULL,
    [field1] [TYPE] [CONSTRAINTS],
    -- ... other fields
    FOREIGN KEY ([foreign_key_id]) REFERENCES [entity1](id) ON DELETE CASCADE
);

-- Add sample data
INSERT INTO [entity1] ([field1], [field2]) VALUES
('sample1', 'value1'),
('sample2', 'value2');
```

#### 2.3 Key Database Patterns to Follow

- Always include `id INT AUTO_INCREMENT PRIMARY KEY`
- Add `created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP` for tracking
- Use proper foreign key constraints
- Keep consistent naming conventions (snake_case for DB, camelCase for Java)

---

### Step 3: Create New Model Classes (15-20 minutes)

#### 3.1 Model Class Template

For each database table, create a corresponding Java class:

```java
package com.[domain].model;

import java.time.LocalDateTime;

public class [EntityName] {
    private int id;
    private String [field1];
    private int [field2];
    private LocalDateTime createdAt;

    // Default constructor
    public [EntityName]() {}

    // Constructor with fields (excluding id and createdAt)
    public [EntityName](String [field1], int [field2]) {
        this.[field1] = [field1];
        this.[field2] = [field2];
    }

    // Full constructor
    public [EntityName](int id, String [field1], int [field2], LocalDateTime createdAt) {
        this.id = id;
        this.[field1] = [field1];
        this.[field2] = [field2];
        this.createdAt = createdAt;
    }

    // Getters and setters for all fields
    // ... (generate using IDE)

    @Override
    public String toString() {
        return "[EntityName]{" +
            "id=" + id +
            ", [field1]='" + [field1] + '\'' +
            ", [field2]=" + [field2] +
            '}';
    }
}
```

#### 3.2 Create Models for Each Entity

- [ ] Identify all entities from database schema
- [ ] Create corresponding model classes
- [ ] Ensure field types match database types (String, int, LocalDateTime, etc.)

---

### Step 4: Create DAO Classes (20-25 minutes)

#### 4.1 DAO Class Template

For each model, create a DAO class for database operations:

```java
package com.[domain].dao;

import com.[domain].model.[EntityName];
import com.[domain].util.DatabaseUtil;

import java.sql.*;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class [EntityName]DAO {

    // Find by ID
    public Optional<[EntityName]> findById(int id) {
        String sql = "SELECT * FROM [table_name] WHERE id = ?";

        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(sql)) {

            stmt.setInt(1, id);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                return Optional.of(mapResultSetToEntity(rs));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return Optional.empty();
    }

    // Find all
    public List<[EntityName]> findAll() {
        List<[EntityName]> entities = new ArrayList<>();
        String sql = "SELECT * FROM [table_name] ORDER BY id";

        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(sql);
             ResultSet rs = stmt.executeQuery()) {

            while (rs.next()) {
                entities.add(mapResultSetToEntity(rs));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return entities;
    }

    // Create
    public boolean create([EntityName] entity) {
        String sql = "INSERT INTO [table_name] ([field1], [field2]) VALUES (?, ?)";

        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

            stmt.setString(1, entity.get[Field1]());
            stmt.setInt(2, entity.get[Field2]());

            int affectedRows = stmt.executeUpdate();

            if (affectedRows > 0) {
                try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
                    if (generatedKeys.next()) {
                        entity.setId(generatedKeys.getInt(1));
                        return true;
                    }
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return false;
    }

    // Update
    public boolean update([EntityName] entity) {
        String sql = "UPDATE [table_name] SET [field1] = ?, [field2] = ? WHERE id = ?";

        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(sql)) {

            stmt.setString(1, entity.get[Field1]());
            stmt.setInt(2, entity.get[Field2]());
            stmt.setInt(3, entity.getId());

            return stmt.executeUpdate() > 0;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return false;
    }

    // Delete
    public boolean delete(int id) {
        String sql = "DELETE FROM [table_name] WHERE id = ?";

        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(sql)) {

            stmt.setInt(1, id);
            return stmt.executeUpdate() > 0;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return false;
    }

    // Helper method to map ResultSet to Entity
    private [EntityName] mapResultSetToEntity(ResultSet rs) throws SQLException {
        [EntityName] entity = new [EntityName]();
        entity.setId(rs.getInt("id"));
        entity.set[Field1](rs.getString("[field1]"));
        entity.set[Field2](rs.getInt("[field2]"));
        entity.setCreatedAt(rs.getTimestamp("created_at").toLocalDateTime());
        return entity;
    }

    // Add domain-specific methods here (e.g., findByName, findBySkill, etc.)
}
```

#### 4.2 Common DAO Patterns

- Always use `Optional<T>` for single entity returns
- Use `List<T>` for multiple entity returns
- Include proper exception handling
- Use `PreparedStatement` to prevent SQL injection
- Create helper methods for complex queries

---

### Step 5: Create Servlet Classes (25-30 minutes)

#### 5.1 Servlet Class Template

```java
package com.[domain].servlet;

import com.[domain].dao.[EntityName]DAO;
import com.[domain].model.[EntityName];

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.List;

public class [EntityName]Servlet extends HttpServlet {

    private [EntityName]DAO [entityName]DAO = new [EntityName]DAO();

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        String action = request.getParameter("action");

        if (action == null) {
            action = "list";
        }

        switch (action) {
            case "list":
                listEntities(request, response);
                break;
            case "show":
                showEntity(request, response);
                break;
            case "new":
                showNewForm(request, response);
                break;
            case "edit":
                showEditForm(request, response);
                break;
            default:
                listEntities(request, response);
                break;
        }
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        String action = request.getParameter("action");

        if (action == null) {
            response.sendError(HttpServletResponse.SC_BAD_REQUEST);
            return;
        }

        switch (action) {
            case "create":
                createEntity(request, response);
                break;
            case "update":
                updateEntity(request, response);
                break;
            case "delete":
                deleteEntity(request, response);
                break;
            default:
                response.sendError(HttpServletResponse.SC_BAD_REQUEST);
                break;
        }
    }

    private void listEntities(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        List<[EntityName]> entities = [entityName]DAO.findAll();
        request.setAttribute("entities", entities);
        request.getRequestDispatcher("/WEB-INF/[entity-name]-list.jsp").forward(request, response);
    }

    private void showEntity(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        int id = Integer.parseInt(request.getParameter("id"));
        [EntityName] entity = [entityName]DAO.findById(id).orElse(null);

        if (entity == null) {
            response.sendError(HttpServletResponse.SC_NOT_FOUND);
            return;
        }

        request.setAttribute("entity", entity);
        request.getRequestDispatcher("/WEB-INF/[entity-name]-detail.jsp").forward(request, response);
    }

    private void createEntity(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        String [field1] = request.getParameter("[field1]");
        String [field2] = request.getParameter("[field2]");

        // Validation
        if ([field1] == null || [field1].trim().isEmpty()) {
            request.setAttribute("error", "[Field1] is required");
            request.getRequestDispatcher("/WEB-INF/[entity-name]-new.jsp").forward(request, response);
            return;
        }

        [EntityName] entity = new [EntityName]([field1], Integer.parseInt([field2]));

        if ([entityName]DAO.create(entity)) {
            response.sendRedirect(request.getContextPath() + "/[entity-name]");
        } else {
            request.setAttribute("error", "Failed to create [entity-name]");
            request.getRequestDispatcher("/WEB-INF/[entity-name]-new.jsp").forward(request, response);
        }
    }

    // Add updateEntity and deleteEntity methods similarly
}
```

#### 5.2 Required Servlets Based on Exam Requirements

Analyze your exam requirements and create servlets for:

- [ ] Main entity listing/display
- [ ] User authentication (if required)
- [ ] Data filtering/searching functionality
- [ ] Bulk operations (if required)

---

### Step 6: Create JSP Pages (20-25 minutes)

#### 6.1 JSP Page Template

```jsp
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>[Page Title] - [App Name]</title>
    <link rel="stylesheet" href="${pageContext.request.contextPath}/css/style.css">
</head>
<body>
    <div class="container">
        <header>
            <h1>[Page Title]</h1>
            <nav>
                <a href="${pageContext.request.contextPath}/[main-page]">Home</a>
                <!-- Add other navigation links -->
            </nav>
        </header>

        <main>
            <!-- Display error messages -->
            <c:if test="${not empty error}">
                <div class="error-message">
                    ${error}
                </div>
            </c:if>

            <!-- Display success messages -->
            <c:if test="${not empty success}">
                <div class="success-message">
                    ${success}
                </div>
            </c:if>

            <!-- Main content -->
            <div class="content">
                <!-- Your content here -->
            </div>
        </main>
    </div>

    <!-- JavaScript for client-side functionality -->
    <script>
        // Add client-side filtering/validation if needed
    </script>
</body>
</html>
```

#### 6.2 Required JSP Pages

Based on exam requirements, create:

- [ ] Main listing page (show all entities)
- [ ] Detail view page (show single entity)
- [ ] Form pages (create/edit entities)
- [ ] User-specific views (filtered data)
- [ ] Search/filter functionality page

---

### Step 7: Update Configuration Files (10 minutes)

#### 7.1 Update `web.xml`

```xml
<!-- Update servlet definitions -->
<servlet>
    <servlet-name>[EntityName]Servlet</servlet-name>
    <servlet-class>com.[domain].servlet.[EntityName]Servlet</servlet-class>
</servlet>

<servlet-mapping>
    <servlet-name>[EntityName]Servlet</servlet-name>
    <url-pattern>/[entity-name]</url-pattern>
</servlet-mapping>
```

#### 7.2 Update Database Connection

Check `src/main/java/com/[domain]/util/DatabaseUtil.java`:

- [ ] Update database URL to match new database name
- [ ] Ensure connection parameters are correct

---

### Step 8: Add Domain-Specific Business Logic (15-20 minutes)

#### 8.1 Common Exam Requirements Implementation

**Filter by specific criteria (e.g., skills):**

```java
// In DAO class
public List<SoftwareDeveloper> findBySkill(String skill) {
    String sql = "SELECT * FROM software_developer WHERE skills LIKE ?";
    List<SoftwareDeveloper> developers = new ArrayList<>();

    try (Connection conn = DatabaseUtil.getConnection();
         PreparedStatement stmt = conn.prepareStatement(sql)) {

        stmt.setString(1, "%" + skill + "%");
        ResultSet rs = stmt.executeQuery();

        while (rs.next()) {
            developers.add(mapResultSetToEntity(rs));
        }
    } catch (SQLException e) {
        e.printStackTrace();
    }

    return developers;
}
```

**Bulk assignment operations:**

```java
// In DAO class
public boolean assignDeveloperToProjects(String developerName, List<String> projectNames) {
    // 1. Check if developer exists
    // 2. For each project: check if exists, create if not
    // 3. Update project members field
    // 4. Use transactions for data consistency
}
```

#### 8.2 Client-side JavaScript for Filtering

```javascript
function filterBySkill() {
  const skill = document.getElementById("skillFilter").value.toLowerCase();
  const rows = document.querySelectorAll(".developer-row");

  rows.forEach((row) => {
    const skills = row.querySelector(".skills").textContent.toLowerCase();
    if (skills.includes(skill) || skill === "") {
      row.style.display = "";
    } else {
      row.style.display = "none";
    }
  });
}
```

---

## Quick Reference Checklist

### Database Setup

- [ ] Create new database schema
- [ ] Update connection settings
- [ ] Add sample data
- [ ] Test database connection

### Java Backend

- [ ] Update package names
- [ ] Create model classes for all entities
- [ ] Create DAO classes with CRUD operations
- [ ] Create servlet classes for request handling
- [ ] Add domain-specific business logic

### Frontend

- [ ] Create JSP pages for all required views
- [ ] Add client-side filtering/validation
- [ ] Style with CSS
- [ ] Test all user interactions

### Configuration

- [ ] Update web.xml servlet mappings
- [ ] Update pom.xml project information
- [ ] Test deployment

### Testing

- [ ] Test all CRUD operations
- [ ] Test user authentication (if required)
- [ ] Test business logic requirements
- [ ] Test error handling

---

## Common Exam Requirements Patterns

### 1. User Authentication Pattern

```java
// Store current user in session
HttpSession session = request.getSession();
session.setAttribute("currentUser", username);

// Filter data based on current user
public List<Project> findProjectsByMember(String memberName) {
    String sql = "SELECT * FROM project WHERE members LIKE ?";
    // Implementation
}
```

### 2. Many-to-Many Relationship Pattern

```java
// For comma-separated members field
public boolean addMemberToProject(String memberName, String projectName) {
    // 1. Get current members
    // 2. Add new member if not exists
    // 3. Update members field
}
```

### 3. Auto-creation Pattern

```java
// Create entity if it doesn't exist
public boolean ensureProjectExists(String projectName) {
    if (!projectExists(projectName)) {
        Project project = new Project(projectName, "", "");
        return create(project);
    }
    return true;
}
```

---

## Time Management for Exam (Total: 2-3 hours)

- **Database Design & Setup**: 30 minutes
- **Model Classes**: 20 minutes
- **DAO Classes**: 40 minutes
- **Servlet Classes**: 45 minutes
- **JSP Pages**: 30 minutes
- **Testing & Debugging**: 30 minutes
- **Final Polish**: 15 minutes

---

## Pro Tips for Exam Success

1. **Start with Database**: Always design your database schema first
2. **Use IDE Features**: Leverage auto-generation for getters/setters, constructors
3. **Copy-Paste-Modify**: Use existing code as templates, don't write from scratch
4. **Test Incrementally**: Test each component as you build it
5. **Keep it Simple**: Don't over-engineer, focus on meeting requirements
6. **Handle Errors Gracefully**: Add basic error handling and validation
7. **Use Consistent Naming**: Follow Java naming conventions throughout
8. **Comment Complex Logic**: Add comments for business logic sections

Remember: The goal is to demonstrate understanding of the web development stack (Frontend + Backend + Database) within the time constraints of an exam environment.
