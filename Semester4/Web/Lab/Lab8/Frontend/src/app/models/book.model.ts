export interface Book {
  id?: number;
  title: string;
  author: string;
  genre?: string;
  pages?: number;
  lentTo?: string | null;
  lentDate?: string | null;
}
