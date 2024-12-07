#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "penjual.h"
#include "../pesanan/pesanan.h"
#include "../pembeli/pembeli.h"
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

    int choice;
    do {
        printf("===========================\n");
        printf("==== Halaman Admin ====\n");
        printf("===========================\n");
        printf("1. Lihat Daftar Produk\n");
        printf("2. Pengelolaan Produk\n");
        printf("3. Cek Pesanan\n");
        printf("4. Cek Rekening\n");
        printf("5. Logout\n");
        printf("Masukan pilihan:(1-5) ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                daftarProduk(idLogin);
                break;
            case 2:
                pengelolaanProduct(idLogin);
                break;
            case 3:
                listPesanan(pesanan, idLogin);
                break;
            case 4:
                CekRekening(idLogin);
                break;
            case 5:
                Logout(&conlog);
                *loggedIn = conlog;
                break;
            default:
                printf("Pilihan tidak ada\n");
        }
    } while (choice != 5);
}

void pengelolaanProduct(int idLogin) {
    printf("===========================\n");
    printf("====== Kelola Produk ======\n");
    printf("===========================\n");

    const int MAX_PRODUCTS = 100;
    Product products[MAX_PRODUCTS];
    int count = 0;

    int choice;
        do {
        printf("1. Tambah Produk\n");
        printf("2. Edit Produk\n");
        printf("3. Hapus Produk\n");
        printf("4. List Produk\n");
        printf("5. Simpan Produk\n");
        printf("6. Kembali\n");
        printf("Masukan pilihan:(1-6) ");
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
                break;
            default:
                printf("Pilihan tidak ada\n");
        }
    } while (choice != 6);
}

