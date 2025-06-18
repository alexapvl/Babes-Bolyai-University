<?php
// Set headers
header('Content-Type: application/json');
header("Access-Control-Allow-Origin: http://localhost:4200");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, Authorization");

if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    exit(0);
}

require_once '../includes/db_connect.php';
require_once '../includes/auth_helper.php';

$database = new Database();
$db = $database->getConnection();

if(!$db) {
    echo json_encode(['error' => 'Database connection failed']);
    http_response_code(500);
    exit;
}

// Get user from token
$user = requireAuth();

// Get the genre filter from the query string, default to 'all'
// $genreFilter = isset($_GET['genre']) ? $_GET['genre'] : 'all';

$query = "SELECT id, userId, totalPrice FROM Orders";

// Add genre filter if specified
// if ($genreFilter !== 'all') {
//     $query .= " AND genre = :genre";
// }
// $query .= " ORDER BY author ASC, title ASC";

$stmt = $db->prepare($query);

// Bind the genre parameter if filtering
// if ($genreFilter !== 'all') {
//     $safeGenre = htmlspecialchars(strip_tags($genreFilter));
//     $stmt->bindParam(':genre', $safeGenre);
// }

$orders = [];
try {
    $stmt->execute();
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $orders[] = $row;
    }
    echo json_encode($orders);
} catch(PDOException $e) {
    error_log("Get Orders Error: " . $e->getMessage());
    echo json_encode(['error' => 'Failed to retrieve orders. ' . $e->getMessage()]);
    http_response_code(500);
}

?> 