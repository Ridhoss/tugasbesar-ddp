#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "penjual.h"
#include "../pesanan/pesanan.h"
#include "../masuk/masuk.h"
#include "../topup/topup.h"

// Deklarasi global
const char *file_products = "database/product.txt";
const char *file_pemesanan = "database/pemesanan.txt";

void halamanAdmin(int *loggedIn, int idLogin){
    char username[50], password[50], phone[16], alamat[50], store_name[50];
    int rekening, role;
    Pesanan pesanan[100];
    int pesananCount = bacaFilePesanan(idLogin, pesanan);

    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);

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
        printf("===========================\n");
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
                listPesanan(pesanan, pesananCount, idLogin);
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

int loadProducts(Product *products, int max_count) {
    FILE *file = fopen(file_products, "r");
    if (!file) {
        printf("Tidak ada data.\n");
        return 0;
    }

    int count = 0;
    printf("Memuat produk...\n");

    while (fscanf(file, "%d,%49[^,],%29[^,],%d,%d,%d\n",
                  &products[count].id, products[count].name,
                  products[count].category, &products[count].price,
                  &products[count].stock, &products[count].id_penjual) == 6) {
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


void listPesanan(Pesanan pesanan[], int count, int idLogin) {
    int found = 0;
    if (count == 0) {
        printf("Pesanan belum tersedia\n");
        return;
    }

    printf("\n--- Daftar Pesanan ---\n");

    for (int i = 0; i < count; i++) {
        // Hanya tampilkan pesanan yang belum terkonfirmasi
        if (pesanan[i].id_penjual == idLogin && strcmp(pesanan[i].status_pembayaran, "Terkonfirmasi") != 0) {
            printf("ID Pesanan: %d, Nomor: %s, Pembeli ID: %d, Barang ID: %d, Jumlah: %d, Total: %d, Status Pembayaran: %s\n",
                   pesanan[i].id_pesanan, pesanan[i].nomorPesanan, pesanan[i].id_pembeli,
                   pesanan[i].id_barang, pesanan[i].jumlah, pesanan[i].total, pesanan[i].status_pembayaran);
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada pesanan yang belum dikonfirmasi.\n");
        return;
    }

    // Memilih pesanan untuk dikonfirmasi
    int konfirmasiID;
    printf("\nMasukkan ID Pesanan yang ingin dikonfirmasi pembayarannya: ");
    scanf("%d", &konfirmasiID);

    int pesananDikonfirmasi = 0;
    for (int i = 0; i < count; i++) {
        if (pesanan[i].id_pesanan == konfirmasiID && pesanan[i].id_penjual == idLogin) {
            if (strcmp(pesanan[i].status_pembayaran, "Belum Dikonfirmasi") == 0) {
                // Update status pembayaran
                strcpy(pesanan[i].status_pembayaran, "Terkonfirmasi");

                // Tambahkan ke rekapan.txt dalam format CSV
                FILE *rekapanFile = fopen("database/rekapan.txt", "a");
                if (rekapanFile == NULL) {
                    printf("Gagal membuka file rekapan.txt untuk menyimpan pesanan yang telah dikonfirmasi.\n");
                    return;
                }
                fprintf(rekapanFile, "%d,%s,%d,%d,%d,%d,%s\n",
                        pesanan[i].id_pesanan, pesanan[i].nomorPesanan, pesanan[i].id_pembeli,
                        pesanan[i].id_barang, pesanan[i].jumlah, pesanan[i].total, pesanan[i].status_pembayaran);
                fclose(rekapanFile);

                printf("Pesanan dengan ID %d berhasil dikonfirmasi dan dimasukkan ke rekapan.txt.\n", konfirmasiID);
                pesananDikonfirmasi = 1;

                // Menulis ulang seluruh data ke pemesanan.txt dengan status yang terkonfirmasi
                FILE *pemesananFile = fopen("database/pemesanan.txt", "w");
                if (pemesananFile == NULL) {
                    printf("Gagal membuka file pemesanan.txt untuk update.\n");
                    return;
                }

                // Menulis ulang data ke file setelah memperbarui status pembayaran
                for (int j = 0; j < count; j++) {
                    fprintf(pemesananFile, "%d,%s,%d,%d,%d,%d,%d,%s\n",
                            pesanan[j].id_pesanan, pesanan[j].nomorPesanan, pesanan[j].id_penjual,
                            pesanan[j].id_pembeli, pesanan[j].id_barang, pesanan[j].jumlah,
                            pesanan[j].total, pesanan[j].status_pembayaran);
                }

                fclose(pemesananFile);
                printf("Pesanan dengan ID %d berhasil diperbarui di database pemesanan.txt.\n", konfirmasiID);
            } else {
                printf("Pesanan dengan ID %d sudah dikonfirmasi sebelumnya.\n", konfirmasiID);
            }
            break;
        }
    }

    if (!pesananDikonfirmasi) {
        printf("Pesanan dengan ID %d tidak ditemukan atau tidak valid untuk dikonfirmasi.\n", konfirmasiID);
    }
}


int bacaProductDariFile(Product product[]) {
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
            product[count].id = temp_id;
            strcpy(product[count].name, temp_name);
            strcpy(product[count].category, temp_category);
            product[count].price = temp_price;
            product[count].stock = temp_stock;
            product[count].id_penjual = temp_id_penjual;
            count++;
    }
    fclose(file);
    return count;
}
int bacaFilePesanan(int idLogin, Pesanan pesanan[]) {
    FILE *file = fopen(file_pemesanan, "r");
    if (file == NULL) {
        printf("File %s tidak ditemukan!\n", file_pemesanan);
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%d,%49[^,],%49[^,],%49[^,],%49[^,]\n", 
        &pesanan[count].id_pesanan, pesanan[count].nomorPesanan, &pesanan[count].id_pembeli, 
        &pesanan[count].id_penjual, &pesanan[count].id_kurir, pesanan[count].tanggalPesanan, 
        &pesanan[count].id_barang, &pesanan[count].jumlah, &pesanan[count].harga, 
        &pesanan[count].total, pesanan[count].alamat, pesanan[count].expedisi, 
        pesanan[count].status_pembayaran, pesanan[count].status_pengiriman) == 14) {
        count++;
        if (count >= 100) break; // Maksimal 100 pesanan
    }

    fclose(file);
    return count;
}