void daftarProduk(int idLogin) {
    Product products[100];
    int productCount = bacaProductDariFile(products);
    int found = 0;

    printf("===========================\n");
    printf("====== Daftar Produk ======\n");
    printf("===========================\n");
    
    for (int i = 0; i < productCount; i++) {
        if (products[i].id_penjual == idLogin) {
            printf("ID Product       : %d\n", products[i].id);
            printf("Nama Product     : %s\n", products[i].name);
            printf("Kategori Product : %s\n", products[i].category);
            printf("Harga Product    : %d\n", products[i].price);
            printf("Stok Product     : %d\n", products[i].stock);
            printf("--------------------------------\n");

            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada produk sementara yang ditemukan.\n");
    }
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

// Menampilkan produk milik toko tertentu
void viewProduct(Product *products, int count, int idLogin) {
    int found = 0;
    printf("===========================\n");
    printf("--- Daftar Produk Sementara ---\n");
    printf("===========================\n");
    printf("Produk yang dimiliki oleh toko %d:\n", idLogin);
    for (int i = 0; i < count; i++) {
        if (products[i].id_penjual == idLogin) {
            printf("ID Product       : %d\n", products[i].id);
            printf("Nama Product     : %s\n", products[i].name);
            printf("Kategori Product : %s\n", products[i].category);
            printf("Harga Product    : %d\n", products[i].price);
            printf("Stok Product     : %d\n", products[i].stock);
            printf("--------------------------------\n");

            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada produk sementara yang ditemukan.\n");
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
    int count = bacaFilePesanan(pesanan);
    int pesCount = 0;

    printf("================================\n");
    printf("======== Daftar Pesanan ========\n");
    printf("================================\n");

    printf("================================\n");
    printf("====== Sudah Dikonfirmasi ======\n");
    printf("================================\n");

    // Menampilkan semua pesanan yang sudah terkonfirmasi
    for (int i = 0; i < count; i++) {
        if(pesanan[i].id_penjual == idLogin){
            if (strcmp(pesanan[i].status_pembayaran, "Terkonfirmasi") == 0)
            {
                Akun akun[100];
                int totalAkun = BacaFileAkun(akun);
                char usPembeli[50];

                for (int j = 0; j < totalAkun ; j++) {
                    if (akun[j].id == pesanan[i].id_pembeli)
                    {
                        strcpy(usPembeli, akun[j].username);
                    }
                }

                Product products[100];
                int totalProduct = bacaProductDariFile(products);
                char temp_namaProduct[50];

                for (int g = 0; g < totalProduct ; g++) {
                    if (products[g].id == pesanan[i].id_barang)
                    {
                        strcpy(temp_namaProduct, products[g].name);
                    }
                }

                // Format harga menjadi ribuan
                char hargaSatuanFor[100];
                formatRibuan(pesanan[i].harga, hargaSatuanFor);
                
                // Format harga menjadi ribuan
                char totalHargaFor[100];
                formatRibuan(pesanan[i].total, totalHargaFor);

                    // Menampilkan data pesanan
                    printf("Pesanan %d:\n", i + 1);
                    printf("  ID Pesanan        : %d\n", pesanan[i].id_pesanan);
                    printf("  Pembeli           : %s\n", usPembeli);
                    printf("  Nomor Pesanan     : %s\n", pesanan[i].nomorPesanan);
                    printf("  Nama Barang       : %s\n", temp_namaProduct);
                    printf("  Jumlah            : %d\n", pesanan[i].jumlah);
                    printf("  Harga Satuan      : Rp.%s\n", hargaSatuanFor);
                    printf("  Total Harga       : Rp.%s\n", totalHargaFor);
                    printf("  Expedisi          : %s\n", pesanan[i].expedisi);
                    printf("  Status Pembayaran : %s\n", pesanan[i].status_pembayaran);
                    printf("  Status Pengiriman : %s\n", pesanan[i].status_pengiriman);
                    printf("----------------------------\n");   

                pesCount ++;
            }
        }
    }

    printf("================================\n");
    printf("====== Belum Dikonfirmasi ======\n");
    printf("================================\n");

    // Menampilkan semua pesanan yang belum terkonfirmasi
    for (int i = 0; i < count; i++) {
        if(pesanan[i].id_penjual == idLogin){
            if (strcmp(pesanan[i].status_pembayaran, "Belum Dikonfirmasi") == 0)
            {
                Akun akun[100];
                int totalAkun = BacaFileAkun(akun);
                char usPembeli[50];

                for (int j = 0; j < totalAkun ; j++) {
                    if (akun[j].id == pesanan[i].id_pembeli)
                    {
                        strcpy(usPembeli, akun[j].username);
                    }
                }

                Product products[100];
                int totalProduct = bacaProductDariFile(products);
                char temp_namaProduct[50];

                for (int g = 0; g < totalProduct ; g++) {
                    if (products[g].id == pesanan[i].id_barang)
                    {
                        strcpy(temp_namaProduct, products[g].name);
                    }
                }

                // Format harga menjadi ribuan
                char hargaSatuanFor[100];
                formatRibuan(pesanan[i].harga, hargaSatuanFor);
                
                // Format harga menjadi ribuan
                char totalHargaFor[100];
                formatRibuan(pesanan[i].total, totalHargaFor);

                    // Menampilkan data pesanan
                    printf("Pesanan %d:\n", i + 1);
                    printf("  ID Pesanan        : %d\n", pesanan[i].id_pesanan);
                    printf("  Pembeli           : %s\n", usPembeli);
                    printf("  Nomor Pesanan     : %s\n", pesanan[i].nomorPesanan);
                    printf("  Nama Barang       : %s\n", temp_namaProduct);
                    printf("  Jumlah            : %d\n", pesanan[i].jumlah);
                    printf("  Harga Satuan      : Rp.%s\n", hargaSatuanFor);
                    printf("  Total Harga       : Rp.%s\n", totalHargaFor);
                    printf("  Expedisi          : %s\n", pesanan[i].expedisi);
                    printf("  Status Pembayaran : %s\n", pesanan[i].status_pembayaran);
                    printf("  Status Pengiriman : %s\n", pesanan[i].status_pengiriman);
                    printf("----------------------------\n");   

                pesCount ++;
            }
        }
    }

    int pilKonfir = 0;
    for (;;) {
        printf("1. Lakukan Konfirmasi\n");
        printf("2. Kembali\n");
        printf("Pilih:(1-2) ");
        scanf("%d", &pilKonfir);

        if(pilKonfir == 1){
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

            break;
        }else if (pilKonfir == 2)
        {
            break;
        }else {
            printf("Eror 404. Input Tidak Valid!\n");
        }
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

        // printf("Baris sebelum parsing: %s\n", line);

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