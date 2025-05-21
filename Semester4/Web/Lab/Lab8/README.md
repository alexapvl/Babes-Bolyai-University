# Personal Library Application with Authentication

This application is a personal library management system with authentication, built using Angular for the frontend and ASP.NET Core for the backend.

## Features

- User authentication (register, login, logout)
- Book management (add, edit, delete)
- Lending functionality (lend books to friends, return books)
- Filter books by genre
- Protected routes requiring authentication

## Project Structure

- `/Frontend` - Angular frontend application
- `/Backend` - ASP.NET Core Web API backend
- `database.sql` - SQL script to create the database schema

## Setup Instructions

### Database Setup

1. Create the MySQL database using the provided SQL script:
   ```bash
   mysql -u root -p < database.sql
   ```

### Backend Setup

1. Navigate to the Backend directory:

   ```bash
   cd Backend
   ```

2. Restore the .NET packages:

   ```bash
   dotnet restore
   ```

3. Run the backend application:
   ```bash
   dotnet watch run
   ```
   The API will be available at https://localhost:7268

### Frontend Setup

1. Navigate to the Frontend directory:

   ```bash
   cd Frontend
   ```

2. Install the dependencies:

   ```bash
   pnpm install
   ```

3. Run the frontend application:
   ```bash
   pnpm start
   ```
   The application will be available at http://localhost:4200

## Default User

A default test user is created in the database:

- Username: `testuser`
- Password: `password`

## Technologies Used

- **Frontend**:

  - Angular 17
  - TypeScript
  - RxJS
  - Angular Router

- **Backend**:
  - ASP.NET Core 8
  - Entity Framework Core
  - JWT Authentication
  - MySQL Database (via Pomelo.EntityFrameworkCore.MySql)

## API Endpoints

### Authentication

- `POST /api/auth/register` - Register a new user
- `POST /api/auth/login` - Login a user
- `GET /api/auth/validate` - Validate authentication token

### Books

- `GET /api/books` - Get all books for the authenticated user
- `GET /api/books/{id}` - Get a specific book by ID
- `POST /api/books` - Add a new book
- `PUT /api/books/{id}` - Update an existing book
- `DELETE /api/books/{id}` - Delete a book
- `POST /api/books/{id}/lend` - Lend a book to someone
- `POST /api/books/{id}/return` - Mark a book as returned
