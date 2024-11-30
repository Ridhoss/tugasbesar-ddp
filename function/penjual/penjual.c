#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "penjual.h"
#include "../masuk/masuk.h"

// Deklarasi global
const char *file_products = "database/product.txt";

void halamanAdmin(int *loggedIn, int idLogin, char username[50], char password[50], int role, char store_name[50]){

    int conlog;
    conlog = *loggedIn;

    printf("===========================\n");
    printf("=== Selamat Datang %s ===\n", store_name);
    printf("===========================\n");

    const int MAX_PRODUCTS = 100;
    Product products[MAX_PRODUCTS];
    int count = 0;

    count = loadProducts(products, MAX_PRODUCTS);

    int choice;
    do {
        printf("\n--- Admin ---\n");
        printf("1. Tambah Produk\n");
        printf("2. List Produk\n");
        printf("3. Simpan Produk\n");
        printf("4. Edit Produk\n");
        printf("5. Hapus Produk\n");
        printf("6. Logout\n");
        printf("Masukan pilihan: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addProduct(products, &count, MAX_PRODUCTS, store_name);
                break;
            case 2:
                viewProduct(products, count, store_name);
                break;
            case 3:
                saveProducts(products, count);
                break;
            case 4:
                editProduct(products, count, store_name);
                break;
            case 5:
                deleteProduct(products, &count,MAX_PRODUCTS ,store_name);
                break;
            case 6:
                printf("Keluar...\n");
                break;
            default:
                printf("Pilihan tidak ada\n");
        }
    } while (choice != 6);
}

void addProduct(Product *products, int *count, int max_count, const char *store_name) {
    if (*count >= max_count) {
        printf("Kapasitas maksimal produk tercapai.\n");
        return;
    }

    // Cari ID terbesar di dalam produk yang ada
    int max_id = 0;
    for (int i = 0; i < *count; i++) {
        if (products[i].id > max_id) {
            max_id = products[i].id;
        }
    }

    // Agar ID Increment
    Product newProduct;
    newProduct.id = max_id + 1;

    // Input data produk baru
    printf("Masukkan nama produk: ");
    scanf(" %[^\n]", newProduct.name);
    printf("Masukkan kategori produk: ");
    scanf(" %[^\n]", newProduct.category);
    printf("Masukkan harga produk: ");
    scanf("%f", &newProduct.price);
    printf("Masukkan stok produk: ");
    scanf("%d", &newProduct.stock);
    strcpy(newProduct.store_name, store_name);

    // Tambahkan produk baru ke array
    products[*count] = newProduct;
    (*count)++;

    printf("Produk berhasil ditambahkan dengan ID: %d\n", newProduct.id);
}


void listProducts(Product *products, int count) {
    if (count == 0) {
        printf("Produk Kosong\n");
        return;
    }
    printf("\n--- Daftar Produk ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Nama: %s, Kategori: %s, Harga: %.2f, Stok: %d\n",
            products[i].id, products[i].name, products[i].category, products[i].price, products[i].stock);
    }
}

void saveProducts(Product *products, int count) {
    FILE *file = fopen(file_products, "w");
    if (!file) {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%.2f,%d,%s\n", 
                products[i].id, products[i].name, products[i].category, 
                products[i].price, products[i].stock, products[i].store_name);
    }

    fclose(file);
    printf("Produk berhasil disimpan.\n");
}

int loadProducts(Product *products, int max_count) {
    FILE *file = fopen(file_products, "r");
    if (!file) {
        printf("Tidak ada data.\n");
        return 0;
    }

    int count = 0;
    printf("Memuat produk...\n");

    while (fscanf(file, "%d,%49[^,],%29[^,],%f,%d,%49[^\n]\n",
                  &products[count].id, products[count].name,
                  products[count].category, &products[count].price,
                  &products[count].stock, products[count].store_name) == 6) {
        // Cek duplikasi ID
        int isDuplicate = 0;
        for (int i = 0; i < count; i++) {
            if (products[i].id == products[count].id) {
                isDuplicate = 1;
                break;
            }
        }
        if (!isDuplicate) {
            count++;
        }
        if (count >= max_count) break;
    }

    fclose(file);
    printf("Memuat %d produk dari file.\n", count);
    return count;
}


// Untuk menampilkan produk milik toko tertentu
void viewProduct(Product *products, int count, const char *store_name) {
    int found = 0;
        printf("\n--- Daftar Produk ---\n");
    printf("Produk yang dimiliki oleh toko %s:\n", store_name);
    for (int i = 0; i < count; i++) {
        if (strcmp(products[i].store_name, store_name) == 0) {
            printf("ID: %d, Nama: %s, Kategori: %s, Harga: %.2f, Stok: %d\n",
                   products[i].id, products[i].name, products[i].category,
                   products[i].price, products[i].stock);
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada produk yang ditemukan.\n");
    }
}

void editProduct(Product *products, int count, const char *store_name) {
    int id;
    printf("Masukkan ID produk yang ingin diedit: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (products[i].id == id) {
            if (strcmp(products[i].store_name, store_name) != 0) {
                printf("Akses ditolak. Anda hanya dapat mengedit produk milik toko Anda.\n");
                return;
            }
            printf("Masukkan Nama Produk baru: ");
            scanf("%s", products[i].name);
            printf("Masukkan Kategori baru: ");
            scanf("%s", products[i].category);
            printf("Masukkan Harga baru: ");
            scanf("%f", &products[i].price);
            printf("Masukkan Stok baru: ");
            scanf("%d", &products[i].stock);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Produk dengan ID %d tidak ditemukan.\n", id);
    } else {
        printf("Produk berhasil diedit.\n");
    }
}


void deleteProduct(Product *products, int *count, int max_count, const char *store_name) {
    int id;
    printf("Masukkan ID produk yang ingin dihapus: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (products[i].id == id) {
            if (strcmp(products[i].store_name, store_name) != 0) {
                printf("Akses ditolak. Anda hanya dapat menghapus produk milik toko Anda.\n");
                return;
            }

            // Pindahkan produk setelah yang dihapus untuk menghapusnya dari array
            for (int j = i; j < *count - 1; j++) {
                products[j] = products[j + 1];
            }
            (*count)--;  // Kurangi jumlah produk
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Produk dengan ID %d tidak ditemukan.\n", id);
    } else {
        printf("Produk berhasil dihapus.\n");
    }
}
