import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterOutlet, RouterLink, Router } from '@angular/router';
import { AuthService, User } from './services/auth.service';
import { Observable } from 'rxjs';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [CommonModule, RouterOutlet, RouterLink],
  template: `
    <header class="app-header">
      <div class="container">
        <h1 class="app-title">Personal Library</h1>
        <nav class="nav-menu">
          <ng-container *ngIf="currentUser$ | async as user; else loginLinks">
            <span class="welcome-message">Welcome, {{ user.username }}!</span>
            <button class="logout-button" (click)="logout()">Logout</button>
          </ng-container>
          <ng-template #loginLinks>
            <a routerLink="/login" class="nav-link">Login</a>
            <a routerLink="/register" class="nav-link">Register</a>
          </ng-template>
        </nav>
      </div>
    </header>

    <main class="container">
      <router-outlet></router-outlet>
    </main>
  `,
  styles: [
    `
      .app-header {
        background-color: #007bff;
        color: white;
        padding: 1rem 0;
        box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
      }

      .container {
        width: 100%;
        max-width: 1200px;
        margin: 0 auto;
        padding: 0 1rem;
      }

      .app-header .container {
        display: flex;
        justify-content: space-between;
        align-items: center;
      }

      .app-title {
        margin: 0;
        font-size: 1.5rem;
      }

      .nav-menu {
        display: flex;
        align-items: center;
      }

      .nav-link {
        color: white;
        text-decoration: none;
        margin-left: 1rem;
        padding: 0.5rem;
      }

      .nav-link:hover {
        text-decoration: underline;
      }

      .welcome-message {
        margin-right: 1rem;
      }

      .logout-button {
        background: none;
        border: 1px solid white;
        color: white;
        padding: 0.5rem 1rem;
        border-radius: 4px;
        cursor: pointer;
      }

      .logout-button:hover {
        background-color: rgba(255, 255, 255, 0.1);
      }

      main.container {
        padding-top: 2rem;
        padding-bottom: 2rem;
      }
    `,
  ],
})
export class AppComponent implements OnInit {
  currentUser$: Observable<User | null>;

  constructor(private authService: AuthService, private router: Router) {
    this.currentUser$ = this.authService.currentUser$;
  }

  ngOnInit() {
    // Any initialization logic can go here
  }

  logout() {
    this.authService.logout();
    this.router.navigate(['/login']);
  }
}
