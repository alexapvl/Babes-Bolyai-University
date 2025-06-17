<?php
require_once '../includes/db_connect.php';
require_once '../includes/auth_helper.php';

// Set content type to JSON
header('Content-Type: application/json');
header("Access-Control-Allow-Origin: http://localhost:4200");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, Authorization");

// Check for preflight CORS request
if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    exit(0);
}

// Enable CORS for all other requests
header('Access-Control-Allow-Origin: *');

// Instantiate the Database class and get the connection
$database = new Database();
$pdo = $database->getConnection();

// Check if connection was successful
if (!$pdo) {
    http_response_code(500);
    echo json_encode(['success' => false, 'message' => 'Database connection failed.']);
    exit;
}

// Get user from token
$user = requireAuth();

// Log the incoming data for debugging
$input_data = file_get_contents('php://input');
error_log("Received data: " . $input_data);
error_log("POST data: " . print_r($_POST, true));

// Check if the request method is POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Get the form data
    $title = filter_input(INPUT_POST, 'title', FILTER_UNSAFE_RAW);
    $author = filter_input(INPUT_POST, 'author', FILTER_UNSAFE_RAW);
    $genre = filter_input(INPUT_POST, 'genre', FILTER_UNSAFE_RAW);
    $pages = filter_input(INPUT_POST, 'pages', FILTER_VALIDATE_INT);
    
    // If form data is not found, try to get from JSON input
    if (empty($title) || empty($author)) {
        $json_data = json_decode($input_data, true);
        if ($json_data) {
            $title = $json_data['title'] ?? '';
            $author = $json_data['author'] ?? '';
            $genre = $json_data['genre'] ?? null;
            $pages = $json_data['pages'] ?? null;
        }
    }

    // Basic validation
    if (empty($title) || empty($author)) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Error: Title and Author are required.']);
        exit;
    }

    if ($pages !== null && !is_numeric($pages)) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Error: Invalid number for pages.']);
        exit;
    }
    
    // Ensure pages is null if empty or 0, or a positive integer
    $pages = ($pages !== null && $pages > 0) ? $pages : null;

    try {
        // Prepare SQL statement to prevent SQL injection - include user_id
        $sql = "INSERT INTO books (title, author, genre, pages, user_id) VALUES (:title, :author, :genre, :pages, :user_id)";
        $stmt = $pdo->prepare($sql);

        // Bind parameters
        $stmt->bindParam(':title', $title);
        $stmt->bindParam(':author', $author);
        $stmt->bindParam(':genre', $genre);
        $stmt->bindParam(':user_id', $user['id']);
        
        // Bind pages as INT or NULL
        if ($pages === null) {
             $stmt->bindValue(':pages', null, PDO::PARAM_NULL);
        } else {
             $stmt->bindParam(':pages', $pages, PDO::PARAM_INT);
        }

        // Execute the statement
        if ($stmt->execute()) {
            $book_id = $pdo->lastInsertId();
            echo json_encode([
                'success' => true, 
                'message' => 'Book added successfully.',
                'id' => $book_id
            ]);
        } else {
            http_response_code(500);
            echo json_encode(['success' => false, 'message' => 'Error: Could not add the book.']);
        }
    } catch (PDOException $e) {
        http_response_code(500);
        echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
    }

} else {
    // Not a POST request
    http_response_code(405);
    echo json_encode(['success' => false, 'message' => 'Method Not Allowed']);
}
?> 