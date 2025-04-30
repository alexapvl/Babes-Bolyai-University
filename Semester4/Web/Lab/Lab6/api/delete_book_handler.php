<?php
require_once '../includes/db_connect.php'; // Adjust path as needed

// Instantiate database connection
$database = new Database();
$pdo = $database->getConnection(); // Get the PDO connection object

// Check if connection was successful before proceeding
if (!$pdo) {
    http_response_code(500); // Internal Server Error
    echo json_encode(['success' => false, 'message' => 'Database connection failed.']);
    exit; // Stop script execution if connection failed
}

header('Content-Type: application/json');

// Check if the request method is POST (or DELETE, depending on your AJAX setup)
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Get the book ID from the request body (assuming JSON payload)
    $data = json_decode(file_get_contents('php://input'), true);
    $bookId = $data['bookId'] ?? null;

    if ($bookId && filter_var($bookId, FILTER_VALIDATE_INT)) {
        try {
            $sql = "DELETE FROM books WHERE id = :id";
            $stmt = $pdo->prepare($sql);
            $stmt->bindParam(':id', $bookId, PDO::PARAM_INT);

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