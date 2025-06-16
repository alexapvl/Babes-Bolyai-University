# Web Application Template - Domain Adaptation Guide

## Overview

This template helps you quickly adapt your Angular + ASP.NET + MySQL web application from one domain (currently: Personal Library) to another domain (e.g., Software Developer/Project Management) during exams.

## Current Architecture

- **Frontend**: Angular with components, services, models
- **Backend**: ASP.NET Core with Controllers, Services, Models
- **Database**: MySQL with Entity Framework-like approach
- **Authentication**: JWT-based user authentication

---

## 🚀 Quick Adaptation Steps (For Exam Speed)

### Step 1: Analyze New Domain Requirements

Before making changes, understand:

1. **Main entities** (e.g., SoftwareDeveloper, Project)
2. **Relationships** (foreign keys, one-to-many, many-to-many)
3. **Required functionality** (CRUD operations, filtering, assignments)
4. **User interactions** (who can do what)

### Step 2: Database Schema Adaptation (5-10 minutes)

#### 2.1 Update `database.sql`

```sql
-- Template for new domain
CREATE DATABASE IF NOT EXISTS [NEW_DOMAIN_NAME];
USE [NEW_DOMAIN_NAME];

-- Keep users table for authentication (usually stays the same)
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_login DATETIME DEFAULT NULL
);

-- Replace books table with your main entities
CREATE TABLE IF NOT EXISTS [MAIN_ENTITY] (
    id INT AUTO_INCREMENT PRIMARY KEY,
    [field1] [TYPE] [CONSTRAINTS],
    [field2] [TYPE] [CONSTRAINTS],
    -- Add user_id if entity belongs to user
    user_id INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS [SECONDARY_ENTITY] (
    id INT AUTO_INCREMENT PRIMARY KEY,
    [foreign_key_id] INT NOT NULL,
    [field1] [TYPE] [CONSTRAINTS],
    [field2] [TYPE] [CONSTRAINTS],
    FOREIGN KEY ([foreign_key_id]) REFERENCES [MAIN_ENTITY](id)
);
```

#### Example for Software Developer/Project Domain:

```sql
CREATE DATABASE IF NOT EXISTS software_project_management;
USE software_project_management;

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_login DATETIME DEFAULT NULL
);

CREATE TABLE IF NOT EXISTS softwaredeveloper (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    age INT,
    skills VARCHAR(500),
    user_id INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS project (
    id INT AUTO_INCREMENT PRIMARY KEY,
    projectmanagerid INT,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    members TEXT,
    user_id INT NOT NULL,
    FOREIGN KEY (projectmanagerid) REFERENCES softwaredeveloper(id),
    FOREIGN KEY (user_id) REFERENCES users(id)
);

INSERT INTO users (username, password_hash) VALUES
('testuser', '$2a$11$ysf7DHI9LaGlJbm0Zg0F1uBvE8NBXhkiXUF.6VV8vBdPLI4huRwrm');
```

### Step 3: Backend Model Adaptation (5 minutes)

#### 3.1 Replace Models in `Backend/Models/`

**Delete or rename**: `Book.cs`
**Create new models**:

```csharp
// Backend/Models/SoftwareDeveloper.cs
namespace Backend.Models
{
    public class SoftwareDeveloper
    {
        public int Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public int? Age { get; set; }
        public string? Skills { get; set; }
        public int UserId { get; set; }
    }
}

// Backend/Models/Project.cs
namespace Backend.Models
{
    public class Project
    {
        public int Id { get; set; }
        public int? ProjectManagerId { get; set; }
        public string Name { get; set; } = string.Empty;
        public string? Description { get; set; }
        public string? Members { get; set; }
        public int UserId { get; set; }
    }
}
```

### Step 4: Backend Service Adaptation (10 minutes)

#### 4.1 Rename/Replace Services

- Copy `BookService.cs` → `SoftwareDeveloperService.cs`
- Create new `ProjectService.cs`

#### 4.2 Update Service Logic

**Template for main entity service**:

```csharp
// Backend/Services/SoftwareDeveloperService.cs
public class SoftwareDeveloperService
{
    private readonly string _connectionString;

    public SoftwareDeveloperService(IConfiguration configuration)
    {
        _connectionString = configuration.GetConnectionString("DefaultConnection");
    }

    // Basic CRUD operations
    public async Task<List<SoftwareDeveloper>> GetDevelopersAsync(int userId)
    {
        // Implementation similar to GetBooksAsync but for developers
    }

    public async Task<SoftwareDeveloper?> GetDeveloperByIdAsync(int id)
    {
        // Implementation similar to GetBookByIdAsync
    }

    public async Task<SoftwareDeveloper> AddDeveloperAsync(SoftwareDeveloper developer)
    {
        // Implementation similar to AddBookAsync
    }

    // Domain-specific methods
    public async Task<List<SoftwareDeveloper>> GetDevelopersBySkillAsync(string skill)
    {
        // New method for filtering by skill
    }
}
```

### Step 5: Backend Controller Adaptation (10 minutes)

#### 5.1 Copy and Rename Controllers

- Copy `BooksController.cs` → `SoftwareDevelopersController.cs`
- Create `ProjectsController.cs`

#### 5.2 Update Controller Routes and Methods

```csharp
[Route("api/[controller]")]
[ApiController]
[Authorize]
public class SoftwareDevelopersController : ControllerBase
{
    private readonly SoftwareDeveloperService _developerService;

    public SoftwareDevelopersController(SoftwareDeveloperService developerService)
    {
        _developerService = developerService;
    }

    [HttpGet]
    public async Task<ActionResult<IEnumerable<SoftwareDeveloper>>> GetDevelopers([FromQuery] string skill = "all")
    {
        // Adapt from GetBooks method
    }

    [HttpPost("assign-to-projects")]
    public async Task<ActionResult> AssignDeveloperToProjects([FromBody] AssignRequest request)
    {
        // New method for domain-specific functionality
    }
}
```

#### 5.3 Update Program.cs Dependencies

```csharp
// Add new services
builder.Services.AddScoped<SoftwareDeveloperService>();
builder.Services.AddScoped<ProjectService>();

// Comment out or remove old services
// builder.Services.AddScoped<BookService>();
```

### Step 6: Frontend Model Adaptation (5 minutes)

#### 6.1 Update Frontend Models

Replace `Frontend/src/app/models/book.model.ts` with:

```typescript
// models/software-developer.model.ts
export interface SoftwareDeveloper {
  id: number;
  name: string;
  age?: number;
  skills?: string;
  userId: number;
}

// models/project.model.ts
export interface Project {
  id: number;
  projectManagerId?: number;
  name: string;
  description?: string;
  members?: string;
  userId: number;
}
```

### Step 7: Frontend Service Adaptation (10 minutes)

#### 7.1 Update/Replace Services

Copy `book.service.ts` → `software-developer.service.ts`:

```typescript
@Injectable({
  providedIn: "root",
})
export class SoftwareDeveloperService {
  private apiUrl = "/api/softwaredevelopers";

  constructor(private http: HttpClient) {}

  getDevelopers(skill: string = "all"): Observable<SoftwareDeveloper[]> {
    return this.http.get<SoftwareDeveloper[]>(`${this.apiUrl}?skill=${skill}`);
  }

  getDevelopersBySkill(skill: string): Observable<SoftwareDeveloper[]> {
    return this.http.get<SoftwareDeveloper[]>(`${this.apiUrl}?skill=${skill}`);
  }

  assignDeveloperToProjects(
    developerId: number,
    projectNames: string[]
  ): Observable<any> {
    return this.http.post(`${this.apiUrl}/assign-to-projects`, {
      developerId,
      projectNames,
    });
  }
}
```

### Step 8: Frontend Component Adaptation (15-20 minutes)

#### 8.1 Rename Component Directories

- `book-list/` → `developer-list/`
- `book-form/` → `developer-form/`
- `book-detail/` → `developer-detail/`

#### 8.2 Update Component Logic

**Template for list component**:

```typescript
export class DeveloperListComponent implements OnInit {
  developers: SoftwareDeveloper[] = [];
  selectedSkill: string = "";

  constructor(private developerService: SoftwareDeveloperService) {}

  ngOnInit(): void {
    this.loadDevelopers();
  }

  loadDevelopers(): void {
    this.developerService.getDevelopers().subscribe({
      next: (developers) => (this.developers = developers),
      error: (error) => console.error("Error loading developers:", error),
    });
  }

  filterBySkill(): void {
    if (this.selectedSkill) {
      this.developerService.getDevelopersBySkill(this.selectedSkill).subscribe({
        next: (developers) => (this.developers = developers),
        error: (error) => console.error("Error filtering developers:", error),
      });
    }
  }
}
```

#### 8.3 Update Templates

Replace book-related HTML with domain-specific content:

```html
<!-- developer-list.component.html -->
<div class="developer-list">
  <h2>Software Developers</h2>

  <div class="filter-section">
    <input
      [(ngModel)]="selectedSkill"
      placeholder="Filter by skill (e.g., Java)"
    />
    <button (click)="filterBySkill()">Filter</button>
  </div>

  <div class="developer-grid">
    <div *ngFor="let developer of developers" class="developer-card">
      <h3>{{developer.name}}</h3>
      <p>Age: {{developer.age}}</p>
      <p>Skills: {{developer.skills}}</p>
    </div>
  </div>
</div>
```

### Step 9: Update Routing (5 minutes)

#### 9.1 Update `app.routes.ts`

```typescript
export const routes: Routes = [
  { path: "", redirectTo: "/developers", pathMatch: "full" },
  {
    path: "developers",
    component: DeveloperListComponent,
    canActivate: [AuthGuard],
  },
  {
    path: "projects",
    component: ProjectListComponent,
    canActivate: [AuthGuard],
  },
  {
    path: "developer/:id",
    component: DeveloperDetailComponent,
    canActivate: [AuthGuard],
  },
  // ... auth routes stay the same
];
```

### Step 10: Update Navigation and App Component (5 minutes)

#### 10.1 Update Menu Items

```html
<!-- app.component.html -->
<nav *ngIf="isLoggedIn">
  <a routerLink="/developers">Developers</a>
  <a routerLink="/projects">Projects</a>
  <button (click)="logout()">Logout</button>
</nav>
```

---

## 🎯 Domain-Specific Implementation Tips

### For Software Developer/Project Management:

1. **User identification**: Use username input field for current user
2. **Member management**: Store members as comma-separated string or JSON
3. **Skill filtering**: Implement client-side JavaScript filtering
4. **Project assignment**: Create bulk assignment endpoint
5. **Auto-creation**: Handle non-existing projects in assignment logic

### For Other Domains (e.g., Student/Course, Employee/Department):

1. **Identify main entities** and their relationships
2. **Map required functionality** to CRUD operations
3. **Handle domain-specific business logic** (filtering, assignments, validations)
4. **Create appropriate UI controls** (dropdowns, multi-select, etc.)

---

## 🚨 Common Exam Gotchas

1. **Database Connection**: Update connection string in `appsettings.json`
2. **CORS Issues**: Ensure frontend proxy is configured for API calls
3. **Authentication**: Keep user authentication logic, just change entities
4. **Foreign Keys**: Pay attention to relationship directions and constraints
5. **Client-side Filtering**: Use JavaScript, not server calls for simple filters
6. **Bulk Operations**: Use single HTTP request for multiple assignments

---

## ⚡ Speed Tips for Exam

1. **Start with database schema** - everything else follows from this
2. **Copy and rename** files rather than creating from scratch
3. **Use Find & Replace** for bulk renaming (Book → SoftwareDeveloper)
4. **Test endpoints** with simple HTTP requests before building UI
5. **Keep authentication logic** unchanged - focus on business entities
6. **Use browser dev tools** to debug API calls quickly

---

## 📋 Exam Day Checklist

- [ ] Analyze requirements and identify entities
- [ ] Create/update database schema
- [ ] Update backend models
- [ ] Adapt backend services and controllers
- [ ] Update frontend models and services
- [ ] Modify frontend components and templates
- [ ] Update routing and navigation
- [ ] Test basic CRUD operations
- [ ] Implement domain-specific features
- [ ] Test filtering and bulk operations

**Estimated Total Time**: 60-90 minutes for complete adaptation

Remember: Focus on functionality over perfect code. The goal is a working application that meets requirements!
