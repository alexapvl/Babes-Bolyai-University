import { Injectable } from '@angular/core';
import {
  HttpClient,
  HttpErrorResponse,
  HttpHeaders,
} from '@angular/common/http';
import { Observable, catchError, tap, throwError } from 'rxjs';
import { Project } from '../models/project.model';

@Injectable({
  providedIn: 'root',
})
export class ProjectService {
  // Base URL to the PHP API
  private apiUrl = '/api';

  constructor(private http: HttpClient) {}

  // Get all books, optionally filtered by genre
  getProjects(): Observable<Project[]> {
    return this.http
      .get<Project[]>(`${this.apiUrl}/get_projects.php`)
      .pipe(catchError(this.handleError));
  }

  getMyProjects(memberName: string): Observable<Project[]> {
    if (memberName === '') {
      return this.getProjects();
    }
    return this.http
      .get<Project[]>(
        `${this.apiUrl}/get_projects.php?memberName=${memberName}`
      )
      .pipe(catchError(this.handleError));
  }

  manageProjects(
    projectsList: string,
    developerToAssign: string
  ): Observable<any> {
    const payload = {
      projectsList,
      developerToAssign,
    };

    const headers = new HttpHeaders({
      'Content-Type': 'application/json',
    });

    return this.http
      .post(`${this.apiUrl}/manage_projects_handler.php`, payload, { headers })
      .pipe(catchError(this.handleError));
  }

  // Error handling method
  private handleError(error: HttpErrorResponse) {
    let errorMessage = 'Unknown error occurred';

    if (error.error instanceof ErrorEvent) {
      // Client-side error
      errorMessage = `Error: ${error.error.message}`;
    } else {
      // Server-side error
      errorMessage = `Error Code: ${error.status}\nMessage: ${error.message}`;

      // Try to parse error response if it's JSON
      if (typeof error.error === 'object') {
        try {
          const errorObj = error.error;
          errorMessage += `\nServer response: ${
            errorObj.message || JSON.stringify(errorObj)
          }`;
        } catch (e) {
          console.error('Could not parse error response:', e);
        }
      } else if (typeof error.error === 'string') {
        errorMessage += `\nServer response: ${error.error}`;
      }
    }

    console.error(errorMessage);
    return throwError(() => new Error(errorMessage));
  }
}
