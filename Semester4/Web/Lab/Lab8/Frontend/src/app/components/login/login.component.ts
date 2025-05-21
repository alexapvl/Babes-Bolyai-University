import { Component } from '@angular/core';
import {
  FormBuilder,
  FormGroup,
  Validators,
  ReactiveFormsModule,
} from '@angular/forms';
import { Router, RouterLink } from '@angular/router';
import { AuthService } from '../../services/auth.service';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-login',
  standalone: true,
  imports: [ReactiveFormsModule, CommonModule, RouterLink],
  template: `
    <div class="login-container">
      <h2>Login</h2>
      <form [formGroup]="loginForm" (ngSubmit)="onSubmit()">
        <div class="form-group">
          <label for="username">Username</label>
          <input
            type="text"
            id="username"
            formControlName="username"
            class="form-control"
          />
          <div
            *ngIf="
              loginForm.get('username')?.invalid &&
              (loginForm.get('username')?.dirty ||
                loginForm.get('username')?.touched)
            "
            class="error-message"
          >
            Username is required
          </div>
        </div>

        <div class="form-group">
          <label for="password">Password</label>
          <input
            type="password"
            id="password"
            formControlName="password"
            class="form-control"
          />
          <div
            *ngIf="
              loginForm.get('password')?.invalid &&
              (loginForm.get('password')?.dirty ||
                loginForm.get('password')?.touched)
            "
            class="error-message"
          >
            Password is required
          </div>
        </div>

        <div class="form-group">
          <button
            type="submit"
            [disabled]="loginForm.invalid || loading"
            class="btn btn-primary"
          >
            {{ loading ? 'Logging in...' : 'Login' }}
          </button>
        </div>

        <div *ngIf="error" class="error-message">
          {{ error }}
        </div>

        <div class="register-link">
          Don't have an account? <a routerLink="/register">Register</a>
        </div>
      </form>
    </div>
  `,
  styles: [
    `
      .login-container {
        max-width: 400px;
        margin: 2rem auto;
        padding: 2rem;
        border: 1px solid #ddd;
        border-radius: 4px;
        box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
      }

      h2 {
        text-align: center;
        margin-bottom: 1.5rem;
      }

      .form-group {
        margin-bottom: 1rem;
      }

      label {
        display: block;
        margin-bottom: 0.5rem;
        font-weight: 500;
      }

      .form-control {
        width: 100%;
        padding: 0.5rem;
        border: 1px solid #ccc;
        border-radius: 4px;
      }

      button {
        width: 100%;
        padding: 0.75rem;
        background-color: #007bff;
        color: white;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        font-size: 1rem;
      }

      button:disabled {
        background-color: #cccccc;
        cursor: not-allowed;
      }

      .error-message {
        color: #dc3545;
        font-size: 0.875rem;
        margin-top: 0.25rem;
      }

      .register-link {
        margin-top: 1rem;
        text-align: center;
      }
    `,
  ],
})
export class LoginComponent {
  loginForm: FormGroup;
  loading = false;
  error = '';

  constructor(
    private formBuilder: FormBuilder,
    private router: Router,
    private authService: AuthService
  ) {
    this.loginForm = this.formBuilder.group({
      username: ['', Validators.required],
      password: ['', Validators.required],
    });
  }

  onSubmit() {
    if (this.loginForm.invalid) {
      return;
    }

    this.loading = true;
    this.error = '';

    this.authService.login(this.loginForm.value).subscribe({
      next: () => {
        this.router.navigate(['/books']);
      },
      error: (error) => {
        this.error =
          error.error?.message ||
          'Login failed. Please check your credentials.';
        this.loading = false;
      },
      complete: () => {
        this.loading = false;
      },
    });
  }
}
