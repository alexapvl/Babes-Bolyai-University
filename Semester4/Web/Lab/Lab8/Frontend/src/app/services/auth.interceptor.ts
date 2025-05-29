import { HttpInterceptorFn } from '@angular/common/http';
import { inject } from '@angular/core';
import { AuthService } from './auth.service';

export const authInterceptor: HttpInterceptorFn = (request, next) => {
  const authService = inject(AuthService);

  // Only intercept API requests
  if (!request.url.includes('/api/')) {
    return next(request);
  }

  // First try to get the token from the current user in the service
  let token: string | undefined = authService.currentUserValue?.token;

  // If no token is found or it's invalid, try to get a valid one from the cookie
  if (!token || !authService.isTokenValid(token)) {
    const userCookieStr = getCookie('currentUser');
    if (userCookieStr) {
      try {
        const userCookie = JSON.parse(userCookieStr);
        if (
          userCookie &&
          userCookie.token &&
          authService.isTokenValid(userCookie.token)
        ) {
          token = userCookie.token;
        } else {
          // Invalid token in cookie, clean it up
          document.cookie =
            'currentUser=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/; SameSite=Strict';
        }
      } catch (e) {
        console.error('Error parsing user cookie in interceptor', e);
      }
    }
  }

  // If a valid token was found, add it to the request
  if (token) {
    request = request.clone({
      setHeaders: {
        Authorization: `Bearer ${token}`,
      },
    });
  }

  return next(request);
};

// Helper method to get a cookie by name
function getCookie(name: string): string | null {
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
