import { Routes } from '@angular/router';
import { BookListComponent } from './components/book-list/book-list.component';
import { BookFormComponent } from './components/book-form/book-form.component';
import { BookDetailComponent } from './components/book-detail/book-detail.component';
import { LoginComponent } from './components/login/login.component';
import { RegisterComponent } from './components/register/register.component';
import { authGuard } from './guards/auth.guard';

export const routes: Routes = [
  { path: '', redirectTo: '/books', pathMatch: 'full' },
  { path: 'login', component: LoginComponent },
  { path: 'register', component: RegisterComponent },
  { path: 'books', component: BookListComponent, canActivate: [authGuard] },
  { path: 'books/add', component: BookFormComponent, canActivate: [authGuard] },
  {
    path: 'books/edit/:id',
    component: BookFormComponent,
    canActivate: [authGuard],
  },
  {
    path: 'books/:id',
    component: BookDetailComponent,
    canActivate: [authGuard],
  },
  { path: '**', redirectTo: '/books' },
];
