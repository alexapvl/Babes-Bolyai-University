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
$pdo = $database->getConnection();

// Check if connection was successful
if (!$pdo) {
    http_response_code(500);
    echo json_encode(['success' => false, 'message' => 'Database connection failed.']);
    exit;
}

// Get user from token
$user = requireAuth();

// Check if the request method is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Get the raw POST data
    $data = json_decode(file_get_contents('php://input'), true);

    // --- Data Validation & Processing ---
    $projectsList = $data['projectsList'] ?? null;
    $developerToAssign = $data['developerToAssign'] ?? null;

    // Validate Developer name
    if (!$developerToAssign) {
        echo json_encode(['success' => false, 'message' => 'Invalid or missing developer name.']);
        exit;
    }

    // check if developer exists in db
    try {
        $checkSql = "SELECT id FROM softwareDeveloper WHERE name = :name";
        $checkStmt = $pdo->prepare($checkSql);
        $checkStmt->bindParam(':name', $developerToAssign, PDO::PARAM_STR);
        $checkStmt->execute();
        
        $developer = $checkStmt->fetch(PDO::FETCH_ASSOC);
        
        if (!$developer) {
            http_response_code(404);
            echo json_encode(['success' => false, 'message' => 'Developer not found.']);
            exit;
        }
    } catch (PDOException $e) {
        error_log("Database error during ownership check: " . $e->getMessage());
        echo json_encode(['success' => false, 'message' => 'Database error occurred.']);
        exit;
    }

    try {
        // take each project from the list of projects (separated by comma) and assign it to the developer
        // if the project does not exist, create it first
        $projects = explode(',', $projectsList);
        foreach ($projects as $projectName) {
            $projectName = trim($projectName);
            if ($projectName !== '') {
                // check if project exists in db
                $checkSql = "SELECT id FROM project WHERE name = :name";
                $checkStmt = $pdo->prepare($checkSql);
                $checkStmt->bindParam(':name', $projectName, PDO::PARAM_STR);
                $checkStmt->execute();
                $existingProject = $checkStmt->fetch(PDO::FETCH_ASSOC);

                if (!$existingProject) {
                    // create project
                    $createSql = "INSERT INTO project (projectManagerId, name) VALUES (:projectManagerId, :name)";
                    $createStmt = $pdo->prepare($createSql);
                    $createStmt->bindParam(':projectManagerId', $user['id'], PDO::PARAM_INT);
                    $createStmt->bindParam(':name', $projectName, PDO::PARAM_STR);
                    $createStmt->execute();
                }
                //check if the developer is already assigned to the project with LIKE
                $checkSql = "SELECT id FROM project WHERE name = :name AND 
                          (
                            members = :developerName 
                            OR members LIKE :members_start 
                            OR members LIKE :members_middle 
                            OR members LIKE :members_end
                          )";
                $checkStmt = $pdo->prepare($checkSql);
                $checkStmt->bindParam(':name', $projectName, PDO::PARAM_STR);
                $checkStmt->bindParam(':developerName', $developerToAssign, PDO::PARAM_STR);
                $members_start = $developerToAssign . ',%';
                $members_middle = '%,' . $developerToAssign . ',%';
                $members_end = '%,' . $developerToAssign;
                $checkStmt->bindParam(':members_start', $members_start, PDO::PARAM_STR);
                $checkStmt->bindParam(':members_middle', $members_middle, PDO::PARAM_STR);
                $checkStmt->bindParam(':members_end', $members_end, PDO::PARAM_STR);
                $checkStmt->execute();
                $projectAssigned = $checkStmt->fetch(PDO::FETCH_ASSOC);
                if (!$projectAssigned) {
                    // assign the developer to the project if it is not already assigned
                    $assignSql = "UPDATE project SET members = CONCAT(COALESCE(members, ''), CASE WHEN members IS NULL OR members = '' THEN '' ELSE ',' END, :newMembers) WHERE name = :name";
                    $assignStmt = $pdo->prepare($assignSql);
                    $assignStmt->bindParam(':newMembers', $developerToAssign, PDO::PARAM_STR);
                    $assignStmt->bindParam(':name', $projectName, PDO::PARAM_STR);
                    $assignStmt->execute();
                }
                
                
            }
        }
        echo json_encode(['success' => true, 'message' => 'Projects managed successfully.']);
        exit;
    } catch (PDOException $e) {
        error_log("Database error in manage projects handler: " . $e->getMessage());
        echo json_encode(['success' => false, 'message' => 'Database error occurred.']);
    }

} else {
    // Method Not Allowed
    http_response_code(405);
    echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
}

?> 