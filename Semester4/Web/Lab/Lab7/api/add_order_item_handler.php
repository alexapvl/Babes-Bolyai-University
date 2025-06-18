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
    $orderId = filter_input(INPUT_POST, 'orderId', FILTER_VALIDATE_INT);
    $productId = filter_input(INPUT_POST, 'productId', FILTER_VALIDATE_INT);
    
    // If form data is not found, try to get from JSON input
    if (empty($orderId) || empty($productId)) {
        $json_data = json_decode($input_data, true);
        if ($json_data) {
            $orderId = $json_data['orderId'] ?? '';
            $productId = $json_data['productId'] ?? '';
        }
    }

    // Basic validation
    if (empty($orderId) || empty($productId)) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Error: order id and product are required.']);
        exit;
    }

    try {
        // Prepare SQL statement to prevent SQL injection - include user_id
        $sql = "INSERT INTO OrderItem (orderId, productId) VALUES (:orderId, :productId)";
        $stmt = $pdo->prepare($sql);

        // Bind parameters
        $stmt->bindParam(':orderId', $orderId);
        $stmt->bindParam(':productId', $productId);

        // Execute the statement
        if ($stmt->execute()) {
            $orderItemId = $pdo->lastInsertId();
            echo json_encode([
                'success' => true, 
                'message' => 'Order Item added successfully.',
                'orderItemId' => $orderItemId
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