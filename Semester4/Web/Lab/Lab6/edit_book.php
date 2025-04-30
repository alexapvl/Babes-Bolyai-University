<?php
require_once 'includes/db_connect.php';
include 'includes/header.php'; 

$book_id = $_GET['id'] ?? null;
$book = null;
$error_message = '';

if ($book_id && filter_var($book_id, FILTER_VALIDATE_INT)) {
    try {
        $database = new Database();
        $pdo = $database->getConnection();

        if ($pdo) {
            $sql = "SELECT id, title, author, genre, pages FROM books WHERE id = :id";
            $stmt = $pdo->prepare($sql);
            $stmt->bindParam(':id', $book_id, PDO::PARAM_INT);
            $stmt->execute();
            $book = $stmt->fetch(PDO::FETCH_ASSOC);

            if (!$book) {
                $error_message = "Book not found.";
            }
        } else {
            $error_message = "Database connection failed.";
        }
    } catch (PDOException $e) {
        error_log("Database error: " . $e->getMessage());
        $error_message = "An error occurred while fetching book details.";
    }
} else {
    $error_message = "Invalid or missing book ID.";
}

?>

<h2>Edit Book</h2>

<?php if ($error_message): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error_message); ?></p>
    <p><a href="browse.php">Back to Browse</a></p>
<?php elseif ($book): ?>
    <form id="edit-book-form">
        <input type="hidden" name="id" value="<?php echo htmlspecialchars($book['id']); ?>">
        
        <div>
            <label for="title">Title:</label>
            <input type="text" id="title" name="title" value="<?php echo htmlspecialchars($book['title']); ?>" required>
        </div>
        
        <div>
            <label for="author">Author:</label>
            <input type="text" id="author" name="author" value="<?php echo htmlspecialchars($book['author']); ?>" required>
        </div>

        <div>
            <label for="genre">Genre:</label>
            <input type="text" id="genre" name="genre" value="<?php echo htmlspecialchars($book['genre'] ?? ''); ?>">
        </div>

        <div>
            <label for="pages">Pages:</label>
            <input type="number" id="pages" name="pages" value="<?php echo htmlspecialchars($book['pages'] ?? ''); ?>">
        </div>

        <button type="submit">Save Changes</button>
        <a href="browse.php">Cancel</a> 
    </form>
    <div id="edit-message" style="margin-top: 1em;"></div>
<?php endif; ?>

<script>
document.addEventListener('DOMContentLoaded', () => {
    const editForm = document.getElementById('edit-book-form');
    const messageDiv = document.getElementById('edit-message');

    if (editForm) {
        editForm.addEventListener('submit', async (event) => {
            event.preventDefault(); 
            messageDiv.textContent = ''; // Clear previous messages
            messageDiv.style.color = 'black';

            const formData = new FormData(editForm);
            // Convert FormData to a plain object for JSON stringification
            const bookData = {};
            formData.forEach((value, key) => {
                 // Handle potential empty string for pages, convert to null if necessary
                 if (key === 'pages' && value === '') {
                    bookData[key] = null;
                 } else {
                    bookData[key] = value; 
                 }
            });

            try {
                const response = await fetch('api/edit_book_handler.php', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(bookData)
                });

                const result = await response.json();

                if (response.ok && result.success) {
                    messageDiv.textContent = 'Book updated successfully!';
                    messageDiv.style.color = 'green';
                    // Optionally redirect after a short delay
                    // setTimeout(() => { window.location.href = 'browse.php'; }, 1500);
                } else {
                    throw new Error(result.message || 'Failed to update book.');
                }

            } catch (error) {
                console.error('Update error:', error);
                messageDiv.textContent = `Error updating book: ${error.message}`;
                messageDiv.style.color = 'red';
            }
        });
    }
});

// Utility function to prevent basic XSS (if needed elsewhere, consider moving to main.js)
function escapeHTML(str) {
    if (str === null || str === undefined) return '';
    return String(str).replace(/[&<>\'"/]/g, function (s) {
        const entityMap = {
            "&": "&amp;",
            "<": "&lt;",
            ">": "&gt;",
            '"': '&quot;',
            "'": '&#39;',
            "/": '&#x2F;'
        };
        return entityMap[s];
    });
}

</script>

<?php include 'includes/footer.php'; ?> 