<?php
// Set header to return JSON content
header('Content-Type: application/json');

require_once '../includes/db_connect.php'; // Adjust path as needed

$database = new Database();
$db = $database->getConnection();

if(!$db) {
    echo json_encode(['error' => 'Database connection failed']);
    http_response_code(500); // Internal Server Error
    exit;
}

// Get the genre filter from the query string, default to 'all'
$genreFilter = isset($_GET['genre']) ? $_GET['genre'] : 'all';

$query = "SELECT id, title, author, genre, pages, lent_to, lent_date FROM books";

// Prepare the statement
if ($genreFilter !== 'all') {
    $query .= " WHERE genre = :genre";
}
$query .= " ORDER BY author ASC, title ASC";

$stmt = $db->prepare($query);

// Bind the parameter if filtering
if ($genreFilter !== 'all') {
    // Sanitize genre input slightly - more robust validation could be added
    $safeGenre = htmlspecialchars(strip_tags($genreFilter));
    $stmt->bindParam(':genre', $safeGenre);
}

$books = [];
try {
    $stmt->execute();
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $books[] = $row;
    }
    echo json_encode($books);
} catch(PDOException $e) {
    // Log error instead of echoing in production
    error_log("Get Books Error: " . $e->getMessage());
    echo json_encode(['error' => 'Failed to retrieve books. ' . $e->getMessage()]); // Provide specific error in dev
    http_response_code(500);
}

?> 