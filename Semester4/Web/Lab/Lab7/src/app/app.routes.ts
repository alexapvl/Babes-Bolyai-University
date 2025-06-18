import { Routes } from '@angular/router';
import { BookFormComponent } from './components/book-form/book-form.component';
import { BookDetailComponent } from './components/book-detail/book-detail.component';
import { LoginComponent } from './components/login/login.component';
import { RegisterComponent } from './components/register/register.component';
import { authGuard } from './guards/auth.guard';
import { ProjectListComponent } from './components/project-list/project-list.component';
import { MyProjectListComponent } from './components/my-project-list/my-project-list.component';
import { DeveloperListComponent } from './components/developer-list/developer-list.component';
import { ManageProjectsFormComponent } from './components/manage-projects-form/manage-projects-form.component';

export const routes: Routes = [
  { path: '', redirectTo: '/projects', pathMatch: 'full' },
  { path: 'login', component: LoginComponent },
  { path: 'register', component: RegisterComponent },
  {
    path: 'projects',
    component: ProjectListComponent,
    canActivate: [authGuard],
  },
  {
    path: 'my-projects',
    component: MyProjectListComponent,
    canActivate: [authGuard],
  },
  {
    path: 'developers',
    component: DeveloperListComponent,
    canActivate: [authGuard],
  },
  {
    path: 'manage-projects',
    component: ManageProjectsFormComponent,
    canActivate: [authGuard],
  },
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
  { path: '**', redirectTo: '/projects' },
];
