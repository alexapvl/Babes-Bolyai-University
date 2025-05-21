import { Injectable } from '@angular/core';
import {
  HttpClient,
  HttpErrorResponse,
  HttpHeaders,
  HttpParams,
} from '@angular/common/http';
import { Observable, throwError } from 'rxjs';
import { catchError } from 'rxjs/operators';
import { Book } from '../models/book.model';
import { AuthService } from './auth.service';
import { Router } from '@angular/router';

@Injectable({
  providedIn: 'root',
})
export class BookService {
  // Base URL to the API
  private apiUrl = '/api/books';

  constructor(
    private http: HttpClient,
    private authService: AuthService,
    private router: Router
  ) {}

  // Get all books, optionally filtered by genre and including all authors
  getBooks(
    genre: string = 'all',
    includeAllAuthors: boolean = false
  ): Observable<Book[]> {
    let params = new HttpParams();
    if (genre !== 'all') {
      params = params.set('genre', genre);
    }
    if (includeAllAuthors) {
      params = params.set('allAuthors', 'true');
    }

    console.log(
      `API call: genre=${genre}, includeAllAuthors=${includeAllAuthors}, params=`,
      params.toString()
    );

    return this.http
      .get<Book[]>(this.apiUrl, { params })
      .pipe(catchError((error) => this.handleError(error)));
  }

  // Get a book by ID
  getBookById(id: number): Observable<Book> {
    return this.http
      .get<Book>(`${this.apiUrl}/${id}`)
      .pipe(catchError((error) => this.handleError(error)));
  }

  // Add a new book
  addBook(book: Book): Observable<Book> {
    return this.http
      .post<Book>(`${this.apiUrl}`, book)
      .pipe(catchError((error) => this.handleError(error)));
  }

  // Update an existing book
  updateBook(book: Book): Observable<Book> {
    return this.http
      .put<Book>(`${this.apiUrl}/${book.id}`, book)
      .pipe(catchError((error) => this.handleError(error)));
  }

  // Delete a book
  deleteBook(id: number): Observable<any> {
    return this.http
      .delete(`${this.apiUrl}/${id}`)
      .pipe(catchError((error) => this.handleError(error)));
  }

  // Lend a book
  lendBook(bookId: number, lentTo: string, lentDate: string): Observable<Book> {
    const payload = {
      LentTo: lentTo,
      LentDate: lentDate,
    };

    return this.http
      .post<Book>(`${this.apiUrl}/${bookId}/lend`, payload)
      .pipe(catchError((error) => this.handleError(error)));
  }

  // Return a book
  returnBook(bookId: number): Observable<Book> {
    return this.http
      .post<Book>(`${this.apiUrl}/${bookId}/return`, {})
      .pipe(catchError((error) => this.handleError(error)));
  }

  // Error handling method
  private handleError(error: HttpErrorResponse) {
    let errorMessage = 'Unknown error occurred';

    if (error.status === 401) {
      // Unauthorized - clear auth data and redirect to login
      this.authService.logout();
      this.router.navigate(['/login']);
      errorMessage = 'Session expired. Please log in again.';
    } else if (error.error instanceof ErrorEvent) {
      // Client-side error
      errorMessage = `Error: ${error.error.message}`;
    } else {
      // Server-side error
      errorMessage = `Error Code: ${error.status}\nMessage: ${error.message}`;

      if (
        error.error &&
        typeof error.error === 'object' &&
        'message' in error.error
      ) {
        errorMessage = error.error.message as string;
      }
    }

    console.error(errorMessage);
    return throwError(() => new Error(errorMessage));
  }
}
