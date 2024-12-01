#include "pembeli.h"
#include "../penjual/penjual.h"
#include "../masuk/masuk.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

// Deklarasi global
const char *pemesanan = "database/pemesanan.txt";
const char *keranjang_file = "database/keranjang.txt";

void halamanUser(int *loggedIn, int idLogin) {
    char username[50], password[50], phone[16], alamat[50], store_name[50];
    int rekening, role;

    int conlog;
    conlog = *loggedIn;

    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);

    const int MAX_PRODUCTS = 100;
    Product products[MAX_PRODUCTS];
    int productCount = 0;
    Keranjang keranjang[100];
    int keranjangCount = 0;
    int statusPesanan = 0;

    int pilihanMenu;
    do {
        printf("\n============================\n");
        printf("    Selamat Datang %s\n", username);
        printf("==============================\n");
        printf("1. Lihat Katalog Produk\n");
        printf("2. Lihat Keranjang\n");
        printf("3. Status Pesanan\n");
        printf("4. Logout\n");
        printf("============================\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihanMenu);

        switch (pilihanMenu) {
            case 1: {
                // Load produk dari file
                productCount = bacaFileProduk(products, MAX_PRODUCTS);
                tampilkanKatalog(products, productCount, keranjang, keranjangCount);

                int pilihan = 0;

                for(;;){
                    printf("============================\n");
                    printf("1. Beli Barang\n");
                    printf("2. Kembali\n");
                    printf("Apakah ingin membeli barang = ");
                    scanf("%d", &pilihan);

                    if (pilihan == 1)
                    {
                        BeliBarang(idLogin, products, MAX_PRODUCTS);
                        break;

                    }else if(pilihan == 2)
                    {
                        break;
                    }
                }
                
                break;
            }
            case 2: {
                tampilkanKeranjangDariFile();
                int pilihanKeranjang;
                printf("1. Kembali\n");
                printf("2. Checkout\n");
                printf("3. Hapus Barang dari Keranjang\n");
                printf("Pilih: ");
                scanf("%d", &pilihanKeranjang);

                if (pilihanKeranjang == 2) {
                    if (keranjangCount > 0) {
                        checkout(keranjang, keranjangCount, &statusPesanan, username, phone, store_name, alamat, "JNE");
                        keranjangCount = 0;
                    } else {
                        printf("Keranjang kosong, tidak dapat checkout.\n");
                    }
                } else if (pilihanKeranjang == 3) {
                    hapusDariKeranjang(keranjang, &keranjangCount);
                }

                break;
            }
            case 3: {
                if (statusPesanan) {
                    printf("Pesanan Anda sedang diproses.\n");
                    bacaFilePesanan();
                } else {
                    printf("Belum ada pesanan.\n");
                }
                break;
            }
            case 4:
                Logout(&conlog);
                *loggedIn = conlog;
                break;
            default:
                printf("Pilihan menu tidak valid.\n");
        }
    } while (*loggedIn && pilihanMenu != 4);
}

void BeliBarang(int loggedIn, Product products[], int maxProducts) {
    int productCount = 0;
    Keranjang keranjang[100];
    int keranjangCount = 0;

    for(;;){
        // Load produk dari file
        productCount = bacaFileProduk(products, maxProducts);
        tampilkanKatalog(products, productCount, keranjang, keranjangCount);


        printf("============================\n");
        printf("Pilih produk yang akan dibeli: ");
        int pilihanBarang;
        scanf("%d", &pilihanBarang);

        if (pilihanBarang >= 1 && pilihanBarang <= productCount) {
            printf("Masukkan jumlah produk yang ingin dibeli: ");
            int jumlahBeli;
            scanf("%d", &jumlahBeli);

            if (jumlahBeli > 0) {
                tambahKeKeranjang(keranjang, &keranjangCount, products, pilihanBarang, jumlahBeli);
                break;
            } else {
                printf("Jumlah tidak valid!\n");
            }
            
        } else {
            printf("Pilihan produk tidak valid!\n");
        }
    }
}

