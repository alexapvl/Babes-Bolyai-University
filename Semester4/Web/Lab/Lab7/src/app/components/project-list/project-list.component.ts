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

import { ProjectService } from '../../services/project.service';
import { Project } from '../../models/project.model';

@Component({
  selector: 'app-project-list',
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
  templateUrl: './project-list.component.html',
  styleUrls: ['./project-list.component.scss'],
})
export class ProjectListComponent implements OnInit {
  projects: Project[] = [];
  loading: boolean = false;

  constructor(
    private projectService: ProjectService,
    private snackBar: MatSnackBar
  ) {}

  ngOnInit(): void {
    this.loadProjects();
  }

  loadProjects(): void {
    this.loading = true;
    this.projectService.getProjects().subscribe({
      next: (data) => {
        this.projects = data;
        this.loading = false;
      },
      error: (error) => {
        console.error('Error fetching projects:', error);
        this.snackBar.open('Failed to load projects', 'Close', {
          duration: 3000,
        });
        this.loading = false;
      },
    });
  }

  // loadGenres(): void {
  //   // Get unique genres from all books
  //   this.bookService.getBooks().subscribe({
  //     next: (books) => {
  //       this.genres = [
  //         ...new Set(
  //           books
  //             .map((book) => book.genre)
  //             .filter(
  //               (genre): genre is string =>
  //                 genre !== undefined && genre !== null && genre !== ''
  //             )
  //         ),
  //       ];
  //     },
  //   });
  // }

  // filterByGenre(): void {
  //   this.loadBooks();
  // }

  // -------------------------------
  // deleteProject(id: number): void {
  //   const project = this.projects.find((project) => project.id === id);
  //   if (!project) return;

  //   if (confirm(`Are you sure you want to delete "${project.name}"?`)) {
  //     this.projectService.deleteBook(id).subscribe({
  //       next: () => {
  //         this.snackBar.open('Book deleted successfully', 'Close', {
  //           duration: 3000,
  //         });
  //         this.loadBooks(); // Reload the books after deletion
  //       },
  //       error: (error) => {
  //         console.error('Error deleting book:', error);
  //         const message = 'Failed to delete book: ' + error;
  //         this.snackBar.open(message, 'Close', {
  //           duration: 3000,
  //         });
  //       },
  //     });
  //   }
  // }

  // lendBook(book: Book): void {
  //   // This would typically open a dialog for lent details
  //   // For simplicity, we'll just handle it here with a basic prompt
  //   const lentTo = prompt('Enter the name of the person borrowing the book:');
  //   if (!lentTo) return;

  //   const today = new Date().toISOString().slice(0, 10);

  //   if (book.id) {
  //     this.bookService.lendBook(book.id, lentTo, today).subscribe({
  //       next: (response) => {
  //         this.snackBar.open('Book lent successfully', 'Close', {
  //           duration: 3000,
  //         });
  //         this.loadBooks();
  //       },
  //       error: (error) => {
  //         console.error('Error lending book:', error);
  //         const message = 'Failed to lend book: ' + error;
  //         this.snackBar.open(message, 'Close', {
  //           duration: 3000,
  //         });
  //       },
  //     });
  //   }
  // }

  // returnBook(id: number): void {
  //   if (confirm('Mark this book as returned?')) {
  //     this.bookService.returnBook(id).subscribe({
  //       next: (response) => {
  //         this.snackBar.open('Book marked as returned', 'Close', {
  //           duration: 3000,
  //         });
  //         this.loadBooks();
  //       },
  //       error: (error) => {
  //         console.error('Error returning book:', error);
  //         this.snackBar.open('Failed to mark book as returned', 'Close', {
  //           duration: 3000,
  //         });
  //       },
  //     });
  //   }
  // }
}
