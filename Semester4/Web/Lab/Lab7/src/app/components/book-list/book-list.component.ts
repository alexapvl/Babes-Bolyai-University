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

import { BookService } from '../../services/book.service';
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
  ],
  templateUrl: './book-list.component.html',
  styleUrls: ['./book-list.component.scss'],
})
export class BookListComponent implements OnInit {
  books: Book[] = [];
  genres: string[] = [];
  selectedGenre: string = 'all';
  loading: boolean = false;

  constructor(
    private bookService: BookService,
    private snackBar: MatSnackBar
  ) {}

  ngOnInit(): void {
    this.loadBooks();
    this.loadGenres();
  }

  loadBooks(): void {
    this.loading = true;
    this.bookService.getBooks(this.selectedGenre).subscribe({
      next: (data) => {
        this.books = data;
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
    this.bookService.getBooks().subscribe({
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

  deleteBook(id: number): void {
    const book = this.books.find((b) => b.id === id);
    if (!book) return;

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
    // This would typically open a dialog for lent details
    // For simplicity, we'll just handle it here with a basic prompt
    const lentTo = prompt('Enter the name of the person borrowing the book:');
    if (!lentTo) return;

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
  }

  returnBook(id: number): void {
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
