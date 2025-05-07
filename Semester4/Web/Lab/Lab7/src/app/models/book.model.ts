export interface Book {
  id?: number;
  title: string;
  author: string;
  genre?: string;
  pages?: number;
  lent_to?: string | null;
  lent_date?: string | null;
}
