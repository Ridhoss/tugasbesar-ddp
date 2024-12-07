#ifndef PENJUAL_H
#define PENJUAL_H

#include "../pesanan/pesanan.h"

typedef struct {
    int id;
    char name[50];
    char category[30];
    int price;
    int stock;
    int id_penjual;
} Product;

// ke halaman admin
void halamanAdmin(int *loggedIn, int idLogin);

void pengelolaanProduct(int idLogin);

// Menambahkan produk baru ke dalam daftar produk
void addProduct(Product *products, int *count, int max_count, int idLogin);

// Menyimpan produk ke file
void saveProducts(Product *products, int count, int idLogin);

// Mengedit produk yang dimiliki oleh toko tertentu
void editProduct(Product *products, int count, int idLogin);

// Menghapus produk milik toko tertentu
void deleteProduct(Product *products, int *count, int max_count, int idLogin);

// Menampilkan produk milik toko tertentu
void viewProduct(Product *products, int count, int idLogin);

// Menampilkan pesanan
void listPesanan(Pesanan pesanan[], int idLogin);

int bacaProductDariFile(Product product[]);

int bacaFilePesanan(Pesanan pesanan[]);

int cariKurir();

int loadProducts(Product *products, int max_count);

void LihatRekap(int idLogin);

void AmbilRekap(int tanggal, int bulan, int tahun, int idLogin);

#endif
