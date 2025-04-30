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

    // --- Data Validation & Processing ---
    $action = $data['action'] ?? null;
    $bookId = $data['bookId'] ?? null;

    // Validate Book ID
    if (!$bookId || !filter_var($bookId, FILTER_VALIDATE_INT)) {
        echo json_encode(['success' => false, 'message' => 'Invalid or missing book ID.']);
        exit;
    }

    try {
        if ($action === 'lend') {
            $lentTo = trim($data['lentTo'] ?? '');
            $lentDate = $data['lentDate'] ?? null;

            // Validate Lend Data
            if (empty($lentTo)) {
                echo json_encode(['success' => false, 'message' => "'Lend To' name cannot be empty."]);
                exit;
            }
            if (empty($lentDate) || !DateTime::createFromFormat('Y-m-d', $lentDate)) {
                 echo json_encode(['success' => false, 'message' => 'Invalid or missing lend date. Please use YYYY-MM-DD format.']);
                 exit;
            }
            
            // Optional: Check if the book is already lent out before trying to lend it again
            // $checkSql = "SELECT lent_to FROM books WHERE id = :id";
            // $checkStmt = $pdo->prepare($checkSql);
            // $checkStmt->bindParam(':id', $bookId, PDO::PARAM_INT);
            // $checkStmt->execute();
            // $currentStatus = $checkStmt->fetchColumn();
            // if ($currentStatus !== null) {
            //     echo json_encode(['success' => false, 'message' => 'This book is already lent out.']);
            //     exit;
            // }


            // Update book status to lent
            $sql = "UPDATE books SET lent_to = :lentTo, lent_date = :lentDate WHERE id = :id";
            $stmt = $pdo->prepare($sql);
            $stmt->bindParam(':lentTo', $lentTo, PDO::PARAM_STR);
            $stmt->bindParam(':lentDate', $lentDate, PDO::PARAM_STR);
            $stmt->bindParam(':id', $bookId, PDO::PARAM_INT);

            if ($stmt->execute()) {
                if ($stmt->rowCount() > 0) {
                    echo json_encode(['success' => true, 'message' => 'Book marked as lent.']);
                } else {
                     echo json_encode(['success' => false, 'message' => 'Book not found or could not be updated.']); // Should not happen if ID is valid
                }
            } else {
                echo json_encode(['success' => false, 'message' => 'Failed to update book status (lend).']);
            }

        } elseif ($action === 'return') {
            // Update book status to available (NULL values)
            $sql = "UPDATE books SET lent_to = NULL, lent_date = NULL WHERE id = :id";
            $stmt = $pdo->prepare($sql);
            $stmt->bindParam(':id', $bookId, PDO::PARAM_INT);

            if ($stmt->execute()) {
                 if ($stmt->rowCount() > 0) {
                    echo json_encode(['success' => true, 'message' => 'Book marked as returned/available.']);
                 } else {
                     // This might happen if the book was already marked as returned
                     echo json_encode(['success' => true, 'message' => 'Book was already marked as available.']);
                 }
            } else {
                echo json_encode(['success' => false, 'message' => 'Failed to update book status (return).']);
            }

        } else {
            // Invalid action
            echo json_encode(['success' => false, 'message' => 'Invalid action specified.']);
        }

    } catch (PDOException $e) {
        error_log("Database error in lend/return handler: " . $e->getMessage());
        echo json_encode(['success' => false, 'message' => 'Database error occurred.']);
    }

} else {
    // Method Not Allowed
    http_response_code(405);
    echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
}

?> 