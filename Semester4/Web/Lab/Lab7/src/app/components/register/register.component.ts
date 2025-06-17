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
import { MatCardModule } from '@angular/material/card';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatInputModule } from '@angular/material/input';
import { MatButtonModule } from '@angular/material/button';
import { MatProgressSpinnerModule } from '@angular/material/progress-spinner';

@Component({
  selector: 'app-register',
  standalone: true,
  imports: [
    ReactiveFormsModule,
    CommonModule,
    RouterLink,
    MatCardModule,
    MatFormFieldModule,
    MatInputModule,
    MatButtonModule,
    MatProgressSpinnerModule,
  ],
  template: `
    <div class="register-container">
      <mat-card class="register-card">
        <mat-card-header>
          <mat-card-title>Register</mat-card-title>
        </mat-card-header>
        <mat-card-content>
          <form [formGroup]="registerForm" (ngSubmit)="onSubmit()">
            <mat-form-field appearance="outline" class="full-width">
              <mat-label>Username</mat-label>
              <input
                matInput
                type="text"
                formControlName="username"
                placeholder="Enter your username"
              />
              <mat-error
                *ngIf="registerForm.get('username')?.hasError('required')"
              >
                Username is required
              </mat-error>
              <mat-error
                *ngIf="registerForm.get('username')?.hasError('minlength')"
              >
                Username must be at least 3 characters
              </mat-error>
            </mat-form-field>

            <mat-form-field appearance="outline" class="full-width">
              <mat-label>Password</mat-label>
              <input
                matInput
                type="password"
                formControlName="password"
                placeholder="Enter your password"
              />
              <mat-error
                *ngIf="registerForm.get('password')?.hasError('required')"
              >
                Password is required
              </mat-error>
              <mat-error
                *ngIf="registerForm.get('password')?.hasError('minlength')"
              >
                Password must be at least 6 characters
              </mat-error>
            </mat-form-field>

            <div class="form-actions">
              <button
                mat-raised-button
                color="primary"
                type="submit"
                [disabled]="registerForm.invalid || loading"
                class="full-width"
              >
                <mat-spinner
                  *ngIf="loading"
                  diameter="20"
                  class="spinner"
                ></mat-spinner>
                {{ loading ? 'Registering...' : 'Register' }}
              </button>
            </div>

            <div *ngIf="error" class="error-message">
              {{ error }}
            </div>

            <div class="login-link">
              Already have an account?
              <a routerLink="/login" mat-button color="primary">Login</a>
            </div>
          </form>
        </mat-card-content>
      </mat-card>
    </div>
  `,
  styles: [
    `
      .register-container {
        display: flex;
        justify-content: center;
        align-items: center;
        min-height: 80vh;
        padding: 2rem;
      }

      .register-card {
        width: 100%;
        max-width: 400px;
        padding: 1rem;
      }

      .full-width {
        width: 100%;
        margin-bottom: 1rem;
      }

      .form-actions {
        margin-top: 1rem;
        margin-bottom: 1rem;
      }

      .spinner {
        margin-right: 0.5rem;
      }

      .error-message {
        color: #f44336;
        font-size: 0.875rem;
        margin-top: 0.5rem;
        padding: 0.5rem;
        background-color: #ffebee;
        border-radius: 4px;
        border-left: 4px solid #f44336;
      }

      .login-link {
        text-align: center;
        margin-top: 1rem;
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