int bacaFileProduk(Product products[], int maxProducts) {
    FILE *file = fopen(file_products, "r");
    if (!file) {
        printf("Gagal membuka file %s\n", file_products);
        return 0;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (count >= maxProducts) {
            printf("Jumlah produk melebihi kapasitas maksimum.\n");
            break;
        }

        Product p;
        if (sscanf(line, "%d,%49[^,],%29[^,],%d,%d,%d", &p.id, p.name, p.category, &p.price, &p.stock, &p.id_penjual) == 6) {
            products[count++] = p;
        } else {
            printf("Gagal mem-parsing baris: %s\n", line);
        }
    }

    fclose(file);
    return count;
}

void formatRibuan(int angka, char *output) {
    char angkas[20];
    sprintf(angkas, "%d", angka);
    int len = strlen(angkas);
    int sisa = len % 3;

    int pos = 0;
    for (int i = 0; i < len; i++) {
        if (i > 0 && (i - sisa) % 3 == 0) {
            output[pos++] = '.';
        }
        output[pos++] = angkas[i];
    }
    output[pos] = '\0';
}

int hitungStokTersedia(Product *product, Keranjang keranjang[], int keranjangCount) {
    int jumlahDiKeranjang = 0;
    for (int i = 0; i < keranjangCount; i++) {
        if (strcmp(product->name, keranjang[i].nama) == 0) {
            jumlahDiKeranjang += keranjang[i].jumlah;
        }
    }
    return product->stock - jumlahDiKeranjang;
}

void tampilkanKatalog(Product products[], int jumlahProduk, Keranjang keranjang[], int keranjangCount) {
    printf("\n============================\n");
    printf("         KATALOG\n");
    printf("============================\n");

    char hargaFormatted[20];
    for (int i = 0; i < jumlahProduk; i++) {
        formatRibuan((int)products[i].price, hargaFormatted);
        int stokTersedia = hitungStokTersedia(&products[i], keranjang, keranjangCount);
        printf("ID: %d\n", products[i].id);
        printf("Nama: %s\n", products[i].name);
        printf("Kategori: %s\n", products[i].category);
        printf("Harga: Rp.%s\n", hargaFormatted);
        printf("Stok: %d\n", stokTersedia);
        printf("Toko: %d\n", products[i].id_penjual);
        printf("----------------------------\n");
    }
}

void simpanKeranjangKeFile(Keranjang keranjang[], int keranjangCount) {
    FILE *file = fopen(keranjang_file, "w");
    if (!file) {
        printf("Gagal menyimpan keranjang ke file.\n");
        return;
    }
    for (int i = 0; i < keranjangCount; i++) {
        fprintf(file, "%s,%d,%d\n", keranjang[i].nama, keranjang[i].jumlah, keranjang[i].harga);
    }
    fclose(file);
}

int bacaKeranjangDariFile(Keranjang keranjang[]) {
    FILE *file = fopen(keranjang_file, "r");
    if (!file) {
        printf("Keranjang kosong atau file tidak ditemukan.\n");
        return 0;
    }
    int count = 0;
    while (fscanf(file, "%[^,],%d,%d\n", keranjang[count].nama, &keranjang[count].jumlah, &keranjang[count].harga) == 3) {
        count++;
    }
    fclose(file);
    return count;
}

void konfirmasiPembelian(Product products[], int jumlahProduk, Keranjang keranjang[], int keranjangCount) {
    for (int i = 0; i < keranjangCount; i++) {
        for (int j = 0; j < jumlahProduk; j++) {
            if (strcmp(products[j].name, keranjang[i].nama) == 0) {
                products[j].stock -= keranjang[i].jumlah;
            }
        }
    }
    tulisUlangFileProduk(products, jumlahProduk);
    printf("Pesanan dikonfirmasi dan stok diperbarui di file produk.\n");

    // Kosongkan keranjang
    FILE *file = fopen("keranjang.txt", "w");
    if (file) fclose(file);
}


void tambahKeKeranjang(Keranjang keranjang[], int *keranjangCount, Product products[], int pilihanProduk, int jumlahBeli) {

    if (products[pilihanProduk - 1].stock < jumlahBeli) {
        printf("Stok tidak mencukupi! Tersedia: %d\n", products[pilihanProduk - 1].stock);
        return;
    }

    // Tambahkan produk ke keranjang
    keranjang[*keranjangCount].jumlah = jumlahBeli;
    strcpy(keranjang[*keranjangCount].nama, products[pilihanProduk - 1].name);
    keranjang[*keranjangCount].harga = products[pilihanProduk - 1].price;

    (*keranjangCount)++;
    printf("Barang berhasil ditambahkan ke keranjang!\n");

    // Simpan keranjang ke file
    simpanKeranjangKeFile(keranjang, *keranjangCount);
}


