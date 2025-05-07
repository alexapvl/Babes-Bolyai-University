import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { MatButtonModule } from '@angular/material/button';
import { MatDividerModule } from '@angular/material/divider';
import { RouterLink, ActivatedRoute, Router } from '@angular/router';
import { BookService } from '../../services/book.service';
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
    lent_to: null,
    lent_date: null,
  };

  constructor(
    private route: ActivatedRoute,
    private router: Router,
    private bookService: BookService
  ) {}

  ngOnInit(): void {
    this.loadBook();
  }

  loadBook(): void {
    this.route.paramMap
      .pipe(
        switchMap((params) => {
          const id = Number(params.get('id'));
          return this.bookService.getBooks();
        })
      )
      .subscribe({
        next: (books: Book[]) => {
          const id = Number(this.route.snapshot.paramMap.get('id'));
          const foundBook = books.find((book) => book.id === id);
          if (foundBook) {
            this.book = foundBook;
          } else {
            console.error('Book not found');
            // Handle the case when book is not found
          }
        },
        error: (error) => {
          console.error('Error loading book:', error);
        },
      });
  }

  deleteBook(): void {
    if (!this.book.id) {
      return;
    }

    if (confirm(`Are you sure you want to delete "${this.book.title}"?`)) {
      this.bookService.deleteBook(this.book.id).subscribe({
        next: () => {
          this.router.navigate(['/books']);
        },
        error: (error) => {
          console.error('Error deleting book:', error);
        },
      });
    }
  }
}
