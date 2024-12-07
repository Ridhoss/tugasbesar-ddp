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

        const int MAX_PRODUCTS = 100;
        Product products[MAX_PRODUCTS];
        int count = 0;
        count = loadProducts(products, MAX_PRODUCTS);
    
    int choice;
    do {
        printf("===========================\n");
        printf("==== Halaman Admin ====\n");
        printf("===========================\n");
        printf("1. Lihat Daftar Produk\n");
        printf("2. Pengelolaan Produk\n");
        printf("3. Cek Pesanan\n");
        printf("4. Cek Rekening\n");
        printf("5. Lihat Rekap Penjualanan\n");
        printf("6. Logout\n");
        printf("Masukan pilihan:(1-5) ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                viewProduct(products, count, idLogin);
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
                LihatRekap(idLogin);
                break;
            case 6:
                Logout(&conlog);
                *loggedIn = conlog;
                break;
            default:
                printf("Pilihan tidak ada\n");
        }
    } while (choice != 6);
}

void pengelolaanProduct(int idLogin) {
    printf("===========================\n");
    printf("====== Kelola Produk ======\n");
    printf("===========================\n");

    const int MAX_PRODUCTS = 100;
    Product products[MAX_PRODUCTS];
    int count = 0;

    count = loadProducts(products, MAX_PRODUCTS);

    viewProduct(products, count, idLogin);

    int choice;
        do {
        printf("1. Tambah Produk\n");
        printf("2. Edit Produk\n");
        printf("3. Hapus Produk\n");
        printf("4. Simpan Produk\n");
        printf("5. Kembali\n");
        printf("Masukan pilihan:(1-5) ");
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
                saveProducts(products, count, idLogin);
                break;
            case 5:
                break;
            default:
                printf("Pilihan tidak ada\n");
        }
    } while (choice != 5);
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
    printf("--- Daftar Produk ---\n");
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
            scanf("%s", products[i].name);
            printf("Masukkan Kategori baru: ");
            scanf("%s", products[i].category);
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

void listPesanan(Pesanan pesanan[], int idLogin) {
    int count = bacaFilePesanan(pesanan);

    tampilkanPesanan(pesanan, count, idLogin, "Terkonfirmasi", "Sudah Dikonfirmasi");
    tampilkanPesanan(pesanan, count, idLogin, "Belum Dikonfirmasi", "Belum Dikonfirmasi");

    int pilKonfir = 0;
    for (;;) {
        printf("1. Lakukan Konfirmasi\n");
        printf("2. Kembali\n");
        printf("Pilih:(1-2) ");
        scanf("%d", &pilKonfir);

        if (pilKonfir == 1) {
            int konfirmasiID, pesananDikonfirmasi = 0;
            printf("\nMasukkan ID Pesanan yang ingin dikonfirmasi pembayarannya: ");
            scanf("%d", &konfirmasiID);

            for (int i = 0; i < count; i++) {
                if (pesanan[i].id_pesanan == konfirmasiID) {
                    strcpy(pesanan[i].status_pembayaran, "Terkonfirmasi");
                    pesanan[i].id_kurir = cariKurir();
                    simpanFilePesanan(pesanan, count);
                    pesananDikonfirmasi = 1;
                    break;
                }
            }

            if (!pesananDikonfirmasi) printf("Pesanan dengan ID %d tidak ditemukan atau tidak valid.\n", konfirmasiID);
            break;
        } else if (pilKonfir == 2) break;
        else printf("Eror 404. Input Tidak Valid!\n");
    }
}

