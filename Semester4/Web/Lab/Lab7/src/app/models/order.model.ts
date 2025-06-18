export interface Order {
  id?: number; // this is int
  userId: number;
  totalPrice: number; // this is double
}

export interface OrderItem {
  id?: number; // this is int
  orderId: number;
  productId: number;
}
