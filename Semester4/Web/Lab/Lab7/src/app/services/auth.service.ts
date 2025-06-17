import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { BehaviorSubject, Observable, tap } from 'rxjs';

export interface User {
  id: number;
  username: string;
  token: string;
}

export interface LoginRequest {
  username: string;
  password: string;
}

export interface RegisterRequest {
  username: string;
  password: string;
}

export interface AuthResponse {
  success: boolean;
  message: string;
  token?: string;
  userId?: number;
  username?: string;
}

@Injectable({
  providedIn: 'root',
})
export class AuthService {
  private apiUrl = '/api/auth_handler.php';
  private currentUserSubject = new BehaviorSubject<User | null>(null);
  public currentUser$ = this.currentUserSubject.asObservable();

  constructor(private http: HttpClient) {
    // Load user from cookie on service initialization
    const userCookie = this.getCookie('currentUser');
    console.log('AuthService init - found cookie:', userCookie ? 'YES' : 'NO');

    if (userCookie) {
      try {
        const user = JSON.parse(userCookie);
        console.log('AuthService init - parsed user:', user);

        if (user && this.isTokenValid(user.token)) {
          console.log(
            'AuthService init - token is valid, setting current user'
          );
          this.currentUserSubject.next(user);
        } else {
          console.log(
            'AuthService init - token is invalid or expired, cleaning up'
          );
          // Token is invalid or expired, clean up
          this.deleteCookie('currentUser');
        }
      } catch (e) {
        console.error('AuthService init - error parsing user cookie', e);
        this.deleteCookie('currentUser');
      }
    }
  }

  public get currentUserValue(): User | null {
    return this.currentUserSubject.value;
  }

  login(credentials: LoginRequest): Observable<AuthResponse> {
    return this.http
      .post<AuthResponse>(`${this.apiUrl}/login`, credentials)
      .pipe(
        tap((response) => {
          if (
            response.success &&
            response.token &&
            response.userId &&
            response.username
          ) {
            const user: User = {
              id: response.userId,
              username: response.username,
              token: response.token,
            };
            this.setCookie('currentUser', JSON.stringify(user), 1); // 1 day expiration
            this.currentUserSubject.next(user);
          }
        })
      );
  }

  register(user: RegisterRequest): Observable<AuthResponse> {
    return this.http.post<AuthResponse>(`${this.apiUrl}/register`, user).pipe(
      tap((response) => {
        if (
          response.success &&
          response.token &&
          response.userId &&
          response.username
        ) {
          const user: User = {
            id: response.userId,
            username: response.username,
            token: response.token,
          };
          this.setCookie('currentUser', JSON.stringify(user), 1); // 1 day expiration
          this.currentUserSubject.next(user);
        }
      })
    );
  }

  logout(): void {
    this.deleteCookie('currentUser');
    this.currentUserSubject.next(null);
  }

  validateToken(): Observable<{ isValid: boolean }> {
    return this.http.get<{ isValid: boolean }>(`${this.apiUrl}/validate`);
  }

  // Check if a username exists
  checkUsernameExists(username: string): Observable<{ exists: boolean }> {
    return this.http.get<{ exists: boolean }>(
      `${this.apiUrl}/check-username/${username}`
    );
  }

  // Check if a token is valid (not expired)
  isTokenValid(token: string): boolean {
    if (!token) return false;

    try {
      // Our token is base64 encoded userId:username:timestamp
      const decoded = atob(token);
      const parts = decoded.split(':');

      if (parts.length !== 3) return false;

      const timestamp = parseInt(parts[2]);
      const maxAge = 24 * 60 * 60 * 1000; // 24 hours in milliseconds

      return Date.now() - timestamp * 1000 < maxAge;
    } catch (e) {
      console.error('Error validating token', e);
      return false;
    }
  }

  // Cookie management functions
  private setCookie(name: string, value: string, days: number): void {
    const date = new Date();
    date.setTime(date.getTime() + days * 24 * 60 * 60 * 1000);
    const expires = '; expires=' + date.toUTCString();
    document.cookie =
      name +
      '=' +
      encodeURIComponent(value) +
      expires +
      '; path=/; SameSite=Strict';
  }

  private getCookie(name: string): string | null {
    const nameEQ = name + '=';
    const ca = document.cookie.split(';');
    for (let i = 0; i < ca.length; i++) {
      let c = ca[i];
      while (c.charAt(0) === ' ') c = c.substring(1, c.length);
      if (c.indexOf(nameEQ) === 0)
        return decodeURIComponent(c.substring(nameEQ.length, c.length));
    }
    return null;
  }

  private deleteCookie(name: string): void {
    document.cookie =
      name +
      '=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/; SameSite=Strict';
  }
}
