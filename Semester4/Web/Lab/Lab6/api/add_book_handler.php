<?php
require_once '../includes/db_connect.php'; // Corrected path for DB connection

// Instantiate the Database class and get the connection
$database = new Database();
$pdo = $database->getConnection();

// Check if connection was successful
if (!$pdo) {
    die("Database connection failed."); // Or handle the error more gracefully
}

// Check if the request method is POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Get the form data
    $title = filter_input(INPUT_POST, 'title', FILTER_UNSAFE_RAW);
    $author = filter_input(INPUT_POST, 'author', FILTER_UNSAFE_RAW);
    $genre = filter_input(INPUT_POST, 'genre', FILTER_UNSAFE_RAW);
    $pages = filter_input(INPUT_POST, 'pages', FILTER_VALIDATE_INT);

    // Basic validation
    if (empty($title) || empty($author)) {
        // Handle error - redirect back or show error message
        // For now, we'll just exit
        die("Error: Title and Author are required.");
    }

    if ($pages === false && $pages !== null) { // Allow null pages, but not invalid numbers
        die("Error: Invalid number for pages.");
    }
    // Ensure pages is null if empty or 0, or a positive integer
    $pages = ($pages !== null && $pages > 0) ? $pages : null;


    try {
        // Prepare SQL statement to prevent SQL injection
        $sql = "INSERT INTO books (title, author, genre, pages) VALUES (:title, :author, :genre, :pages)";
        $stmt = $pdo->prepare($sql);

        // Bind parameters
        $stmt->bindParam(':title', $title);
        $stmt->bindParam(':author', $author);
        $stmt->bindParam(':genre', $genre);
        // Bind pages as INT or NULL
        if ($pages === null) {
             $stmt->bindValue(':pages', null, PDO::PARAM_NULL);
        } else {
             $stmt->bindParam(':pages', $pages, PDO::PARAM_INT);
        }


        // Execute the statement
        if ($stmt->execute()) {
            // Redirect back to the add book page or a success page with a success message
            // Example: Redirect with a success status
            header("Location: ../add_book.php?status=success");
            exit();
        } else {
            // Handle error during execution
            die("Error: Could not add the book.");
        }
    } catch (PDOException $e) {
        // Handle database errors
        die("Database error: " . $e->getMessage());
    }

} else {
    // Not a POST request, redirect or show error
    header("Location: ../add_book.php"); // Redirect back if accessed directly
    exit();
}
?> 