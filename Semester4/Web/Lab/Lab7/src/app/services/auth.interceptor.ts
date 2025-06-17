import { HttpInterceptorFn } from '@angular/common/http';
import { inject } from '@angular/core';
import { AuthService } from './auth.service';

export const authInterceptor: HttpInterceptorFn = (req, next) => {
  const authService = inject(AuthService);
  const currentUser = authService.currentUserValue;

  console.log('Auth Interceptor - URL:', req.url);
  console.log(
    'Auth Interceptor - Current User:',
    currentUser ? 'EXISTS' : 'NULL'
  );

  if (currentUser && currentUser.token) {
    console.log('Auth Interceptor - Adding Authorization header with token');
    // Clone the request and add the authorization header
    const authReq = req.clone({
      headers: req.headers.set('Authorization', `Bearer ${currentUser.token}`),
    });
    return next(authReq);
  }

  console.log(
    'Auth Interceptor - No token available, proceeding without auth header'
  );
  return next(req);
};
