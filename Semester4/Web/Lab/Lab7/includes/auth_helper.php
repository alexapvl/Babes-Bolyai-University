<?php

function getUserFromAuthHeader() {
    // Try multiple ways to get headers for better compatibility
    $authHeader = '';
    
    // Method 1: getallheaders() if available
    if (function_exists('getallheaders')) {
        $headers = getallheaders();
        // Check both cases for Authorization header
        $authHeader = isset($headers['Authorization']) ? $headers['Authorization'] : 
                     (isset($headers['authorization']) ? $headers['authorization'] : '');
    }
    
    // Method 2: Check $_SERVER if getallheaders didn't work
    if (empty($authHeader)) {
        if (isset($_SERVER['HTTP_AUTHORIZATION'])) {
            $authHeader = $_SERVER['HTTP_AUTHORIZATION'];
        } elseif (isset($_SERVER['REDIRECT_HTTP_AUTHORIZATION'])) {
            $authHeader = $_SERVER['REDIRECT_HTTP_AUTHORIZATION'];
        }
    }
    
    // PHP 8.0+ has str_starts_with, for older versions use substr
    $hasBearer = (function_exists('str_starts_with')) ? 
        str_starts_with($authHeader, 'Bearer ') : 
        (substr($authHeader, 0, 7) === 'Bearer ');
    
    if (empty($authHeader) || !$hasBearer) {
        return null;
    }
    
    $token = substr($authHeader, 7);
    
    if (!validateToken($token)) {
        return null;
    }
    
    return getUserFromToken($token);
}

function validateToken($token) {
    try {
        $decoded = base64_decode($token);
        $parts = explode(':', $decoded);
        
        if (count($parts) != 3) {
            return false;
        }
        
        $timestamp = $parts[2];
        
        // Check if token is not too old (24 hours)
        $maxAge = 24 * 60 * 60; // 24 hours in seconds
        if (time() - $timestamp > $maxAge) {
            return false;
        }
        
        return true;
    } catch (Exception $e) {
        return false;
    }
}

function getUserFromToken($token) {
    try {
        $decoded = base64_decode($token);
        $parts = explode(':', $decoded);
        
        if (count($parts) != 3) {
            return null;
        }
        
        return [
            'id' => $parts[0],
            'username' => $parts[1],
            'timestamp' => $parts[2]
        ];
    } catch (Exception $e) {
        return null;
    }
}

function requireAuth() {
    $user = getUserFromAuthHeader();
    if (!$user) {
        http_response_code(401);
        echo json_encode(['success' => false, 'message' => 'Unauthorized']);
        exit;
    }
    return $user;
}

?> 