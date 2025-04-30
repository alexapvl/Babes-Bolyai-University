<?php include 'includes/header.php'; ?>

<h2>Add a New Book</h2>

<form id="add-book-form" action="api/add_book_handler.php" method="POST">
    <div>
        <label for="title">Title:</label>
        <input type="text" id="title" name="title" required>
    </div>
    <div>
        <label for="author">Author:</label>
        <input type="text" id="author" name="author" required>
    </div>
    <div>
        <label for="genre">Genre:</label>
        <input type="text" id="genre" name="genre">
    </div>
    <div>
        <label for="pages">Pages:</label>
        <input type="number" id="pages" name="pages" min="1">
    </div>
    <button type="submit">Add Book</button>
</form>

<div id="add-book-status">
<?php
    // Check if a status message is passed in the URL (from the handler redirect)
    if (isset($_GET['status']) && $_GET['status'] === 'success') {
        echo '<p style="color: green;">Book added successfully!</p>';
    }
    // You could add checks for error statuses here too if needed
?>
</div>

<?php include 'includes/footer.php'; ?> 