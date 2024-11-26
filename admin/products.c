#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"

void addProduct(Product *products, int *count, int max_count) {
    if (*count >= max_count) {
        printf("Tidak bisa menambahkan produk lagi\n");
        return;
    }
    Product *p = &products[*count];
    printf("Masukan ID Produk:  ");
    scanf("%d", &p->id);
    printf("Masukan Nama Produk: ");
    scanf("%s", p->name);
    printf("Masukan Kategori: ");
    scanf("%s", p->category);
    printf("Masukan Harga: ");
    scanf("%f", &p->price);
    printf("Masukan Stok: ");
    scanf("%d", &p->stock);
    (*count)++;
    printf("Produk berhasil ditambah\n");
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
    FILE *file = fopen("inventory.txt", "w");
    if (!file) {
        printf("Error\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%.2f,%d\n",
                products[i].id, products[i].name, products[i].category,
                products[i].price, products[i].stock);
    }
    fclose(file);
    printf("Produk berhasil disimpan\n");
}

int loadProducts(Product *products, int max_count) {
    FILE *file = fopen("inventory.txt", "r");
    if (!file) {
        printf("Tidak ada data.\n");
        return 0;
    }

    int count = 0;
    printf("Memuat produk...\n");

    while (fscanf(file, "%d,%49[^,],%29[^,],%f,%d\n",
                  &products[count].id, products[count].name,
                  products[count].category, &products[count].price,
                  &products[count].stock) == 5) {
        count++;
        if (count >= max_count) break;
    }
    
    fclose(file);

    printf("Memuat %d produk dari file.\n", count);
    return count;
}


void editProduct(Product *products, int count) {
    if (count == 0) {
        printf("Tidak ada produk untuk diedit.\n");
        return;
    }

    int id, found = 0;
    printf("\n--- Edit Produk ---\n");
    printf("Masukkan ID Produk yang ingin diedit: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (products[i].id == id) {
            found = 1;
            printf("\nMengedit Produk ID: %d\n", id);
            printf("Nama saat ini: %s\nMasukkan Nama Baru: ", products[i].name);
            scanf("%s", products[i].name);
            printf("Kategori saat ini: %s\nMasukkan Kategori Baru: ", products[i].category);
            scanf("%s", products[i].category);
            printf("Harga saat ini: %.2f\nMasukkan Harga Baru: ", products[i].price);
            scanf("%f", &products[i].price);
            printf("Stok saat ini: %d\nMasukkan Stok Baru: ", products[i].stock);
            scanf("%d", &products[i].stock);
            printf("Produk berhasil diperbarui!\n");
            return;
        }
    }

    if (!found) {
        printf("Produk dengan ID %d tidak ditemukan.\n", id);
    }
}

void deleteProduct(Product *products, int *count) {
    if (*count == 0) {
        printf("Tidak ada produk untuk dihapus.\n");
        return;
    }

    int id, found = 0;
    printf("Masukan ID produk yang ingin dihapus");
    scanf("%d", &id);

    for (int i = 0; i < *count; i++) {
        if (products[i].id == id) {
            found = 1;

            for (int j = i; j < *count - 1; j++) {
                products[j] = products[j + 1];
            }

            (*count)--;
            printf("Produk berhasil dihapus\n", id);
            break;
        }
    }

    if (!found) {
        printf("Produk tidak ditemukan.\n", id);
    }
}