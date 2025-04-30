document.addEventListener('DOMContentLoaded', () => {
    // Only run book fetching logic if we are on the browse page and the elements exist
    const bookListDiv = document.getElementById('book-list');
    const filterButton = document.getElementById('filter-button');
    const genreFilterSelect = document.getElementById('genre-filter');
    const lastFilterDisplay = document.getElementById('last-filter-display');

    if (bookListDiv && filterButton && genreFilterSelect && lastFilterDisplay) {
        // Initial fetch of all books when the page loads
        fetchBooks('all');

        // Event listener for the filter button
        filterButton.addEventListener('click', () => {
            const selectedGenre = genreFilterSelect.value;
            fetchBooks(selectedGenre);
        });
    }

    // --- Event delegation for dynamic buttons (Edit/Delete/Lend) ---
    if (bookListDiv) {
        bookListDiv.addEventListener('click', handleBookActionClick);
    }

     // --- Modal Handling --- 
    const lendForm = document.getElementById('lendForm');
    if(lendForm) {
        lendForm.addEventListener('submit', handleLendSubmit);
    }
});

// Function to fetch books from the API
async function fetchBooks(genre = 'all') {
    const bookListDiv = document.getElementById('book-list');
    const lastFilterDisplay = document.getElementById('last-filter-display');
    bookListDiv.innerHTML = '<p>Loading books...</p>'; // Show loading state

    try {
        const response = await fetch(`api/get_books.php?genre=${encodeURIComponent(genre)}`);
        if (!response.ok) {
            // Attempt to read error message from API if available
            let errorMsg = `HTTP error! Status: ${response.status}`;
            try {
                 const errorData = await response.json();
                 if(errorData.error) {
                    errorMsg += ` - ${errorData.error}`;
                 }
            } catch(e) { /* Ignore if response wasn't JSON */ }
            throw new Error(errorMsg);
        }
        const books = await response.json();
        displayBooks(books);

        // Update and display the last filter used
        const genreText = genre === 'all' ? 'All Genres' : document.querySelector(`#genre-filter option[value="${genre}"]`).textContent;
        lastFilterDisplay.textContent = `Filtered by: ${genreText}`;

    } catch (error) {
        console.error('Fetch error:', error);
        bookListDiv.innerHTML = `<p style="color: red;">Error loading books: ${error.message}. Please check connection or API endpoint.</p>`;
        lastFilterDisplay.textContent = 'Error applying filter.';
    }
}

// Function to display books in the HTML table
function displayBooks(books) {
    const bookListDiv = document.getElementById('book-list');
    bookListDiv.innerHTML = ''; // Clear previous list or loading message

    if (!books || books.length === 0) {
        bookListDiv.innerHTML = '<p>No books found matching the criteria.</p>';
        return;
    }

    const table = document.createElement('table');
    table.innerHTML = `
        <thead>
            <tr>
                <th>Title</th>
                <th>Author</th>
                <th>Genre</th>
                <th>Pages</th>
                <th>Status</th>
                <th>Actions</th>
            </tr>
        </thead>
        <tbody>
        </tbody>
    `;

    const tbody = table.querySelector('tbody');
    books.forEach(book => {
        const tr = document.createElement('tr');
        tr.setAttribute('data-id', book.id);

        const lentStatus = book.lent_to ? `Lent to ${book.lent_to} on ${book.lent_date}` : 'Available';
        const lendButtonText = book.lent_to ? 'Return' : 'Lend';
        const lendButtonClass = book.lent_to ? 'return-btn action-btn' : 'lend-btn action-btn'; // Add return-btn class if needed

        tr.innerHTML = `
            <td>${escapeHTML(book.title)}</td>
            <td>${escapeHTML(book.author)}</td>
            <td>${escapeHTML(book.genre) || 'N/A'}</td>
            <td>${book.pages || 'N/A'}</td>
            <td>${escapeHTML(lentStatus)}</td>
            <td>
                <button class="edit-btn action-btn" data-id="${book.id}">Edit</button>
                <button class="delete-btn action-btn" data-id="${book.id}">Delete</button>
                <button class="${lendButtonClass}" data-id="${book.id}" data-title="${escapeHTML(book.title)}">${lendButtonText}</button>
            </td>
        `;
        tbody.appendChild(tr);
    });

    bookListDiv.appendChild(table);
}

