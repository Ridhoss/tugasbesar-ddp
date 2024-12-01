#include "pembeli.h"
#include "../penjual/penjual.h"
#include "../masuk/masuk.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

void halamanUser(
    int *loggedIn, int idLogin, char *username, char *password, char *phone, 
    char *alamat, int role, char *store_name) {

    const int MAX_PRODUCTS = 100;
    Product products[MAX_PRODUCTS];
    int productCount = 0;
    Keranjang keranjang[100];
    int keranjangCount = 0;
    int statusPesanan = 0;

    // Load produk dari file
    productCount = bacaFileProduk("database/product.txt", products, MAX_PRODUCTS);

    int pilihanMenu;
    do {
        printf("\n============================\n");
        printf("    Selamat Datang %s\n", username);
        printf("============================\n");
        printf("1. Lihat Katalog Produk\n");
        printf("2. Lihat Keranjang\n");
        printf("3. Status Pesanan\n");
        printf("4. Logout\n");
        printf("============================\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihanMenu);

        switch (pilihanMenu) {
            case 1: {
                tampilkanKatalog(products, productCount);
                int pilihanBarang, jumlahBeli;
                printf("Pilih produk yang akan dibeli: ");
                scanf("%d", &pilihanBarang);

                if (pilihanBarang >= 1 && pilihanBarang <= productCount) {
                    printf("Masukkan jumlah produk yang ingin dibeli: ");
                    scanf("%d", &jumlahBeli);

                    if (jumlahBeli > 0 && jumlahBeli <= products[pilihanBarang - 1].stock) {
                        tambahKeKeranjang(
                            keranjang, &keranjangCount, products, 
                            pilihanBarang, jumlahBeli
                        );
                        products[pilihanBarang - 1].stock -= jumlahBeli;
                        printf("Produk berhasil ditambahkan ke keranjang!\n");
                    } else {
                        printf("Jumlah tidak valid atau stok tidak mencukupi!\n");
                    }
                } else {
                    printf("Pilihan produk tidak valid!\n");
                }
                break;
            }
            case 2: {
                tampilkanKeranjang(keranjang, keranjangCount);
                int pilihanKeranjang;
                printf("1. Kembali\n");
                printf("2. Checkout\n");
                printf("3. Hapus Barang dari Keranjang\n");
                printf("Pilih: ");
                scanf("%d", &pilihanKeranjang);

                if (pilihanKeranjang == 2) {
                    if (keranjangCount > 0) {
                        checkout(
                            keranjang, keranjangCount, &statusPesanan,
                            username, phone, store_name, alamat, "JNE"
                        );
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
                    bacaFilePesanan("pesanan.txt");
                } else {
                    printf("Belum ada pesanan.\n");
                }
                break;
            }
            case 4:
                printf("Logout berhasil. Sampai jumpa, %s!\n", username);
                *loggedIn = 0;
                break;
            default:
                printf("Pilihan menu tidak valid.\n");
        }
    } while (*loggedIn && pilihanMenu != 4);
}

int bacaFileProduk(const char *filename, Product products[], int maxProducts) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Gagal membuka file %s\n", filename);
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
        if (sscanf(
            line, "%d,%49[^,],%29[^,],%f,%d,%49[^\n]",
            &p.id, p.name, p.category, &p.price, &p.stock, p.store_name
        ) == 6) {
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

void tampilkanKatalog(Product products[], int jumlahProduk) {
    printf("\n============================\n");
    printf("         KATALOG\n");
    printf("============================\n");

    char hargaFormatted[20];
    for (int i = 0; i < jumlahProduk; i++) {
        formatRibuan((int)products[i].price, hargaFormatted);
        printf("ID: %d\n", products[i].id);
        printf("Nama: %s\n", products[i].name);
        printf("Kategori: %s\n", products[i].category);
        printf("Harga: Rp.%s\n", hargaFormatted);
        printf("Stok: %d\n", products[i].stock);
        printf("Toko: %s\n", products[i].store_name);
        printf("----------------------------\n");
    }
}

void tambahKeKeranjang(
    Keranjang keranjang[], int *keranjangCount, Product products[], 
    int pilihanProduk, int jumlahBeli) {

    if (products[pilihanProduk - 1].stock < jumlahBeli) {
        printf("Stok tidak mencukupi! Tersedia: %d\n", products[pilihanProduk - 1].stock);
        return;
    }

    keranjang[*keranjangCount].jumlah = jumlahBeli;
    strcpy(keranjang[*keranjangCount].nama, products[pilihanProduk - 1].name);
    keranjang[*keranjangCount].harga = products[pilihanProduk - 1].price;

    products[pilihanProduk - 1].stock -= jumlahBeli;
    (*keranjangCount)++;
    printf("Barang berhasil ditambahkan ke keranjang!\n");
}

void tampilkanKeranjang(Keranjang keranjang[], int keranjangCount) {
    printf("\n============================\n");
    printf("         KERANJANG\n");
    printf("============================\n");

    if (keranjangCount == 0) {
        printf("Keranjang kosong!\n");
        return;
    }

    char hargaFormatted[20];
    for (int i = 0; i < keranjangCount; i++) {
        formatRibuan((int)keranjang[i].harga, hargaFormatted);
        printf("Barang %d\n", i + 1);
        printf("Nama: %s\n", keranjang[i].nama);
        printf("Jumlah: %d\n", keranjang[i].jumlah);
        printf("Harga Satuan: Rp.%s\n", hargaFormatted);
        printf("----------------------------\n");
    }
}

// Fungsi untuk checkout keranjang
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
            tulisPesanan(keranjang, keranjangCount, "database/order.txt", pembeli, kontak, penjual, alamat, ekspedisi, *statusPesanan);
        } else {
            printf("Pembayaran gagal! Nominal kurang dari total harga.\n");
        }
    } else {
        printf("Metode pembayaran tidak valid!\n");
    }
}



