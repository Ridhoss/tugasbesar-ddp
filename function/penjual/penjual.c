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
    // int pesananCount = bacaFilePesanan(idLogin, pesanan); //! masih ngirim id login

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


void listPesanan(Pesanan pesanan[], int idLogin) {
    int found = 0;
    int count = bacaFilePesanan(pesanan); // Memanggil fungsi bacaFilePesanan
    
    // Jika tidak ada data pesanan
    if (count == 0) {
        printf("Pesanan belum tersedia atau semua pesanan sudah dikonfirmasi.\n");
        return;
    }

    printf("\n--- Daftar Pesanan ---\n");

    // Menampilkan semua pesanan yang belum terkonfirmasi
    for (int i = 0; i < count; i++) {
        printf("ID Pesanan: %d, Nomor: %s, Pembeli ID: %d, Barang ID: %d, Jumlah: %d, Total: %d, Status Pembayaran: %s\n",
               pesanan[i].id_pesanan, pesanan[i].nomorPesanan, pesanan[i].id_pembeli,
               pesanan[i].id_barang, pesanan[i].jumlah, pesanan[i].total, pesanan[i].status_pembayaran);
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

            // Menyimpan perubahan status pembayaran ke file
            FILE *pemesananFile = fopen("database/pemesanan.txt", "r+");
            if (pemesananFile == NULL) {
                printf("Gagal membuka file pemesanan.txt untuk update.\n");
                return;
            }

            // Temp file untuk menyimpan perubahan
            FILE *tempFile = fopen("database/temp.txt", "w");
            if (tempFile == NULL) {
                printf("Gagal membuat file sementara.\n");
                fclose(pemesananFile);
                return;
            }

            char line[1024];
            while (fgets(line, sizeof(line), pemesananFile)) {
                int id_pesanan;
                sscanf(line, "%d", &id_pesanan);

                // Mencocokkan ID pesanan dan mengubah status
                if (id_pesanan == pesanan[i].id_pesanan) {
                    fprintf(tempFile, "%d,%s,%d,%d,%d,%s,%d,%d,%d,%d,%s,%s,%s,%s\n",
                            pesanan[i].id_pesanan, pesanan[i].nomorPesanan, pesanan[i].id_pembeli,
                            pesanan[i].id_penjual, pesanan[i].id_kurir, pesanan[i].tanggalPesanan,
                            pesanan[i].id_barang, pesanan[i].jumlah, pesanan[i].harga,
                            pesanan[i].total, pesanan[i].alamat, pesanan[i].expedisi,
                            pesanan[i].status_pembayaran, pesanan[i].status_pengiriman);
                } else {
                    fputs(line, tempFile); // Salin baris lainnya
                }
            }

            fclose(pemesananFile);
            fclose(tempFile);

            // Ganti file asli dengan file sementara
            remove("database/pemesanan.txt");
            rename("database/temp.txt", "database/pemesanan.txt");

            printf("Pesanan dengan ID %d berhasil dikonfirmasi dan diperbarui.\n", konfirmasiID);
            pesananDikonfirmasi = 1;
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