// --- Placeholder functions for actions (to be implemented later) ---

function handleBookActionClick(event) {
    const target = event.target;
    const bookId = target.dataset.id;
    const bookTitle = target.dataset.title; // For lending modal

    if (target.classList.contains('edit-btn')) {
        // Redirect to edit page (passing ID)
        window.location.href = `edit_book.php?id=${bookId}`;
    } else if (target.classList.contains('delete-btn')) {
        handleDeleteBook(bookId);
    } else if (target.classList.contains('lend-btn')) {
        openLendModal(bookId, bookTitle);
    } else if (target.classList.contains('return-btn')) {
        handleReturnBook(bookId);
    }
}

async function handleDeleteBook(bookId) {
    if (confirm('Are you sure you want to delete this book?')) {
        try {
            const response = await fetch('api/delete_book_handler.php', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ bookId: bookId })
            });

            const result = await response.json();

            if (response.ok && result.success) {
                alert('Book deleted successfully!');
                // Refresh the book list (fetch with the current filter)
                const currentGenre = document.getElementById('genre-filter').value;
                fetchBooks(currentGenre);
            } else {
                throw new Error(result.message || 'Failed to delete book');
            }
        } catch (error) {
            console.error('Delete error:', error);
            alert(`Error deleting book: ${error.message}`);
        }
    }
}

// --- Modal Functions ---
function openModal(modalId) {
    const modal = document.getElementById(modalId);
    if(modal) modal.style.display = "block";
}

function closeModal(modalId) {
    const modal = document.getElementById(modalId);
    if(modal) modal.style.display = "none";
}

function openLendModal(bookId, bookTitle) {
    // Populate the modal before showing
    document.getElementById('lendBookId').value = bookId;
    document.getElementById('lendBookTitle').textContent = bookTitle;
    document.getElementById('lentTo').value = ''; // Clear previous entries
    // Set current date as default 
    document.getElementById('lentDate').valueAsDate = new Date(); 
    openModal('lendModal');
}

async function handleLendSubmit(event) {
    event.preventDefault();
    const form = event.target;
    const bookId = document.getElementById('lendBookId').value;
    const lentTo = document.getElementById('lentTo').value;
    const lentDate = document.getElementById('lentDate').value;

    try {
        const response = await fetch('api/lend_return_handler.php', {
            method: 'POST',
            headers: {
                 'Content-Type': 'application/json'
            },
             body: JSON.stringify({ 
                 action: 'lend', 
                 bookId: bookId, 
                 lentTo: lentTo,
                 lentDate: lentDate
             })
        });

        const result = await response.json();

        if(response.ok && result.success) {
            alert('Book marked as lent!');
            closeModal('lendModal');
             const currentGenre = document.getElementById('genre-filter').value;
             fetchBooks(currentGenre); // Refresh list
        } else {
            throw new Error(result.message || 'Failed to lend book.');
        }
    } catch (error) {
        console.error('Lend error:', error);
        alert(`Error lending book: ${error.message}`);
    }
}

async function handleReturnBook(bookId) {
     if (confirm('Are you sure you want to mark this book as returned/available?')) {
         try {
             const response = await fetch('api/lend_return_handler.php', {
                 method: 'POST',
                 headers: {
                     'Content-Type': 'application/json'
                 },
                 body: JSON.stringify({ action: 'return', bookId: bookId })
             });

             const result = await response.json();

             if(response.ok && result.success) {
                 alert('Book marked as available!');
                 const currentGenre = document.getElementById('genre-filter').value;
                 fetchBooks(currentGenre); // Refresh list
             } else {
                 throw new Error(result.message || 'Failed to return book.');
             }
         } catch (error) {
             console.error('Return error:', error);
            alert(`Error returning book: ${error.message}`);
         }
    }
}


// Utility function to prevent basic XSS by escaping HTML
function escapeHTML(str) {
    if (str === null || str === undefined) return '';
    return String(str).replace(/[&<>'"/]/g, function (s) {
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

// Close modal if clicked outside the content
window.onclick = function(event) {
  const modals = document.getElementsByClassName('modal');
  for (let i = 0; i < modals.length; i++) {
      if (event.target == modals[i]) {
          modals[i].style.display = "none";
      }
  }
} 