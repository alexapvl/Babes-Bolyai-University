import { Component, inject, OnInit } from '@angular/core';
import { CommonModule, formatNumber } from '@angular/common';
import { MatCardModule } from '@angular/material/card';
import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { MatSelectModule } from '@angular/material/select';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatSnackBar, MatSnackBarModule } from '@angular/material/snack-bar';
import { FormsModule } from '@angular/forms';
import { RouterLink } from '@angular/router';

import { AuthService } from '../../services/auth.service';
import { ProductService } from '../../services/product.service';
import { Product } from '../../models/product.model';
import { Order } from '../../models/order.model';
import { OrderItem } from '../../models/order.model';

@Component({
  selector: 'app-book-list',
  standalone: true,
  imports: [
    CommonModule,
    MatCardModule,
    MatButtonModule,
    MatIconModule,
    MatSelectModule,
    MatFormFieldModule,
    MatSnackBarModule,
    FormsModule,
    RouterLink,
  ],
  templateUrl: './product-list.component.html',
  styleUrls: ['./product-list.component.scss'],
})
export class ProductListComponent implements OnInit {
  products: Product[] = [];
  currentOrder: Product[] = [];
  loading: boolean = false;
  authService = inject(AuthService);
  currentUser = this.authService.currentUserValue;

  constructor(
    private productService: ProductService,
    private snackBar: MatSnackBar
  ) {}

  ngOnInit(): void {
    this.loadProducts();
    // this.loadGenres();
  }

  loadProducts(): void {
    this.loading = true;
    this.productService.getProducts().subscribe({
      next: (data) => {
        this.products = data;
        this.loading = false;
      },
      error: (error) => {
        console.error('Error fetching products:', error);
        this.snackBar.open('Failed to load products', 'Close', {
          duration: 3000,
        });
        this.loading = false;
      },
    });
  }

  addProductToOrder(product: Product): void {
    if (!this.currentOrder.includes(product)) {
      this.currentOrder.push(product);
      console.log('Current order:');
      for (let i = 0; i < this.currentOrder.length; i++) {
        console.log(this.currentOrder[i].name);
      }
    } else {
      console.log('Item already in cart.');
    }
  }

  confirmOrder(): void {
    let totalPrice: number = 0;
    for (let i = 0; i < this.currentOrder.length; i++) {
      totalPrice = totalPrice + Number(this.currentOrder[i].price);
    }
    let discount: number = 0;
    if (this.currentOrder.length >= 3) {
      discount = 0.1;
    }

    totalPrice = totalPrice * (1 - discount);
    let userId = this.currentUser?.id;

    let newOrder: Order = { userId: Number(userId), totalPrice: totalPrice };
    let newOrderId: number = -1;

    // Subscribe to the Observable to actually execute the HTTP request
    this.productService.addOrder(newOrder).subscribe({
      next: (response) => {
        console.log('Order placed successfully:', response);
        newOrderId = (response as { orderId: number }).orderId;

        /////
        for (let i = 0; i < this.currentOrder.length; i++) {
          let newOrderItem: OrderItem = {
            orderId: newOrderId,
            productId: Number(this.currentOrder[i].id),
          };
          console.log(newOrderItem.orderId, newOrderItem.productId);
          this.productService.addOrderItem(newOrderItem).subscribe({
            next: (response) => {
              //////
            },
            error: (error) => {
              /////
            },
          });
        }
        ////
        this.snackBar.open('Order placed successfully!', 'Close', {
          duration: 3000,
        });
        // Clear the current order after successful submission
        this.currentOrder = [];
      },
      error: (error) => {
        console.error('Error placing order:', error);
        this.snackBar.open('Failed to place order: ' + error, 'Close', {
          duration: 3000,
        });
      },
    });
  }

  // loadGenres(): void {
  //   // Get unique genres from all books
  //   this.bookService.getBooks().subscribe({
  //     next: (books) => {
  //       this.genres = [
  //         ...new Set(
  //           books
  //             .map((book) => book.genre)
  //             .filter(
  //               (genre): genre is string =>
  //                 genre !== undefined && genre !== null && genre !== ''
  //             )
  //         ),
  //       ];
  //     },
  //   });
  // }

  // filterByGenre(): void {
  //   this.loadBooks();
  // }

  // deleteBook(id: number): void {
  //   const book = this.books.find((b) => b.id === id);
  //   if (!book) return;

  //   if (confirm(`Are you sure you want to delete "${book.title}"?`)) {
  //     this.bookService.deleteBook(id).subscribe({
  //       next: () => {
  //         this.snackBar.open('Book deleted successfully', 'Close', {
  //           duration: 3000,
  //         });
  //         this.loadBooks(); // Reload the books after deletion
  //       },
  //       error: (error) => {
  //         console.error('Error deleting book:', error);
  //         const message = 'Failed to delete book: ' + error;
  //         this.snackBar.open(message, 'Close', {
  //           duration: 3000,
  //         });
  //       },
  //     });
  //   }
  // }

  // lendBook(book: Book): void {
  //   // This would typically open a dialog for lent details
  //   // For simplicity, we'll just handle it here with a basic prompt
  //   const lentTo = prompt('Enter the name of the person borrowing the book:');
  //   if (!lentTo) return;

  //   const today = new Date().toISOString().slice(0, 10);

  //   if (book.id) {
  //     this.bookService.lendBook(book.id, lentTo, today).subscribe({
  //       next: (response) => {
  //         this.snackBar.open('Book lent successfully', 'Close', {
  //           duration: 3000,
  //         });
  //         this.loadBooks();
  //       },
  //       error: (error) => {
  //         console.error('Error lending book:', error);
  //         const message = 'Failed to lend book: ' + error;
  //         this.snackBar.open(message, 'Close', {
  //           duration: 3000,
  //         });
  //       },
  //     });
  //   }
  // }

  // returnBook(id: number): void {
  //   if (confirm('Mark this book as returned?')) {
  //     this.bookService.returnBook(id).subscribe({
  //       next: (response) => {
  //         this.snackBar.open('Book marked as returned', 'Close', {
  //           duration: 3000,
  //         });
  //         this.loadBooks();
  //       },
  //       error: (error) => {
  //         console.error('Error returning book:', error);
  //         this.snackBar.open('Failed to mark book as returned', 'Close', {
  //           duration: 3000,
  //         });
  //       },
  //     });
  //   }
  // }
}
