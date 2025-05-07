import { Routes } from '@angular/router';
import { BookListComponent } from './components/book-list/book-list.component';
import { BookFormComponent } from './components/book-form/book-form.component';
import { BookDetailComponent } from './components/book-detail/book-detail.component';

export const routes: Routes = [
  { path: '', redirectTo: 'books', pathMatch: 'full' },
  { path: 'books', component: BookListComponent },
  { path: 'books/add', component: BookFormComponent },
  { path: 'books/edit/:id', component: BookFormComponent },
  { path: 'books/:id', component: BookDetailComponent },
  { path: '**', redirectTo: 'books' },
];
