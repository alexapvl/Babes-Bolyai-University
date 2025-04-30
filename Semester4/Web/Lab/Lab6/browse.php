<?php 
require_once 'includes/db_connect.php';
include 'includes/header.php'; 

// Fetch distinct genres
$database = new Database();
$pdo = $database->getConnection();
$genres = [];
if ($pdo) {
    try {
        $sql = "SELECT DISTINCT genre FROM books WHERE genre IS NOT NULL AND genre != '' ORDER BY genre ASC";
        $stmt = $pdo->query($sql);
        $genres = $stmt->fetchAll(PDO::FETCH_COLUMN);
    } catch (PDOException $e) {
        // Handle error appropriately - maybe log it or display a user-friendly message
        error_log("Error fetching genres: " . $e->getMessage());
    }
}

?>

<h2>Browse Books</h2>

<div id="filter-area">
    <label for="genre-filter">Filter by Genre:</label>
    <select id="genre-filter">
        <option value="all">All Genres</option>
        <?php foreach ($genres as $genre): ?>
            <option value="<?php echo htmlspecialchars($genre); ?>"><?php echo htmlspecialchars($genre); ?></option>
        <?php endforeach; ?>
    </select>
    <button id="filter-button">Filter</button>
    <div id="last-filter-display"></div> <!-- Area to display the last used filter -->
</div>

<div id="book-list">
    <!-- Book list will be loaded here via AJAX -->
    <p>Loading books...</p> 
</div>

<!-- Modal for lending book -->
<div id="lendModal" class="modal">
  <div class="modal-content">
    <span class="close-btn" onclick="closeModal('lendModal')">&times;</span>
    <h3>Lend Book</h3>
    <form id="lendForm">
        <input type="hidden" id="lendBookId" name="bookId">
        <p>Lending: <strong id="lendBookTitle"></strong></p>
        <label for="lentTo">Lend To:</label>
        <input type="text" id="lentTo" name="lentTo" required>
        <label for="lentDate">Lend Date:</label>
        <input type="date" id="lentDate" name="lentDate" required>
        <button type="submit">Lend</button>
        <button type="button" onclick="closeModal('lendModal')">Cancel</button>
    </form>
  </div>
</div>

<?php include 'includes/footer.php'; ?> 