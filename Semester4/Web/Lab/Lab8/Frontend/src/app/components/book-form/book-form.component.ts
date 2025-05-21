import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { MatButtonModule } from '@angular/material/button';
import {
  ReactiveFormsModule,
  FormBuilder,
  FormGroup,
  Validators,
} from '@angular/forms';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatInputModule } from '@angular/material/input';
import { RouterLink, ActivatedRoute, Router } from '@angular/router';
import { MatSnackBar, MatSnackBarModule } from '@angular/material/snack-bar';
import { BookService } from '../../services/book.service';
import { Book } from '../../models/book.model';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-book-form',
  standalone: true,
  imports: [
    CommonModule,
    MatCardModule,
    MatButtonModule,
    ReactiveFormsModule,
    MatFormFieldModule,
    MatInputModule,
    RouterLink,
    MatSnackBarModule,
  ],
  templateUrl: './book-form.component.html',
  styleUrls: ['./book-form.component.scss'],
})
export class BookFormComponent implements OnInit {
  bookForm: FormGroup;
  editMode = false;
  bookId: number | null = null;
  loading = false;
  submitError: string = '';

  constructor(
    private fb: FormBuilder,
    private route: ActivatedRoute,
    private router: Router,
    private bookService: BookService,
    private snackBar: MatSnackBar,
    private authService: AuthService
  ) {
    this.bookForm = this.fb.group({
      title: ['', Validators.required],
      genre: [''],
      pages: [null],
    });
  }

  ngOnInit(): void {
    this.route.paramMap.subscribe((params) => {
      const id = params.get('id');
      if (id) {
        this.editMode = true;
        this.bookId = +id;
        this.loadBookData(this.bookId);
      }
    });
  }

  loadBookData(id: number): void {
    this.loading = true;
    this.bookService.getBookById(id).subscribe({
      next: (book: Book) => {
        this.bookForm.patchValue({
          title: book.title,
          genre: book.genre || '',
          pages: book.pages || null,
        });
        this.loading = false;
      },
      error: (error) => {
        console.error('Error loading book:', error);
        this.snackBar.open(
          'Error loading book data: ' + this.getErrorMessage(error),
          'Close',
          {
            duration: 5000,
          }
        );
        this.router.navigate(['/books']);
        this.loading = false;
      },
    });
  }

  onSubmit(): void {
    if (this.bookForm.invalid) {
      this.snackBar.open('Please fix the form errors', 'Close', {
        duration: 3000,
      });
      return;
    }

    this.submitError = '';
    this.loading = true;
    const username = this.authService.currentUserValue?.username;

    const book: Book = {
      ...this.bookForm.value,
      author: username || 'Unknown',
    };

    console.log('Submitting book:', book);

    if (this.editMode && this.bookId) {
      book.id = this.bookId;
      this.bookService.updateBook(book).subscribe({
        next: (response) => {
          console.log('Update response:', response);
          this.snackBar.open('Book updated successfully', 'Close', {
            duration: 3000,
          });
          this.router.navigate(['/books']);
          this.loading = false;
        },
        error: (error) => {
          console.error('Error updating book:', error);
          this.submitError = this.getErrorMessage(error);
          this.snackBar.open(
            'Error updating book: ' + this.submitError,
            'Close',
            {
              duration: 5000,
            }
          );
          this.loading = false;
        },
      });
    } else {
      this.bookService.addBook(book).subscribe({
        next: (response) => {
          console.log('Add response:', response);
          this.snackBar.open('Book added successfully', 'Close', {
            duration: 3000,
          });
          this.router.navigate(['/books']);
          this.loading = false;
        },
        error: (error) => {
          console.error('Error adding book:', error);
          this.submitError = this.getErrorMessage(error);
          this.snackBar.open(
            'Error adding book: ' + this.submitError,
            'Close',
            {
              duration: 5000,
            }
          );
          this.loading = false;
        },
      });
    }
  }

  private getErrorMessage(error: any): string {
    if (error.message) {
      return error.message;
    }
    if (typeof error === 'string') {
      return error;
    }
    return 'Unknown error occurred';
  }
}
