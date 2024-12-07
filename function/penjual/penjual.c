#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "penjual.h"
#include "../pesanan/pesanan.h"
#include "../masuk/masuk.h"
#include "../topup/topup.h"
#include "../kurir/kurir.h"

// Deklarasi global
const char *file_products = "database/product.txt";
const char *file_pemesanan = "database/pemesanan.txt";

void halamanAdmin(int *loggedIn, int idLogin){
    char username[50], password[50], phone[16], alamat[50], store_name[50];
    int rekening, role;
    Pesanan pesanan[100];

    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);

    int conlog;
    conlog = *loggedIn;

    printf("===========================\n");
    printf("=== Selamat Datang %s ===\n", store_name);
    printf("===========================\n");

    const int MAX_PRODUCTS = 100;
    Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    int choice;
    do {
        printf("==== Halaman Admin ====\n");
        printf("===========================\n");
        printf("1. Tambah Produk\n");
        printf("2. Edit Produk\n");
        printf("3. Hapus Produk\n");
        printf("4. List Produk\n");
        printf("5. Simpan Produk\n");
        printf("6. Cek Pesanan\n");
        printf("7. Cek Rekening\n");
        printf("8. Logout\n");
        printf("Masukan pilihan: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addProduct(products, &count, MAX_PRODUCTS, idLogin);
                break;
            case 2:
                editProduct(products, count, idLogin);
                break;
            case 3:
                deleteProduct(products, &count,MAX_PRODUCTS, idLogin);
                break;
            case 4:
                viewProduct(products, count, idLogin);
                break;
            case 5:
                saveProducts(products, count, idLogin);
                break;
            case 6:
                listPesanan(pesanan, idLogin);
                break;
            case 7:
                CekRekening(idLogin);
                break;
            case 8:
                Logout(&conlog);
                *loggedIn = conlog;
                break;
            default:
                printf("Pilihan tidak ada\n");
        }
    } while (choice != 8);
}

void addProduct(Product *products, int *count, int max_count, int idLogin) {
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
    scanf("%d", &newProduct.price);
    printf("Masukkan stok produk: ");
    scanf("%d", &newProduct.stock);
    newProduct.id_penjual = idLogin;

    // Tambahkan produk baru ke array
    products[*count] = newProduct;
    (*count)++;

    printf("Produk berhasil ditambahkan dengan ID: %d\n", newProduct.id);
}

void saveProducts(Product *products, int count, int idLogin) {
    FILE *file = fopen(file_products, "w");
    if (!file) {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%d,%d,%d\n", 
                products[i].id, products[i].name, products[i].category, 
                products[i].price, products[i].stock, products[i].id_penjual);
    }

    fclose(file);
    printf("Produk berhasil disimpan.\n");
}

int loadProducts(Product *products) {
    FILE *file = fopen(file_products, "r");
    if (!file) {
        printf("Gagal membuka file untuk membaca data.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%d,%d,%d\n", 
                  &products[count].id, products[count].name, products[count].category, 
                  &products[count].price, &products[count].stock, &products[count].id_penjual) == 6) {
        count++;
    }

    fclose(file);
    return count;
}

