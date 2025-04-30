<?php
require_once '../includes/db_connect.php'; // Adjust path as needed

header('Content-Type: application/json');

// Instantiate database connection
$database = new Database();
$pdo = $database->getConnection();

// Check if connection was successful
if (!$pdo) {
    http_response_code(500);
    echo json_encode(['success' => false, 'message' => 'Database connection failed.']);
    exit;
}

// Check if the request method is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Get the raw POST data
    $data = json_decode(file_get_contents('php://input'), true);

    // --- Data Validation --- 
    $book_id = $data['id'] ?? null;
    $title = trim($data['title'] ?? '');
    $author = trim($data['author'] ?? '');
    $genre = trim($data['genre'] ?? '') ?: null; // Set to null if empty after trimming
    $pages = $data['pages'] ?? null;

    // Basic validation
    if (!$book_id || !filter_var($book_id, FILTER_VALIDATE_INT)) {
        echo json_encode(['success' => false, 'message' => 'Invalid or missing book ID.']);
        exit;
    }
    if (empty($title)) {
        echo json_encode(['success' => false, 'message' => 'Title cannot be empty.']);
        exit;
    }
     if (empty($author)) {
        echo json_encode(['success' => false, 'message' => 'Author cannot be empty.']);
        exit;
    }
    if ($pages !== null && (!filter_var($pages, FILTER_VALIDATE_INT) || $pages < 0)) {
        echo json_encode(['success' => false, 'message' => 'Invalid page number.']);
        exit;
    }
    // If pages is an empty string from form, it might be passed as empty string instead of null
    if ($pages === '') {
        $pages = null;
    }

    // --- Database Update --- 
    try {
        $sql = "UPDATE books 
                SET title = :title, author = :author, genre = :genre, pages = :pages 
                WHERE id = :id";
        
        $stmt = $pdo->prepare($sql);

        // Bind parameters
        $stmt->bindParam(':title', $title, PDO::PARAM_STR);
        $stmt->bindParam(':author', $author, PDO::PARAM_STR);
        $stmt->bindParam(':genre', $genre, PDO::PARAM_STR);
        // Bind pages as INT or NULL
        if ($pages === null) {
            $stmt->bindValue(':pages', null, PDO::PARAM_NULL);
        } else {
            $stmt->bindParam(':pages', $pages, PDO::PARAM_INT);
        }
        $stmt->bindParam(':id', $book_id, PDO::PARAM_INT);

        // Execute the statement
        if ($stmt->execute()) {
            // Check if any row was actually updated
            if ($stmt->rowCount() > 0) {
                echo json_encode(['success' => true, 'message' => 'Book updated successfully.']);
            } else {
                 // This can happen if the data submitted was identical to the existing data,
                 // or if the ID didn't exist (though we fetched based on ID in edit_book.php)
                 // We can consider this a success in terms of the operation completing.
                 echo json_encode(['success' => true, 'message' => 'No changes detected or book data was already up-to-date.']);
            }
        } else {
            echo json_encode(['success' => false, 'message' => 'Failed to execute update statement.']);
        }

    } catch (PDOException $e) {
        error_log("Database error: " . $e->getMessage());
        echo json_encode(['success' => false, 'message' => 'Database error occurred during update.']);
    }

} else {
    // Method Not Allowed
    http_response_code(405);
    echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
}

?> 