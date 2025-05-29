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
  selector: 'app-register',
  standalone: true,
  imports: [ReactiveFormsModule, CommonModule, RouterLink],
  template: `
    <div class="register-container">
      <h2>Register</h2>
      <form [formGroup]="registerForm" (ngSubmit)="onSubmit()">
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
              registerForm.get('username')?.invalid &&
              (registerForm.get('username')?.dirty ||
                registerForm.get('username')?.touched)
            "
            class="error-message"
          >
            <div *ngIf="registerForm.get('username')?.errors?.['required']">
              Username is required
            </div>
            <div *ngIf="registerForm.get('username')?.errors?.['minlength']">
              Username must be at least 3 characters
            </div>
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
              registerForm.get('password')?.invalid &&
              (registerForm.get('password')?.dirty ||
                registerForm.get('password')?.touched)
            "
            class="error-message"
          >
            <div *ngIf="registerForm.get('password')?.errors?.['required']">
              Password is required
            </div>
            <div *ngIf="registerForm.get('password')?.errors?.['minlength']">
              Password must be at least 6 characters
            </div>
          </div>
        </div>

        <div class="form-group">
          <button
            type="submit"
            [disabled]="registerForm.invalid || loading"
            class="btn btn-primary"
          >
            {{ loading ? 'Registering...' : 'Register' }}
          </button>
        </div>

        <div *ngIf="error" class="error-message">
          {{ error }}
        </div>

        <div class="login-link">
          Already have an account? <a routerLink="/login">Login</a>
        </div>
      </form>
    </div>
  `,
  styles: [
    `
      .register-container {
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

      .login-link {
        margin-top: 1rem;
        text-align: center;
      }
    `,
  ],
})
export class RegisterComponent {
  registerForm: FormGroup;
  loading = false;
  error = '';

  constructor(
    private formBuilder: FormBuilder,
    private router: Router,
    private authService: AuthService
  ) {
    this.registerForm = this.formBuilder.group({
      username: ['', [Validators.required, Validators.minLength(3)]],
      password: ['', [Validators.required, Validators.minLength(6)]],
    });
  }

  onSubmit() {
    if (this.registerForm.invalid) {
      return;
    }

    this.loading = true;
    this.error = '';

    this.authService.register(this.registerForm.value).subscribe({
      next: () => {
        this.router.navigate(['/books']);
      },
      error: (error) => {
        this.error =
          error.error?.message || 'Registration failed. Please try again.';
        this.loading = false;
      },
      complete: () => {
        this.loading = false;
      },
    });
  }
}