// Menampilkan produk milik toko tertentu
void viewProduct(Product *products, int count, int idLogin) {
    int found = 0;
        printf("\n--- Daftar Produk ---\n");
    printf("Produk yang dimiliki oleh toko %d:\n", idLogin);
    for (int i = 0; i < count; i++) {
        if (products[i].id_penjual == idLogin) {
            printf("ID: %d, Nama: %s, Kategori: %s, Harga: %d, Stok: %d\n",
                products[i].id, products[i].name, products[i].category,
                products[i].price, products[i].stock);
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada produk yang ditemukan.\n");
    }
}

// Edit Produk
void editProduct(Product *products, int count, int idLogin) {
    int id;
    printf("Masukkan ID produk yang ingin diedit: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (products[i].id == id) {
            if (products[i].id_penjual != idLogin) {
                printf("Akses ditolak. Anda hanya dapat mengedit produk milik toko Anda.\n");
                return;
            }
            printf("Masukkan Nama Produk baru: ");
            scanf(" %[^\n]", products[i].name);
            printf("Masukkan Kategori baru: ");
            scanf(" %[^\n]", products[i].category);
            printf("Masukkan Harga baru: ");
            scanf("%d", &products[i].price);
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

// Hapus produk
void deleteProduct(Product *products, int *count, int max_count, int idLogin) {
    int id;
    printf("Masukkan ID produk yang ingin dihapus: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (products[i].id == id) {
            if (products[i].id_penjual != idLogin) {
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

// Melihat Pesanan yang pembayarannya belum dikonfirmasi penjual
void listPesanan(Pesanan pesanan[], int idLogin) {
    int found = 0;
    int count = bacaFilePesanan(pesanan);

    if (count == 0) {
        printf("Pesanan belum tersedia atau semua pesanan sudah dikonfirmasi.\n");
        return;
    }

    printf("\n--- Daftar Pesanan ---\n");

    // Menampilkan semua pesanan yang belum terkonfirmasi
    for (int i = 0; i < count; i++) {
        printf("ID Pesanan: %d, Nomor: %s, Pembeli ID: %d, Barang ID: %d, Jumlah: %d, Total: %d, Status Pembayaran: %s, Status Pengiriman: %s\n",
               pesanan[i].id_pesanan, pesanan[i].nomorPesanan, pesanan[i].id_pembeli,
               pesanan[i].id_barang, pesanan[i].jumlah, pesanan[i].total,
               pesanan[i].status_pembayaran, pesanan[i].status_pengiriman);
    }

    // Memilih pesanan untuk dikonfirmasi
    int konfirmasiID;
    printf("\nMasukkan ID Pesanan yang ingin dikonfirmasi pembayarannya: ");
    scanf("%d", &konfirmasiID);

    int pesananDikonfirmasi = 0;
    for (int i = 0; i < count; i++) {
        if (pesanan[i].id_pesanan == konfirmasiID) {
            // Update status pembayaran
            strcpy(pesanan[i].status_pembayaran, "Terkonfirmasi");

            // Assign pesanan ke kurir
            int idKurir = cariKurir();
            if (idKurir != -1) {
                pesanan[i].id_kurir = idKurir;
                printf("Pesanan berhasil diassign ke kurir dengan ID %d.\n", idKurir);
            } else {
                printf("Tidak ada kurir tersedia untuk assign pesanan.\n");
            }

            // Menyimpan perubahan ke file
            simpanFilePesanan(pesanan, count);
            pesananDikonfirmasi = 1;
            break;
        }
    }

    if (!pesananDikonfirmasi) {
        printf("Pesanan dengan ID %d tidak ditemukan atau tidak valid untuk dikonfirmasi.\n", konfirmasiID);
    }
}


int bacaProductDariFile(Product *products) {
    FILE *file = fopen(file_products, "r");
    if (!file) {
        printf("Product kosong atau file tidak ditemukan.\n");
        return 0;
    }

    int count = 0;
    char temp_name[50];
    char temp_category[30];
    int temp_id, temp_price, temp_stock, temp_id_penjual;

    while (fscanf(file, "%d,%49[^,],%29[^,],%d,%d,%d\n", &temp_id, temp_name, temp_category, &temp_price, &temp_stock, &temp_id_penjual) == 6) {
        products[count].id = temp_id;
        strcpy(products[count].name, temp_name);
        strcpy(products[count].category, temp_category);
        products[count].price = temp_price;
        products[count].stock = temp_stock;
        products[count].id_penjual = temp_id_penjual;
        count++;
    }
    fclose(file);
    return count;
}

int bacaFilePesanan(Pesanan pesanan[]) {
    FILE *file = fopen("database/pemesanan.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file pemesanan.txt.\n");
        return 0;
    }

    int count = 0;
    while (!feof(file)) {
        Pesanan tempPesanan;

        // Membaca satu baris data dari file
        int itemsRead = fscanf(file, "%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%d,%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                               &tempPesanan.id_pesanan, tempPesanan.nomorPesanan, &tempPesanan.id_pembeli,
                               &tempPesanan.id_penjual, &tempPesanan.id_kurir, tempPesanan.tanggalPesanan,
                               &tempPesanan.id_barang, &tempPesanan.jumlah, &tempPesanan.harga,
                               &tempPesanan.total, tempPesanan.alamat, tempPesanan.expedisi,
                               tempPesanan.status_pembayaran, tempPesanan.status_pengiriman);

        // Pastikan semua kolom berhasil terbaca
        if (itemsRead == 14) {
            pesanan[count] = tempPesanan; // Menyimpan data ke array
            count++;
        } else if (itemsRead > 0) {
            printf("Data tidak lengkap ditemukan di file, baris dilewati.\n");
        }
    }

    fclose(file);
    return count;
}

int cariKurir() {
    FILE *file = fopen("database/users.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file\n");
        return -1;
    }

    char line[256];
    int id, role;

    while (fgets(line, sizeof(line), file)) {
        // Inisialisasi default
        id = role = 0;

        printf("Baris sebelum parsing: %s\n", line);

        // Parsing hanya ID dan role
        int parsed = sscanf(line, "%d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%d,%*[^,]",
                            &id, &role);

        // Periksa apakah parsing sukses dan role == 3
        if (parsed == 2 && role == 3) {
            fclose(file);
            return id;
        }
    }

    fclose(file);
    return -1; // Tidak ada kurir ditemukan
}

