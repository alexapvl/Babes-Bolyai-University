<?php
require_once '../includes/db_connect.php';
require_once '../includes/auth_helper.php';

header('Content-Type: application/json');
header("Access-Control-Allow-Origin: http://localhost:4200");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, Authorization");

if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    exit(0);
}

// Instantiate database connection
$database = new Database();
$pdo = $database->getConnection(); // Get the PDO connection object

// Check if connection was successful before proceeding
if (!$pdo) {
    http_response_code(500); // Internal Server Error
    echo json_encode(['success' => false, 'message' => 'Database connection failed.']);
    exit; // Stop script execution if connection failed
}

// Get user from token
$user = requireAuth();

// Check if the request method is POST (or DELETE, depending on your AJAX setup)
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Get the book ID from the request body (assuming JSON payload)
    $data = json_decode(file_get_contents('php://input'), true);
    $bookId = $data['bookId'] ?? null;

    if ($bookId && filter_var($bookId, FILTER_VALIDATE_INT)) {
        // Check if the book belongs to the current user
        try {
            $checkSql = "SELECT user_id FROM books WHERE id = :id";
            $checkStmt = $pdo->prepare($checkSql);
            $checkStmt->bindParam(':id', $bookId, PDO::PARAM_INT);
            $checkStmt->execute();
            
            $book = $checkStmt->fetch(PDO::FETCH_ASSOC);
            
            if (!$book) {
                http_response_code(404);
                echo json_encode(['success' => false, 'message' => 'Book not found.']);
                exit;
            }
            
            if ($book['user_id'] != $user['id']) {
                http_response_code(403);
                echo json_encode(['success' => false, 'message' => 'You can only delete your own books.']);
                exit;
            }
        } catch (PDOException $e) {
            error_log("Database error during ownership check: " . $e->getMessage());
            echo json_encode(['success' => false, 'message' => 'Database error occurred.']);
            exit;
        }

        try {
            $sql = "DELETE FROM books WHERE id = :id AND user_id = :user_id";
            $stmt = $pdo->prepare($sql);
            $stmt->bindParam(':id', $bookId, PDO::PARAM_INT);
            $stmt->bindParam(':user_id', $user['id'], PDO::PARAM_INT);

            if ($stmt->execute()) {
                // Check if any row was actually deleted
                if ($stmt->rowCount() > 0) {
                    echo json_encode(['success' => true, 'message' => 'Book deleted successfully.']);
                } else {
                    echo json_encode(['success' => false, 'message' => 'Book not found or already deleted.']);
                }
            } else {
                echo json_encode(['success' => false, 'message' => 'Failed to execute delete statement.']);
            }
        } catch (PDOException $e) {
            // Log error instead of echoing sensitive information
            error_log("Database error: " . $e->getMessage());
            echo json_encode(['success' => false, 'message' => 'Database error occurred.']);
        } 
    } else {
        echo json_encode(['success' => false, 'message' => 'Invalid or missing book ID.']);
    }
} else {
    // Method Not Allowed
    http_response_code(405);
    echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
}
?> 