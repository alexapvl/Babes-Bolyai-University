import { Routes } from '@angular/router';
import { LoginComponent } from './components/login/login.component';
import { RegisterComponent } from './components/register/register.component';
import { authGuard } from './guards/auth.guard';

export const routes: Routes = [
  { path: '', redirectTo: '/books', pathMatch: 'full' },
  { path: 'login', component: LoginComponent },
  { path: 'register', component: RegisterComponent },
  {
    path: 'books',
    loadComponent: () =>
      import('./components/book-list/book-list.component').then(
        (m) => m.BookListComponent
      ),
    canActivate: [authGuard],
  },
  {
    path: 'books/add',
    loadComponent: () =>
      import('./components/book-form/book-form.component').then(
        (m) => m.BookFormComponent
      ),
    canActivate: [authGuard],
  },
  {
    path: 'books/edit/:id',
    loadComponent: () =>
      import('./components/book-form/book-form.component').then(
        (m) => m.BookFormComponent
      ),
    canActivate: [authGuard],
  },
  {
    path: 'books/:id',
    loadComponent: () =>
      import('./components/book-detail/book-detail.component').then(
        (m) => m.BookDetailComponent
      ),
    canActivate: [authGuard],
  },
  { path: '**', redirectTo: '/books' },
];
