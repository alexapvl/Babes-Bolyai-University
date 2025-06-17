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

$query = "SELECT id, name, age, skills FROM softwareDeveloper";

// get by name if name in members string
if (isset($_GET['skill']) && $_GET['skill'] !== '') {
    $skill = $_GET['skill'];
    $query .= " WHERE skills LIKE '%$skill%'";
}

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

$developers = [];
try {
    $stmt->execute();
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $developers[] = $row;
    }
    echo json_encode($developers);
} catch(PDOException $e) {
    error_log("Get Developers Error: " . $e->getMessage());
    echo json_encode(['error' => 'Failed to retrieve developers. ' . $e->getMessage()]);
    http_response_code(500);
}

?> 