import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { MatSelectModule } from '@angular/material/select';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatSnackBar, MatSnackBarModule } from '@angular/material/snack-bar';
import { FormsModule } from '@angular/forms';
import { RouterLink } from '@angular/router';
import { MatSlideToggleModule } from '@angular/material/slide-toggle';

import { BookService } from '../../services/book.service';
import { AuthService } from '../../services/auth.service';
import { Book } from '../../models/book.model';

@Component({
  selector: 'app-book-list',
  standalone: true,
  imports: [
    CommonModule,
    MatCardModule,
    MatButtonModule,
    MatIconModule,
    MatSelectModule,
    MatFormFieldModule,
    MatSnackBarModule,
    FormsModule,
    RouterLink,
    MatSlideToggleModule,
  ],
  templateUrl: './book-list.component.html',
  styleUrls: ['./book-list.component.scss'],
})
export class BookListComponent implements OnInit {
  books: Book[] = [];
  genres: string[] = [];
  selectedGenre: string = 'all';
  loading: boolean = false;
  currentUsername: string = '';
  showAllBooks: boolean = true;

  constructor(
    private bookService: BookService,
    private snackBar: MatSnackBar,
    private authService: AuthService
  ) {}

  ngOnInit(): void {
    this.currentUsername = this.authService.currentUserValue?.username || '';
    this.loadBooks();
    this.loadGenres();
  }

  loadBooks(): void {
    this.loading = true;
    console.log(
      `Loading books with: genre=${this.selectedGenre}, showAllBooks=${this.showAllBooks}`
    );
    this.bookService.getBooks(this.selectedGenre, this.showAllBooks).subscribe({
      next: (data) => {
        this.books = data;
        console.log(`Loaded ${data.length} books`);
        this.loading = false;
      },
      error: (error) => {
        console.error('Error fetching books:', error);
        this.snackBar.open('Failed to load books', 'Close', { duration: 3000 });
        this.loading = false;
      },
    });
  }

  loadGenres(): void {
    // Get unique genres from all books
    this.bookService.getBooks('all', true).subscribe({
      next: (books) => {
        this.genres = [
          ...new Set(
            books
              .map((book) => book.genre)
              .filter(
                (genre): genre is string =>
                  genre !== undefined && genre !== null && genre !== ''
              )
          ),
        ];
      },
    });
  }

  filterByGenre(): void {
    this.loadBooks();
  }

  toggleBookView(): void {
    console.log(`Toggle changed: showAllBooks=${this.showAllBooks}`);
    this.loadBooks();
  }

  isOwner(book: Book): boolean {
    return book.author === this.currentUsername;
  }

  deleteBook(id: number): void {
    const book = this.books.find((b) => b.id === id);
    if (!book) return;

    // Check if user is the owner of the book
    if (!this.isOwner(book)) {
      this.snackBar.open('You can only delete your own books', 'Close', {
        duration: 3000,
      });
      return;
    }

    if (confirm(`Are you sure you want to delete "${book.title}"?`)) {
      this.bookService.deleteBook(id).subscribe({
        next: () => {
          this.snackBar.open('Book deleted successfully', 'Close', {
            duration: 3000,
          });
          this.loadBooks(); // Reload the books after deletion
        },
        error: (error) => {
          console.error('Error deleting book:', error);
          this.snackBar.open('Failed to delete book', 'Close', {
            duration: 3000,
          });
        },
      });
    }
  }

  lendBook(book: Book): void {
    // Check if user is the owner of the book
    if (!this.isOwner(book)) {
      this.snackBar.open('You can only lend your own books', 'Close', {
        duration: 3000,
      });
      return;
    }

    // This would typically open a dialog for lent details
    // For simplicity, we'll just handle it here with a basic prompt
    const lentTo = prompt(
      'Enter the username of the person borrowing the book:'
    );
    if (!lentTo) return;

    // Validate if the username exists using the AuthService
    this.authService.checkUsernameExists(lentTo).subscribe({
      next: (response) => {
        if (!response.exists) {
          this.snackBar.open(
            'User not found. Please enter a valid username.',
            'Close',
            {
              duration: 3000,
            }
          );
          return;
        }

        // Username exists, proceed with lending
        const today = new Date().toISOString().slice(0, 10);

        if (book.id) {
          this.bookService.lendBook(book.id, lentTo, today).subscribe({
            next: (response) => {
              this.snackBar.open('Book lent successfully', 'Close', {
                duration: 3000,
              });
              this.loadBooks();
            },
            error: (error) => {
              console.error('Error lending book:', error);
              this.snackBar.open('Failed to lend book', 'Close', {
                duration: 3000,
              });
            },
          });
        }
      },
      error: (error) => {
        console.error('Error validating username:', error);
        this.snackBar.open('Cannot lend book to a non-existing user', 'Close', {
          duration: 3000,
        });
      },
    });
  }

  returnBook(id: number): void {
    const book = this.books.find((b) => b.id === id);
    if (!book) return;

    // Check if user is the owner of the book
    if (!this.isOwner(book)) {
      this.snackBar.open('You can only return your own books', 'Close', {
        duration: 3000,
      });
      return;
    }

    if (confirm('Mark this book as returned?')) {
      this.bookService.returnBook(id).subscribe({
        next: (response) => {
          this.snackBar.open('Book marked as returned', 'Close', {
            duration: 3000,
          });
          this.loadBooks();
        },
        error: (error) => {
          console.error('Error returning book:', error);
          this.snackBar.open('Failed to mark book as returned', 'Close', {
            duration: 3000,
          });
        },
      });
    }
  }
}