void tulisUlangFileProduk(Product products[], int jumlahProduk) {
    FILE *file = fopen(file_products, "w");
    if (!file) {
        printf("Gagal membuka file untuk menulis ulang.\n");
        return;
    }

    for (int i = 0; i < jumlahProduk; i++) {
        fprintf(file, "%d,%s,%s,%d,%d,%d\n",
                products[i].id,
                products[i].name,
                products[i].category,
                products[i].price,
                products[i].stock,
                products[i].id_penjual);
    }

    fclose(file);
    printf("Stok berhasil diperbarui di file.\n");
}

void tampilkanKeranjangDariFile() {
    Keranjang keranjang[100];
    int keranjangCount = bacaKeranjangDariFile(keranjang); // Membaca keranjang dari file

    printf("\n============================\n");
    printf("         KERANJANG\n");
    printf("============================\n");

    if (keranjangCount == 0) {
        printf("Keranjang kosong!\n");
        return;
    }

    char hargaFormatted[20];
    char totalHargaFormatted[20];
    int totalHargaKeseluruhan = 0;

    for (int i = 0; i < keranjangCount; i++) {
        formatRibuan((int)keranjang[i].harga, hargaFormatted);
        int totalHargaBarang = keranjang[i].harga * keranjang[i].jumlah;
        totalHargaKeseluruhan += totalHargaBarang;
        formatRibuan(totalHargaBarang, totalHargaFormatted);
        
        printf("Barang %d:\n", i + 1);
        printf("  Nama           : %s\n", keranjang[i].nama);
        printf("  Jumlah         : %d\n", keranjang[i].jumlah);
        printf("  Harga Satuan   : Rp.%s\n", hargaFormatted);
        printf("  Total Harga    : Rp.%s\n", totalHargaFormatted);
        printf("----------------------------\n");
    }

    formatRibuan(totalHargaKeseluruhan, totalHargaFormatted);
    printf("Total Harga Keseluruhan: Rp.%s\n", totalHargaFormatted);
    printf("============================\n");
}


void checkout(Keranjang keranjang[], int keranjangCount, int *statusPesanan, 
              const char *pembeli, const char *kontak, const char *penjual, 
              const char *alamat, const char *ekspedisi) {
    float totalHarga = 0;
    for (int i = 0; i < keranjangCount; i++) {
        totalHarga += keranjang[i].harga * keranjang[i].jumlah;
    }

    printf("Total Harga: Rp. %.2f\n", totalHarga);
    printf("Silahkan pilih metode pembayaran:\n");
    printf("1. BCA\n");
    printf("2. BRI\n");
    printf("3. MANDIRI\n");
    printf("Pilih: ");
    int metodePembayaran;
    scanf("%d", &metodePembayaran);

    if (metodePembayaran >= 1 && metodePembayaran <= 3) {
        printf("Masukkan nominal pembayaran: ");
        float nominal;
        scanf("%f");

        if (nominal >= totalHarga) {
            printf("Pembayaran berhasil! Kembalian Anda: Rp. %.2f\n", nominal - totalHarga);
            *statusPesanan = 1;
            tulisPesanan(keranjang, keranjangCount, pembeli, kontak, penjual, alamat, ekspedisi, *statusPesanan);
        } else {
            printf("Pembayaran gagal! Nominal kurang dari total harga.\n");
        }
    } else {
        printf("Metode pembayaran tidak valid!\n");
    }
}

