# Application Topic Adaptation Template

## Overview

This template provides step-by-step instructions for adapting your Angular + PHP + MySQL application from one topic to another while maintaining the same structural design. Perfect for exam scenarios where you need to quickly change from one domain (e.g., books) to another (e.g., software developers and projects).

## Project Structure

```
├── src/app/
│   ├── components/         # UI Components
│   ├── models/            # Data Models
│   ├── services/          # API Communication
│   └── app.routes.ts      # Routing
├── api/                   # PHP Backend
└── database.sql          # Database Schema
```

---

## 🚀 Quick Adaptation Steps

### Step 1: Database Schema Changes

**File:** `database.sql`

1. **Rename Database:**

   ```sql
   CREATE DATABASE IF NOT EXISTS your_new_database_name;
   USE your_new_database_name;
   ```

2. **Replace Table Definitions:**

   - Remove existing tables
   - Add new table definitions based on your exam requirements

   **Example for SoftwareDeveloper & Project:**

   ```sql
   CREATE TABLE IF NOT EXISTS SoftwareDeveloper (
       id INT AUTO_INCREMENT PRIMARY KEY,
       name VARCHAR(255) NOT NULL,
       age INT,
       skills VARCHAR(500)
   );

   CREATE TABLE IF NOT EXISTS Project (
       id INT AUTO_INCREMENT PRIMARY KEY,
       ProjectManagerID INT,
       name VARCHAR(255) NOT NULL,
       description TEXT,
       members TEXT,
       FOREIGN KEY (ProjectManagerID) REFERENCES SoftwareDeveloper(id)
   );
   ```

3. **Update Database Connection:**
   - Update database name in `includes/db_connect.php`

### Step 2: Data Models (Angular)

**Directory:** `src/app/models/`

1. **Rename/Replace Model Files:**

   - Delete: `book.model.ts`
   - Create: `{entity1}.model.ts`, `{entity2}.model.ts`

2. **Update Model Interfaces:**

   ```typescript
   // Example: software-developer.model.ts
   export interface SoftwareDeveloper {
     id?: number;
     name: string;
     age?: number;
     skills?: string;
   }

   // Example: project.model.ts
   export interface Project {
     id?: number;
     ProjectManagerID?: number;
     name: string;
     description?: string;
     members?: string;
   }
   ```

### Step 3: Backend API (PHP)

**Directory:** `api/`

1. **Rename API Files:**

   - `get_books.php` → `get_{entities}.php`
   - `add_book_handler.php` → `add_{entity}_handler.php`
   - `edit_book_handler.php` → `edit_{entity}_handler.php`
   - `delete_book_handler.php` → `delete_{entity}_handler.php`
   - Add new handlers as needed

2. **Update SQL Queries in Each File:**

   - Replace table names
   - Replace column names
   - Update validation logic

   **Template for GET endpoint:**

   ```php
   $query = "SELECT id, column1, column2, column3 FROM {table_name}";

   // Add filters if needed
   if ($filter !== 'all') {
       $query .= " WHERE {column} = :filter";
   }
   $query .= " ORDER BY {sort_column} ASC";
   ```

3. **Update Form Data Handling:**
   ```php
   // Replace field names in $_POST handling
   $field1 = $_POST['field1'] ?? '';
   $field2 = $_POST['field2'] ?? '';
   ```

### Step 4: Angular Service Layer

**Directory:** `src/app/services/`

1. **Rename Service File:**

   - `book.service.ts` → `{entity}.service.ts`

2. **Update Service Class:**

   ```typescript
   @Injectable({
     providedIn: 'root',
   })
   export class {Entity}Service {
     private apiUrl = '/api';

     constructor(private http: HttpClient) {}

     // Update method names and endpoints
     get{Entities}(filter: string = 'all'): Observable<{Entity}[]> {
       return this.http
         .get<{Entity}[]>(`${this.apiUrl}/get_{entities}.php?filter=${filter}`)
         .pipe(catchError(this.handleError));
     }

     add{Entity}({entity}: {Entity}): Observable<any> {
       // Update API endpoint
       return this.http
         .post(`${this.apiUrl}/add_{entity}_handler.php`, {entity}, { headers })
         .pipe(catchError(this.handleError));
     }
   }
   ```

3. **Update Import Statements:**
   ```typescript
   import { {Entity} } from '../models/{entity}.model';
   ```

### Step 5: Angular Components

**Directory:** `src/app/components/`

1. **Rename Component Directories:**

   - `book-list/` → `{entity}-list/`
   - `book-form/` → `{entity}-form/`
   - `book-detail/` → `{entity}-detail/`