// Fungsi untuk menulis pesanan ke file
void tulisPesanan(Keranjang keranjang[], int keranjangCount, const char *filename, 
                  const char *pembeli, const char *kontak, const char *penjual, 
                  const char *alamat, const char *ekspedisi, int statusPembayaran) {
    FILE *file = fopen(filename, "a"); // Buka file dalam mode append
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menyimpan pesanan!\n", filename);
        return;
    }

    // Dapatkan tanggal saat ini
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char tanggalPesanan[50];
    strftime(tanggalPesanan, sizeof(tanggalPesanan), "%d %B %Y", local);

    // Randomize nomor pesanan
    srand(time(0));
    int noPesanan = rand() % 900 + 100; // Random angka 100-999

    // Tulis detail pesanan ke file
    fprintf(file, "ID: %d\n", 1); // Contoh ID (dapat diubah jika perlu ID dinamis)
    fprintf(file, "No. Pesanan: %d\n", noPesanan);
    fprintf(file, "Pembeli: %s\n", pembeli);
    fprintf(file, "Kontak: %s\n", kontak);
    fprintf(file, "Penjual: %s\n", penjual);
    fprintf(file, "Tanggal Pesanan: %s\n", tanggalPesanan);

    // Tulis informasi setiap produk dalam keranjang
    float totalHarga = 0;
    for (int i = 0; i < keranjangCount; i++) {
        float hargaTotal = keranjang[i].harga * keranjang[i].jumlah;
        totalHarga += hargaTotal;
        fprintf(file, "Nama Produk: %s\n", keranjang[i].nama);
        fprintf(file, "Qty: %d\n", keranjang[i].jumlah);
        fprintf(file, "Harga: Rp. %.2f\n", keranjang[i].harga);
        fprintf(file, "Total Harga: Rp. %.2f\n", hargaTotal);
    }

    fprintf(file, "Alamat: %s\n", alamat);
    fprintf(file, "Ekspedisi: %s\n", ekspedisi);
    fprintf(file, "Status Pembayaran: %s\n", statusPembayaran ? "Sudah dibayar" : "Belum dibayar");
    fprintf(file, "===============================\n");

    fclose(file); // Tutup file
    printf("Pesanan berhasil disimpan ke file %s.\n", filename);
}

// Fungsi untuk membaca file pesanan
void bacaFilePesanan(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File %s tidak ditemukan!\n", filename);
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

void simpanPesanan(Keranjang keranjang[], int keranjangCount, const char *filename) {
    FILE *file = fopen(filename, "a"); // Buka file dalam mode append
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menyimpan pesanan.\n", filename);
        return;
    }

    fprintf(file, "=== PESANAN BARU ===\n");
    for (int i = 0; i < keranjangCount; i++) {
        float totalHargaBarang = keranjang[i].harga * keranjang[i].jumlah;
        fprintf(file, "Nama Barang: %s\n", keranjang[i].nama);
        fprintf(file, "Jumlah: %d\n", keranjang[i].jumlah);
        fprintf(file, "Harga Satuan: Rp.%.2f\n", keranjang[i].harga);
        fprintf(file, "Harga Total: Rp.%.2f\n", totalHargaBarang);
        fprintf(file, "-----------------------\n");
    }
    fprintf(file, "\n");

    fclose(file); // Tutup file setelah selesai
    printf("Pesanan berhasil disimpan ke file %s.\n", filename);
}
