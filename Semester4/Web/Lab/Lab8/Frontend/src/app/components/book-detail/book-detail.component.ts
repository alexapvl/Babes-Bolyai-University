import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { MatButtonModule } from '@angular/material/button';
import { MatDividerModule } from '@angular/material/divider';
import { RouterLink, ActivatedRoute, Router } from '@angular/router';
import { MatSnackBar, MatSnackBarModule } from '@angular/material/snack-bar';
import { BookService } from '../../services/book.service';
import { AuthService } from '../../services/auth.service';
import { Book } from '../../models/book.model';
import { switchMap } from 'rxjs/operators';

@Component({
  selector: 'app-book-detail',
  standalone: true,
  imports: [
    CommonModule,
    MatCardModule,
    MatButtonModule,
    MatDividerModule,
    MatSnackBarModule,
    RouterLink,
  ],
  templateUrl: './book-detail.component.html',
  styleUrls: ['./book-detail.component.scss'],
})
export class BookDetailComponent implements OnInit {
  book: Book = {
    id: 0,
    title: 'Loading...',
    author: 'Loading...',
    genre: 'Loading...',
    pages: 0,
    lentTo: null,
    lentDate: null,
  };
  currentUsername: string = '';
  isOwner: boolean = false;

  constructor(
    private route: ActivatedRoute,
    private router: Router,
    private bookService: BookService,
    private snackBar: MatSnackBar,
    private authService: AuthService
  ) {}

  ngOnInit(): void {
    this.currentUsername = this.authService.currentUserValue?.username || '';
    this.loadBook();
  }

  loadBook(): void {
    this.route.paramMap.subscribe((params) => {
      const id = Number(params.get('id'));
      if (id) {
        this.bookService.getBookById(id).subscribe({
          next: (book: Book) => {
            this.book = book;
            this.isOwner = this.book.author === this.currentUsername;
          },
          error: (error) => {
            console.error('Error loading book:', error);
            this.snackBar.open(
              'Book not found or you do not have access to it',
              'Close',
              {
                duration: 3000,
              }
            );
            this.router.navigate(['/books']);
          },
        });
      }
    });
  }

  deleteBook(): void {
    if (!this.book.id) {
      return;
    }

    if (!this.isOwner) {
      this.snackBar.open('You can only delete your own books', 'Close', {
        duration: 3000,
      });
      return;
    }

    if (confirm(`Are you sure you want to delete "${this.book.title}"?`)) {
      this.bookService.deleteBook(this.book.id).subscribe({
        next: () => {
          this.snackBar.open('Book deleted successfully', 'Close', {
            duration: 3000,
          });
          this.router.navigate(['/books']);
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

  lendBook(): void {
    if (!this.book.id) {
      return;
    }

    if (!this.isOwner) {
      this.snackBar.open('You can only lend your own books', 'Close', {
        duration: 3000,
      });
      return;
    }

    const lentTo = prompt(
      'Enter the username of the person borrowing the book:'
    );
    if (!lentTo) return;

    // Validate if the username exists
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

        this.bookService.lendBook(this.book.id!, lentTo, today).subscribe({
          next: (response) => {
            this.book = response; // Update book with latest data
            this.snackBar.open('Book lent successfully', 'Close', {
              duration: 3000,
            });
          },
          error: (error) => {
            console.error('Error lending book:', error);
            this.snackBar.open('Failed to lend book', 'Close', {
              duration: 3000,
            });
          },
        });
      },
      error: (error) => {
        console.error('Error validating username:', error);
        this.snackBar.open('Failed to validate username', 'Close', {
          duration: 3000,
        });
      },
    });
  }

  returnBook(): void {
    if (!this.book.id) {
      return;
    }

    if (!this.isOwner) {
      this.snackBar.open('You can only return your own books', 'Close', {
        duration: 3000,
      });
      return;
    }

    if (confirm('Mark this book as returned?')) {
      this.bookService.returnBook(this.book.id).subscribe({
        next: (response) => {
          this.book = response; // Update book with latest data
          this.snackBar.open('Book marked as returned', 'Close', {
            duration: 3000,
          });
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