void tampilkanPesanan(Pesanan pesanan[], int count, int idLogin, char *status, char *judul) {
    Akun akun[100];
    Product products[100];
    int totalAkun = BacaFileAkun(akun);
    int totalProduct = bacaProductDariFile(products);
    int pesCount = 0;

    printf("================================\n");
    printf("======= %s =======\n", judul);
    printf("================================\n");

    for (int i = 0; i < count; i++) {
        if (pesanan[i].id_penjual == idLogin && strcmp(pesanan[i].status_pembayaran, status) == 0 && strcmp(pesanan[i].status_pengiriman,"Pesanan Selesai") != 0) {
            char usPembeli[50], temp_namaProduct[50], hargaSatuanFor[100], totalHargaFor[100], totalOngkirFor[100];
            char pesananTanggal[3], pesananBulan[3], pesananTahun[5];
            int pTanggal, pBulan, pTahun;

            for (int j = 0; j < totalAkun; j++) {
                if (akun[j].id == pesanan[i].id_pembeli) strcpy(usPembeli, akun[j].username);
            }
            for (int g = 0; g < totalProduct; g++) {
                if (products[g].id == pesanan[i].id_barang) strcpy(temp_namaProduct, products[g].name);
            }

            formatRibuan(pesanan[i].harga, hargaSatuanFor);
            formatRibuan(pesanan[i].harga * pesanan[i].jumlah, totalHargaFor);
            formatRibuan(pesanan[i].ongkir, totalOngkirFor);

            strncpy(pesananTanggal, pesanan[i].tanggalPesanan, 2);
            strncpy(pesananBulan, pesanan[i].tanggalPesanan + 2, 2);
            strncpy(pesananTahun, pesanan[i].tanggalPesanan + 4, 4);
            pesananTanggal[2] = pesananBulan[2] = pesananTahun[4] = '\0';
            pTanggal = atoi(pesananTanggal);
            pBulan = atoi(pesananBulan);
            pTahun = atoi(pesananTahun);

            printf("Pesanan %d:\n", i + 1);
            printf("  ID Pesanan        : %d\n", pesanan[i].id_pesanan);
            printf("  Pembeli           : %s\n", usPembeli);
            printf("  Nomor Pesanan     : %s\n", pesanan[i].nomorPesanan);
            printf("  Tanggal           : %02d-%02d-%04d\n", pTanggal, pBulan, pTahun);
            printf("  Nama Barang       : %s\n", temp_namaProduct);
            printf("  Jumlah            : %d\n", pesanan[i].jumlah);
            printf("  Harga Satuan      : Rp.%s\n", hargaSatuanFor);
            printf("  Ongkir            : Rp.%s\n", totalOngkirFor);
            printf("  Total Harga       : Rp.%s\n", totalHargaFor);
            printf("  Expedisi          : %s\n", pesanan[i].expedisi);
            printf("  Status Pembayaran : %s\n", pesanan[i].status_pembayaran);
            printf("  Status Pengiriman : %s\n", pesanan[i].status_pengiriman);
            printf("----------------------------\n");
            pesCount++;
        }
    }

    if (pesCount == 0) printf("-- Tidak ada pesanan %s ini! --\n", judul);
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
        int itemsRead = fscanf(file, "%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%d,%d,%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                               &tempPesanan.id_pesanan, tempPesanan.nomorPesanan, &tempPesanan.id_pembeli,
                               &tempPesanan.id_penjual, &tempPesanan.id_kurir, tempPesanan.tanggalPesanan,
                               &tempPesanan.id_barang, &tempPesanan.jumlah, &tempPesanan.harga, &tempPesanan.ongkir,
                               &tempPesanan.total, tempPesanan.alamat, tempPesanan.expedisi,
                               tempPesanan.status_pembayaran, tempPesanan.status_pengiriman);

        // Pastikan semua kolom berhasil terbaca
        if (itemsRead == 15) {
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

void LihatRekap(int idLogin) {
    printf("======================================\n");
    printf("====== Rekapitulasi Penjualanan ======\n");
    printf("======================================\n");

    int tanggal = 0;
    int bulan = 0;
    int tahun = 0;

    for (;;) {
        printf("1. Rekap Harian\n");
        printf("2. Rekap Bulanan\n");
        printf("3. Rekap Tahunan\n");
        int pilModeRekap;
        printf("Pilih:(1-3) ");
        scanf("%d", &pilModeRekap);

        if(pilModeRekap == 1){
            printf("Masukan Tanggal:(1-31) ");
            scanf("%d", &tanggal);

            printf("Masukan Bulan:(1-12) ");
            scanf("%d", &bulan);

            printf("Masukan Tahun: ");
            scanf("%d", &tahun);

            AmbilRekap(tanggal, bulan, tahun, idLogin);
            break;
        }else if (pilModeRekap == 2)
        {
            printf("Masukan Bulan:(1-12) ");
            scanf("%d", &bulan);

            printf("Masukan Tahun: ");
            scanf("%d", &tahun);

            AmbilRekap(0, bulan, tahun, idLogin);
            break;
        }else if (pilModeRekap == 3)
        {
            printf("Masukan Tahun: ");
            scanf("%d", &tahun);

            AmbilRekap(0, 0, tahun, idLogin);
            break;
        }else {
            printf("Error 404. Input Tidak Valid!\n");
        }
    }

}

void AmbilRekap(int tanggal, int bulan, int tahun, int idLogin) {
    Pesanan pesanan[100];
    int pesananCount = bacaFilePesanan(pesanan);
    
    printf("===================================\n");
    printf("Rekap Pesanan:\n");
    printf("===================================\n");

    int adaPesanan = 0;

    for (int i = 0; i < pesananCount; i++) {
        if (pesanan[i].id_penjual == idLogin) {
            if (strcmp(pesanan[i].status_pengiriman,"Pesanan Selesai") == 0)
            {
                // Ambil tanggal, bulan, dan tahun dari pesanan
                char pesananTanggal[3], pesananBulan[3], pesananTahun[5];
                strncpy(pesananTanggal, pesanan[i].tanggalPesanan, 2);
                strncpy(pesananBulan, pesanan[i].tanggalPesanan + 2, 2);
                strncpy(pesananTahun, pesanan[i].tanggalPesanan + 4, 4);

                pesananTanggal[2] = '\0';
                pesananBulan[2] = '\0';
                pesananTahun[4] = '\0';

                int pTanggal = atoi(pesananTanggal);
                int pBulan = atoi(pesananBulan);
                int pTahun = atoi(pesananTahun);

                // Format harga menjadi ribuan
                char totalHargaFor[100];
                formatRibuan((pesanan[i].harga * pesanan[i].jumlah), totalHargaFor);

                Product products[100];
                int totalProduct = bacaProductDariFile(products);
                char temp_namaProduct[50];

                for (int g = 0; g < totalProduct ; g++) {
                    if (products[g].id == pesanan[i].id_barang)
                    {
                        strcpy(temp_namaProduct, products[g].name);
                    }
                }


                // Filter berdasarkan cakupan
                if ((tanggal == 0 || pTanggal == tanggal) && (bulan == 0 || pBulan == bulan) && (tahun == pTahun)) {
                    adaPesanan = 1;

                    printf("  Nomor Pesanan     : %s\n", pesanan[i].nomorPesanan);
                    printf("  Tanggal           : %02d-%02d-%04d\n", pTanggal, pBulan, pTahun);
                    printf("  Nama Barang       : %s\n", temp_namaProduct);
                    printf("  Jumlah            : %d\n", pesanan[i].jumlah);
                    printf("  Total Harga       : %s\n", totalHargaFor);

                    printf("-------------------------------------\n");
                }   
            }
        }
    }

    if (!adaPesanan) {
        printf("Tidak ada pesanan ditemukan untuk cakupan tersebut.\n");
    }
}