<?php
header("Access-Control-Allow-Origin: http://localhost:4200");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, Authorization");
header("Content-Type: application/json");

if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    exit(0);
}

// Include database connection
require_once '../includes/db_connect.php';

// Get database connection
$database = new Database();
$db = $database->getConnection();

if (!$db) {
    echo json_encode(['success' => false, 'message' => 'Database connection failed']);
    http_response_code(500);
    exit;
}

// Get the request method and path
$method = $_SERVER['REQUEST_METHOD'];
$path = $_SERVER['REQUEST_URI'];

// Parse the path to get the action
$pathParts = explode('/', trim($path, '/'));
$action = end($pathParts);

try {
    switch ($method) {
        case 'POST':
            if (strpos($path, '/login') !== false) {
                handleLogin($db);
            } elseif (strpos($path, '/register') !== false) {
                handleRegister($db);
            } else {
                http_response_code(404);
                echo json_encode(['success' => false, 'message' => 'Endpoint not found']);
            }
            break;
        case 'GET':
            if (strpos($path, '/validate') !== false) {
                handleValidateToken();
            } elseif (strpos($path, '/check-username/') !== false) {
                $username = end($pathParts);
                handleCheckUsername($db, $username);
            } else {
                http_response_code(404);
                echo json_encode(['success' => false, 'message' => 'Endpoint not found']);
            }
            break;
        default:
            http_response_code(405);
            echo json_encode(['success' => false, 'message' => 'Method not allowed']);
            break;
    }
} catch (Exception $e) {
    http_response_code(500);
    echo json_encode(['success' => false, 'message' => 'Internal server error: ' . $e->getMessage()]);
}

function handleLogin($db) {
    $input = json_decode(file_get_contents('php://input'), true);
    
    if (!isset($input['username']) || !isset($input['password'])) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Username and password are required']);
        return;
    }
    
    $username = $input['username'];
    $password = $input['password'];
    
    // Find user by username
    $stmt = $db->prepare("SELECT id, username, password FROM users WHERE username = :username");
    $stmt->bindParam(':username', $username);
    $stmt->execute();
    
    $user = $stmt->fetch(PDO::FETCH_ASSOC);
    
    if (!$user) {
        http_response_code(401);
        echo json_encode(['success' => false, 'message' => 'Invalid username or password']);
        return;
    }
    
    // Check password (plain text comparison as requested)
    if ($password !== $user['password']) {
        http_response_code(401);
        echo json_encode(['success' => false, 'message' => 'Invalid username or password']);
        return;
    }
    
    // Generate a simple token (user_id:username:timestamp)
    $token = generateToken($user['id'], $user['username']);
    
    echo json_encode([
        'success' => true,
        'message' => 'Login successful',
        'token' => $token,
        'userId' => $user['id'],
        'username' => $user['username']
    ]);
}

function handleRegister($db) {
    $input = json_decode(file_get_contents('php://input'), true);
    
    if (!isset($input['username']) || !isset($input['password'])) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Username and password are required']);
        return;
    }
    
    $username = $input['username'];
    $password = $input['password'];
    
    // Validate input
    if (strlen($username) < 3) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Username must be at least 3 characters']);
        return;
    }
    
    if (strlen($password) < 6) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Password must be at least 6 characters']);
        return;
    }
    
    // Check if user already exists
    $stmt = $db->prepare("SELECT id FROM users WHERE username = :username");
    $stmt->bindParam(':username', $username);
    $stmt->execute();
    
    if ($stmt->fetch(PDO::FETCH_ASSOC)) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Username already exists']);
        return;
    }
    
    // Create new user (store password as plain text as requested)
    $stmt = $db->prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    $stmt->bindParam(':username', $username);
    $stmt->bindParam(':password', $password);
    
    if ($stmt->execute()) {
        $userId = $db->lastInsertId();
        
        // Generate token
        $token = generateToken($userId, $username);
        
        echo json_encode([
            'success' => true,
            'message' => 'Registration successful',
            'token' => $token,
            'userId' => $userId,
            'username' => $username
        ]);
    } else {
        http_response_code(500);
        echo json_encode(['success' => false, 'message' => 'Failed to create user']);
    }
}

function handleValidateToken() {
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
        http_response_code(401);
        echo json_encode(['isValid' => false]);
        return;
    }
    
    $token = substr($authHeader, 7);
    
    if (validateToken($token)) {
        echo json_encode(['isValid' => true]);
    } else {
        http_response_code(401);
        echo json_encode(['isValid' => false]);
    }
}

function handleCheckUsername($db, $username) {
    $stmt = $db->prepare("SELECT id FROM users WHERE username = :username");
    $stmt->bindParam(':username', $username);
    $stmt->execute();
    
    echo json_encode(['exists' => $stmt->fetch(PDO::FETCH_ASSOC) !== false]);
}

function generateToken($userId, $username) {
    $timestamp = time();
    $data = $userId . ':' . $username . ':' . $timestamp;
    return base64_encode($data);
}

function validateToken($token) {
    try {
        $decoded = base64_decode($token);
        $parts = explode(':', $decoded);
        
        if (count($parts) != 3) {
            return false;
        }
        
        $userId = $parts[0];
        $username = $parts[1];
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
?> 