void tulisPesanan(Keranjang keranjang[], int keranjangCount, const char *pembeli, const char *kontak, const char *penjual, const char *alamat, const char *ekspedisi, int statusPembayaran) {

    FILE *file = fopen(pemesanan, "a"); // Buka file dalam mode append
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menyimpan pesanan!\n", pemesanan);
        return;
    }

    int last_id = 0;

    // Cari id terakhir
    FILE *cekpes = fopen(pemesanan, "r");
    if (cekpes != NULL) {
        char line[200];
        while (fgets(line, sizeof(line), cekpes)) {
            int temp_id;
            sscanf(line, "%d,", &temp_id);
            if (temp_id > last_id) {
                last_id = temp_id;
            }
        }
        fclose(cekpes);
    }

    // Dapatkan tanggal saat ini
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char tanggalPesanan[50];
    strftime(tanggalPesanan, sizeof(tanggalPesanan), "%d %B %Y", local);

    // Randomize nomor pesanan
    srand(time(0));
    int noPesanan = rand() % 900 + 100; // Random angka 100-999

    fprintf(file, "%d,%d,%s,%s,%s,%s,", last_id + 1, noPesanan, pembeli, kontak, penjual, tanggalPesanan);

    // Tulis informasi setiap produk dalam keranjang
    float totalHarga = 0;
    for (int i = 0; i < keranjangCount; i++) {
        float hargaTotal = keranjang[i].harga * keranjang[i].jumlah;
        totalHarga += hargaTotal;

        fprintf(file, "%s,%d,%d,%d,", keranjang[i].nama, keranjang[i].jumlah, keranjang[i].harga, hargaTotal);
    }

    fprintf(file, "%s,%s,%s\n", alamat, ekspedisi, statusPembayaran ? "Sudah dibayar" : "Belum dibayar");

    fclose(file); // Tutup file
    printf("Pesanan berhasil disimpan ke file %s.\n", pemesanan);
}

// Fungsi untuk membaca file pesanan
void bacaFilePesanan() {
    FILE *file = fopen(pemesanan, "r");
    if (file == NULL) {
        printf("File %s tidak ditemukan!\n", pemesanan);
        return;
    }

    char line[256];
    printf("\n============================\n");
    printf("      RIWAYAT PESANAN\n");
    printf("============================\n");

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

// Fungsi untuk menghapus barang dari keranjang
void hapusDariKeranjang(Keranjang keranjang[], int *keranjangCount) {
    int hapusBarang;
    printf("Masukkan nomor barang yang ingin dihapus (1-%d): ", *keranjangCount);
    scanf("%d", &hapusBarang);

    if (hapusBarang >= 1 && hapusBarang <= *keranjangCount) {
        for (int i = hapusBarang - 1; i < *keranjangCount - 1; i++) {
            keranjang[i] = keranjang[i + 1];
        }
        (*keranjangCount)--;
        printf("Barang berhasil dihapus dari keranjang!\n");
    } else {
        printf("Pilihan tidak valid!\n");
    }
}

void konfirmasiPesanan(int *statusPesanan) {
    if (*statusPesanan) {
        printf("Pesanan Anda sudah dikonfirmasi!\n");
    } else {
        printf("Pesanan Anda belum dikonfirmasi.\n");
    }
}

void statusPengiriman(int *pengiriman) {
    printf("\n============================\n");
    printf("  STATUS PENGIRIMAN\n");
    printf("============================\n");
    if (*pengiriman) {
        printf("Pesanan Anda sedang dalam perjalanan!\n");
    } else {
        printf("Pesanan Anda belum dikirimkan.\n");
    }
}

void simpanPesanan(Keranjang keranjang[], int keranjangCount) {
    FILE *file = fopen(pemesanan, "a"); // Buka file dalam mode append
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menyimpan pesanan.\n", pemesanan);
        return;
    }

    fprintf(file, "=== PESANAN BARU ===\n");
    for (int i = 0; i < keranjangCount; i++) {
        float totalHargaBarang = keranjang[i].harga * keranjang[i].jumlah;
        fprintf(file, "Nama Barang: %s\n", keranjang[i].nama);
        fprintf(file, "Jumlah: %d\n", keranjang[i].jumlah);
        fprintf(file, "Harga Satuan: Rp.%d\n", keranjang[i].harga);
        fprintf(file, "Harga Total: Rp.%d\n", totalHargaBarang);
        fprintf(file, "-----------------------\n");
    }
    fprintf(file, "\n");

    fclose(file); // Tutup file setelah selesai
    printf("Pesanan berhasil disimpan ke file %s.\n", pemesanan);
}
