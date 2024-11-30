#ifndef PENJUAL_H
#define PENJUAL_H

typedef struct {
    int id;
    char name[50];
    char category[30];
    float price;
    int stock;
    char store_name[50];
} Product;

// ke halaman admin
void halamanAdmin(int *loggedIn, int idLogin, char username[50], char password[50], int role, char store_name[50]);

// Menambahkan produk baru ke dalam daftar produk
void addProduct(Product *products, int *count, int max_count, const char *store_name);

// Menampilkan semua produk
void listProducts(Product *products, int count);

// Menyimpan produk ke file
void saveProducts(Product *products, int count);

// Memuat produk dari file
int loadProducts(Product *products, int max_count);

// Mengedit produk yang dimiliki oleh toko tertentu
void editProduct(Product *products, int count, const char *store_name);

// Menghapus produk milik toko tertentu
void deleteProduct(Product *products, int *count, int max_count, const char *store_name);

// Menampilkan produk milik toko tertentu
void viewProduct(Product *products, int count, const char *store_name);

#endif
