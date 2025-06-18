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
    $userId = filter_input(INPUT_POST, 'userId', FILTER_VALIDATE_INT);
    $totalPrice = filter_input(INPUT_POST, 'totalPrice', FILTER_VALIDATE_FLOAT);
    
    // If form data is not found, try to get from JSON input
    if (empty($userId) || empty($totalPrice)) {
        $json_data = json_decode($input_data, true);
        if ($json_data) {
            $userId = $json_data['userId'] ?? '';
            $totalPrice = $json_data['totalPrice'] ?? '';
        }
    }

    // Basic validation
    if (empty($userId) || empty($totalPrice)) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Error: User ID and Total Price are required.']);
        exit;
    }

    try {
        // Prepare SQL statement to prevent SQL injection - include user_id
        $sql = "INSERT INTO Orders (userId, totalPrice) VALUES (:userId, :totalPrice)";
        $stmt = $pdo->prepare($sql);

        // Bind parameters
        $stmt->bindParam(':userId', $userId);
        $stmt->bindParam(':totalPrice', $totalPrice);

        // Execute the statement
        if ($stmt->execute()) {
            $orderId = $pdo->lastInsertId();
            echo json_encode([
                'success' => true, 
                'message' => 'Order added successfully.',
                'orderId' => $orderId
            ]);
        } else {
            http_response_code(500);
            echo json_encode(['success' => false, 'message' => 'Error: Could not add the order.']);
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