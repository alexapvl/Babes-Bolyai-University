# Personal Library - Angular UI with PHP Backend

This project implements a Personal Library application with an Angular frontend and PHP backend.

## Project Structure

- **Angular Frontend**: User interface built with Angular and Angular Material
- **PHP Backend**: API endpoints in the `/api` folder
- **MySQL Database**: Stores book data as defined in `database.sql`

## Setup Instructions

### Prerequisites

- Node.js and pnpm installed
- PHP server (Apache/Nginx)
- MySQL database

### Database Setup

1. Import the database schema:

```sql
mysql -u yourusername -p < database.sql
```

2. Update database connection settings in `includes/db_connect.php` if needed.

### Running the Application

1. Install dependencies:

```
pnpm install
```

2. Start the PHP server in one terminal:

```
cd Lab7
php -S localhost:8000
```

3. Start the Angular development server in another terminal:

```
cd Lab7
pnpm start
```

4. Open your browser and navigate to:

```
http://localhost:4200/
```

The Angular development server will automatically proxy API requests to your PHP server.

## Features

- Browse all books with filtering by genre
- Add new books to your collection
- Edit existing book details
- Delete books from your collection
- Track lending status (lend and return books)
- Responsive UI with Angular Material components

## Implementation Details

The application consists of:

1. **Angular Components**:

   - Book List: Displays all books with filtering options
   - Book Form: For adding and editing books
   - Book Detail: Shows detailed information about a book

2. **PHP API Endpoints**:
   - `get_books.php`: Retrieves books with optional genre filtering
   - `add_book_handler.php`: Adds new books
   - `edit_book_handler.php`: Updates existing books
   - `delete_book_handler.php`: Deletes books
   - `lend_return_handler.php`: Handles lending and returning of books
