import { Injectable } from '@angular/core';
import {
  HttpClient,
  HttpErrorResponse,
  HttpHeaders,
} from '@angular/common/http';
import { Observable, throwError } from 'rxjs';
import { catchError, tap } from 'rxjs/operators';
import { Book } from '../models/book.model';

@Injectable({
  providedIn: 'root',
})
export class BookService {
  // Base URL to the PHP API
  private apiUrl = '/api';

  constructor(private http: HttpClient) {}

  // Get all books, optionally filtered by genre
  getBooks(genre: string = 'all'): Observable<Book[]> {
    return this.http
      .get<Book[]>(`${this.apiUrl}/get_books.php?genre=${genre}`)
      .pipe(catchError(this.handleError));
  }

  // Add a new book
  addBook(book: Book): Observable<any> {
    console.log('Sending book data:', book);

    // Try sending as JSON first
    const headers = new HttpHeaders({
      'Content-Type': 'application/json',
    });

    return this.http
      .post(`${this.apiUrl}/add_book_handler.php`, book, { headers })
      .pipe(
        tap((response) => console.log('Add book response:', response)),
        catchError((error) => {
          console.error('Error adding book as JSON:', error);

          // If JSON fails, try FormData as fallback
          const formData = new FormData();
          formData.append('title', book.title);
          formData.append('author', book.author);

          if (book.genre) {
            formData.append('genre', book.genre);
          }

          if (book.pages) {
            formData.append('pages', book.pages.toString());
          }

          console.log('Trying FormData approach...');
          return this.http
            .post(`${this.apiUrl}/add_book_handler.php`, formData)
            .pipe(
              tap((response) => console.log('FormData response:', response)),
              catchError(this.handleError)
            );
        })
      );
  }

  // Update an existing book
  updateBook(book: Book): Observable<any> {
    // Try with JSON first
    const headers = new HttpHeaders({
      'Content-Type': 'application/json',
    });

    return this.http
      .post(`${this.apiUrl}/edit_book_handler.php`, book, { headers })
      .pipe(
        tap((response) => console.log('Update book response:', response)),
        catchError((error) => {
          console.error('Error updating book as JSON:', error);

          // Fall back to FormData
          const formData = new FormData();

          if (book.id) {
            formData.append('id', book.id.toString());
          }

          formData.append('title', book.title);
          formData.append('author', book.author);

          if (book.genre) {
            formData.append('genre', book.genre);
          }

          if (book.pages) {
            formData.append('pages', book.pages.toString());
          }

          return this.http
            .post(`${this.apiUrl}/edit_book_handler.php`, formData)
            .pipe(
              tap((response) => console.log('FormData response:', response)),
              catchError(this.handleError)
            );
        })
      );
  }

  // Delete a book
  deleteBook(id: number): Observable<any> {
    const payload = { bookId: id };
    const headers = new HttpHeaders({
      'Content-Type': 'application/json',
    });

    return this.http
      .post(`${this.apiUrl}/delete_book_handler.php`, payload, { headers })
      .pipe(
        catchError((error) => {
          // Fallback to FormData
          const formData = new FormData();
          formData.append('bookId', id.toString());

          return this.http
            .post(`${this.apiUrl}/delete_book_handler.php`, formData)
            .pipe(catchError(this.handleError));
        })
      );
  }

  // Lend a book
  lendBook(bookId: number, lentTo: string, lentDate: string): Observable<any> {
    const payload = {
      action: 'lend',
      bookId,
      lentTo,
      lentDate,
    };

    const headers = new HttpHeaders({
      'Content-Type': 'application/json',
    });

    return this.http
      .post(`${this.apiUrl}/lend_return_handler.php`, payload, { headers })
      .pipe(
        catchError((error) => {
          // Fallback to FormData
          const formData = new FormData();
          formData.append('action', 'lend');
          formData.append('bookId', bookId.toString());
          formData.append('lentTo', lentTo);
          formData.append('lentDate', lentDate);

          return this.http
            .post(`${this.apiUrl}/lend_return_handler.php`, formData)
            .pipe(catchError(this.handleError));
        })
      );
  }

  // Return a book
  returnBook(bookId: number): Observable<any> {
    const payload = {
      action: 'return',
      bookId,
    };

    const headers = new HttpHeaders({
      'Content-Type': 'application/json',
    });

    return this.http
      .post(`${this.apiUrl}/lend_return_handler.php`, payload, { headers })
      .pipe(
        catchError((error) => {
          // Fallback to FormData
          const formData = new FormData();
          formData.append('action', 'return');
          formData.append('bookId', bookId.toString());

          return this.http
            .post(`${this.apiUrl}/lend_return_handler.php`, formData)
            .pipe(catchError(this.handleError));
        })
      );
  }

  // Error handling method
  private handleError(error: HttpErrorResponse) {
    let errorMessage = 'Unknown error occurred';

    if (error.error instanceof ErrorEvent) {
      // Client-side error
      errorMessage = `Error: ${error.error.message}`;
    } else {
      // Server-side error
      errorMessage = `Error Code: ${error.status}\nMessage: ${error.message}`;

      // Try to parse error response if it's JSON
      if (typeof error.error === 'object') {
        try {
          const errorObj = error.error;
          errorMessage += `\nServer response: ${
            errorObj.message || JSON.stringify(errorObj)
          }`;
        } catch (e) {
          console.error('Could not parse error response:', e);
        }
      } else if (typeof error.error === 'string') {
        errorMessage += `\nServer response: ${error.error}`;
      }
    }

    console.error(errorMessage);
    return throwError(() => new Error(errorMessage));
  }
}