2. **Update Component Files in Each Directory:**

   - Rename `.ts`, `.html`, `.scss` files
   - Update class names
   - Update template references
   - Update form fields

   **Component Class Template:**

   ```typescript
   export class {Entity}ListComponent {
     {entities}: {Entity}[] = [];

     constructor(private {entity}Service: {Entity}Service) {}

     load{Entities}() {
       this.{entity}Service.get{Entities}().subscribe({
         next: ({entities}) => this.{entities} = {entities},
         error: (error) => console.error(error)
       });
     }
   }
   ```

3. **Update HTML Templates:**
   - Replace property names in interpolation: `{{book.title}}` → `{{entity.name}}`
   - Update form controls: `formControlName="title"` → `formControlName="name"`
   - Update table headers and data

### Step 6: Routing and Navigation

**File:** `src/app/app.routes.ts`

1. **Update Route Paths:**

   ```typescript
   export const routes: Routes = [
     { path: '', redirectTo: '/{entities}', pathMatch: 'full' },
     { path: '{entities}', component: {Entity}ListComponent },
     { path: 'add-{entity}', component: {Entity}FormComponent },
     { path: 'edit-{entity}/:id', component: {Entity}FormComponent },
   ];
   ```

2. **Update Component Imports:**
   ```typescript
   import { {Entity}ListComponent } from './components/{entity}-list/{entity}-list.component';
   ```

### Step 7: Application Component Updates

**File:** `src/app/app.component.html`

1. **Update Navigation Links:**
   ```html
   <nav>
     <a routerLink="/{entities}">View {Entities}</a>
     <a routerLink="/add-{entity}">Add {Entity}</a>
   </nav>
   ```

### Step 8: Service Registration

**File:** `src/app/app.config.ts`

1. **Update Service Imports (if manually registered):**
   ```typescript
   import { {Entity}Service } from './services/{entity}.service';
   ```

---

## 🎯 Exam-Specific Requirements Implementation

### User Authentication/Session

If exam requires user identification:

1. **Add User Input Component:**

   ```typescript
   // Add to main component
   currentUser: string = '';

   setCurrentUser(username: string) {
     this.currentUser = username;
     // Store in session/local storage if needed
   }
   ```

2. **Filter Data by User:**
   ```php
   // In PHP endpoints
   $query .= " WHERE members LIKE :username";
   $stmt->bindParam(':username', '%' . $username . '%');
   ```

### Complex Operations (Multi-table)

For operations involving multiple tables:

1. **Create Combined Endpoints:**

   ```php
   // assign_developer_to_projects.php
   // Handle multiple project assignments in single request
   ```

2. **Batch Operations Service:**
   ```typescript
   assignDeveloperToProjects(developerId: number, projectIds: number[]): Observable<any> {
     const payload = { developerId, projectIds };
     return this.http.post(`${this.apiUrl}/assign_developer.php`, payload);
   }
   ```

### Client-side Filtering

For JavaScript filtering requirements:

1. **Add Filter Component:**
   ```typescript
   filterBySkill(skill: string) {
     this.filteredDevelopers = this.allDevelopers.filter(dev =>
       dev.skills?.toLowerCase().includes(skill.toLowerCase())
     );
   }
   ```

---

## 🔧 Quick Reference Checklist

### Database Layer

- [ ] Update database name
- [ ] Replace table schemas
- [ ] Update foreign key relationships
- [ ] Test database connection

### Backend (PHP)

- [ ] Rename API files
- [ ] Update table names in queries
- [ ] Update column names
- [ ] Update form field handling
- [ ] Test all endpoints

### Frontend Models

- [ ] Create new model interfaces
- [ ] Update property names
- [ ] Update import statements

### Frontend Services

- [ ] Rename service file and class
- [ ] Update API endpoints
- [ ] Update method names
- [ ] Update model imports

### Frontend Components

- [ ] Rename component directories
- [ ] Update component classes
- [ ] Update templates (HTML)
- [ ] Update form fields
- [ ] Update data binding

### Navigation & Routing

- [ ] Update route paths
- [ ] Update component imports
- [ ] Update navigation links

### Testing

- [ ] Test database operations
- [ ] Test all API endpoints
- [ ] Test frontend functionality
- [ ] Test data flow end-to-end

---

## 💡 Pro Tips for Exam Success

1. **Start with Database:** Always begin with database schema - it drives everything else
2. **Work Backwards:** Database → API → Service → Components
3. **Test Early:** Test each layer before moving to the next
4. **Use Find & Replace:** Leverage IDE's find/replace for bulk renaming
5. **Keep Naming Consistent:** Use the same naming convention throughout all layers
6. **Focus on Core Features First:** Get basic CRUD working before adding complex features
7. **Prepare Templates:** Have template code snippets ready for common patterns

## Time Management for Exams

- **5 minutes:** Plan and understand requirements
- **10 minutes:** Database schema and API updates
- **15 minutes:** Frontend models and services
- **15 minutes:** Component updates
- **10 minutes:** Testing and debugging
- **5 minutes:** Final review and polish

---

_This template is designed for rapid adaptation during exam conditions while maintaining code quality and functionality._
