#include "pembeli.h"
#include "../penjual/penjual.h"
#include "../masuk/masuk.h"
#include "../topup/topup.h"
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
        printf("4. Cek Rekening\n");
        printf("5. Logout\n");
        printf("============================\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihanMenu);

        switch (pilihanMenu) {
            case 1: {
                // Load produk dari file
                productCount = bacaFileProduk(products, MAX_PRODUCTS);
                tampilkanKatalog(products, productCount, idLogin);

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
                tampilkanKeranjangDariFile(idLogin);
                int pilihanKeranjang;
                printf("1. Kembali\n");
                printf("2. Checkout\n");
                printf("3. Hapus Barang dari Keranjang\n");
                printf("Pilih: ");
                scanf("%d", &pilihanKeranjang);

                if (pilihanKeranjang == 2) {
                        checkout(idLogin);
                } else if (pilihanKeranjang == 3) {
                    hapusDariKeranjang(idLogin);
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
                // cek rekening
                CekRekening(idLogin);
                break;
            case 5:
                Logout(&conlog);
                *loggedIn = conlog;
                break;
            default:
                printf("Pilihan menu tidak valid.\n");
        }
    } while (*loggedIn && pilihanMenu != 5);
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

void BeliBarang(int idLogin, Product products[], int maxProducts) {
    int productCount = 0;
    Keranjang keranjang[100];
    int keranjangCount = bacaKeranjangDariFile(keranjang, idLogin);

    for(;;) {
        // Load produk dari file
        productCount = bacaFileProduk(products, maxProducts);
        tampilkanKatalog(products, productCount, idLogin);

        printf("============================\n");
        printf("Pilih produk yang akan dibeli: ");
        int pilihanBarang;
        scanf("%d", &pilihanBarang);

        if (pilihanBarang >= 1 && pilihanBarang <= productCount) {
            int stokAsli = products[pilihanBarang - 1].stock;
            int stokTerpakai = 0;

            // Hitung jumlah barang yang sudah ada di keranjang
            for (int i = 0; i < keranjangCount; i++) {
                if (keranjang[i].id_barang == products[pilihanBarang - 1].id) {
                    stokTerpakai += keranjang[i].jumlah;
                }
            }

            // Hitung stok tersedia
            int stokTersedia = stokAsli - stokTerpakai;
            if (stokTersedia < 0) stokTersedia = 0; // Hindari stok negatif

            printf("Stok tersedia: %d\n", stokTersedia);
            printf("Masukkan jumlah produk yang ingin dibeli: ");
            int jumlahBeli;
            scanf("%d", &jumlahBeli);

            if (jumlahBeli > 0 && jumlahBeli <= stokTersedia) {
                tambahKeKeranjang(idLogin, keranjang, &keranjangCount, products, pilihanBarang, jumlahBeli);
                break; // Keluar dari loop setelah berhasil menambah ke keranjang
            } else {
                printf("Jumlah tidak valid! (Stok tersedia: %d)\n", stokTersedia);
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

//! KATALOG

void tampilkanKatalog(Product products[], int jumlahProduk, int idLogin) {
    // Baca keranjang dari file
    Keranjang keranjang[100];
    int keranjangCount = bacaKeranjangDariFile(keranjang, idLogin);

    printf("\n============================\n");
    printf("         KATALOG\n");
    printf("============================\n");

    char hargaFormatted[20];
    for (int i = 0; i < jumlahProduk; i++) {
        int stokAsli = products[i].stock;
        int stokTerpakai = 0;

        // Hitung jumlah barang yang sudah ada di keranjang
        for (int j = 0; j < keranjangCount; j++) {
            if (keranjang[j].id_barang == products[i].id) {
                stokTerpakai += keranjang[j].jumlah;
            }
        }

        // Hitung stok tersedia
        int stokTersedia = stokAsli - stokTerpakai;

        // Format harga menjadi ribuan
        formatRibuan((int)products[i].price, hargaFormatted);

        // Tampilkan informasi produk
        printf("ID: %d\n", products[i].id);
        printf("Nama: %s\n", products[i].name);
        printf("Kategori: %s\n", products[i].category);
        printf("Harga: Rp.%s\n", hargaFormatted);
        printf("Stok: %d\n", stokTersedia >= 0 ? stokTersedia : 0); // Hindari stok negatif
        printf("Toko: %d\n", products[i].id_penjual);
        printf("----------------------------\n");
    }
}

//! KERANJANG
void simpanKeranjangKeFile(Keranjang keranjang[], int keranjangCount) {
    FILE *file = fopen(keranjang_file, "w");
    if (!file) {
        printf("Gagal menyimpan keranjang ke file.\n");
        return;
    }

    for (int i = 0; i < keranjangCount; i++) {
        fprintf(file, "%d,%d,%d,%d,%d\n", 
                keranjang[i].id, 
                keranjang[i].id_barang, 
                keranjang[i].jumlah, 
                keranjang[i].harga, 
                keranjang[i].id_pembeli);
    }
    fclose(file);
    printf("Keranjang berhasil disimpan ke file.\n");
}

void tambahKeKeranjang(int idLogin, Keranjang keranjang[], int *keranjangCount, Product products[], int pilihanProduk, int jumlahBeli) {
    int idBarang = products[pilihanProduk - 1].id;
    int stokTersedia = products[pilihanProduk - 1].stock;

    // Hitung total barang yang sama di keranjang
    int totalBarangDiKeranjang = 0;
    for (int i = 0; i < *keranjangCount; i++) {
        if (keranjang[i].id_barang == idBarang && keranjang[i].id_pembeli == idLogin) {
            totalBarangDiKeranjang += keranjang[i].jumlah;
        }
    }

    // Periksa apakah jumlah barang yang akan ditambahkan melebihi stok
    if (totalBarangDiKeranjang + jumlahBeli > stokTersedia) {
        printf("Stok tidak mencukupi untuk menambah barang ke keranjang!\n");
        printf("Stok tersedia: %d, jumlah di keranjang: %d\n", stokTersedia - totalBarangDiKeranjang, totalBarangDiKeranjang);
        return;
    }

    // Periksa apakah barang sudah ada di keranjang
    for (int i = 0; i < *keranjangCount; i++) {
        if (keranjang[i].id_barang == idBarang && keranjang[i].id_pembeli == idLogin) {
            keranjang[i].jumlah += jumlahBeli;  // Perbarui jumlah barang
            printf("Jumlah barang di keranjang berhasil diperbarui!\n");
            simpanKeranjangKeFile(keranjang, *keranjangCount);  // Simpan perubahan
            return;
        }
    }

    // Jika barang belum ada di keranjang, tambahkan sebagai entri baru
    keranjang[*keranjangCount].id_barang = idBarang;
    keranjang[*keranjangCount].harga = products[pilihanProduk - 1].price;
    keranjang[*keranjangCount].jumlah = jumlahBeli;
    keranjang[*keranjangCount].id_pembeli = idLogin;

    (*keranjangCount)++;
    printf("Barang berhasil ditambahkan ke keranjang!\n");

    // Simpan keranjang ke file
    simpanKeranjangKeFile(keranjang, *keranjangCount);
}

int bacaKeranjangDariFile(Keranjang keranjang[], int idLogin) {
    FILE *file = fopen(keranjang_file, "r");
    if (!file) {
        printf("Keranjang kosong atau file tidak ditemukan.\n");
        return 0;
    }
    int count = 0;
    int temp_id, temp_id_barang, temp_harga, temp_jumlah, temp_id_pembeli;

    while (fscanf(file, "%d,%d,%d,%d,%d\n", &temp_id, &temp_id_barang, &temp_jumlah, &temp_harga, &temp_id_pembeli) == 5) {
        // Hanya tambahkan data jika id_pembeli cocok dengan idLogin
        if (temp_id_pembeli == idLogin) {
            keranjang[count].id = temp_id;
            keranjang[count].id_barang = temp_id_barang;
            keranjang[count].harga = temp_harga;
            keranjang[count].jumlah = temp_jumlah;
            keranjang[count].id_pembeli = temp_id_pembeli;
            count++;
        }
    }
    fclose(file);
    return count;
}

void konfirmasiPembelian(Product products[], int jumlahProduk, Keranjang keranjang[], int keranjangCount) {
    for (int i = 0; i < keranjangCount; i++) {
        for (int j = 0; j < jumlahProduk; j++) {
            if (products[j].id == keranjang[i].id_barang) {
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

void tampilkanKeranjangDariFile(int idLogin) {
    // Membaca data produk dari file
    Product products[100];
    int productCount = bacaFileProduk(products, 100);
    if (productCount == 0) {
        printf("Gagal membaca data produk. Tidak dapat menampilkan keranjang.\n");
        return;
    }

    // Membaca data keranjang dari file
    Keranjang keranjang[100];
    int keranjangCount = bacaKeranjangDariFile(keranjang, idLogin);

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
        // Mencari nama barang berdasarkan id_barang
        const char *namaBarang = "Tidak ditemukan";
        for (int j = 0; j < productCount; j++) {
            if (keranjang[i].id_barang == products[j].id) {
                namaBarang = products[j].name;
                break;
            }
        }

        // Format harga
        formatRibuan((int)keranjang[i].harga, hargaFormatted);
        int totalHargaBarang = keranjang[i].harga * keranjang[i].jumlah;
        totalHargaKeseluruhan += totalHargaBarang;
        formatRibuan(totalHargaBarang, totalHargaFormatted);

        // Menampilkan data keranjang
        printf("Barang %d:\n", i + 1);
        printf("  Nama           : %s\n", namaBarang);
        printf("  Jumlah         : %d\n", keranjang[i].jumlah);
        printf("  Harga Satuan   : Rp.%s\n", hargaFormatted);
        printf("  Total Harga    : Rp.%s\n", totalHargaFormatted);
        printf("----------------------------\n");
    }

    // Menampilkan total harga keseluruhan
    formatRibuan(totalHargaKeseluruhan, totalHargaFormatted);
    printf("Total Harga Keseluruhan: Rp.%s\n", totalHargaFormatted);
    printf("============================\n");
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

void checkout(int idLogin) {
    int totalHarga = 0;
    int saldo = 0;

    Keranjang keranjang[100];
    int keranjangCount = bacaKeranjangDariFile(keranjang, idLogin);

    int metodePembayaran = 0;
    char konfpem;

    for (int i = 0; i < keranjangCount; i++) {
        totalHarga += keranjang[i].harga * keranjang[i].jumlah;
    }

    printf("============================\n");
    printf("Total Pembayaran: Rp. %d\n", totalHarga);
    printf("============================\n");
    printf("Silahkan pilih metode pembayaran:\n");
    printf("1. RisaizPay\n");
    printf("Pilih: ");
    scanf("%d", &metodePembayaran);

    if (metodePembayaran == 1) {
        saldo = HanyaTampilkanSaldo(idLogin);
        printf("Lakukan Pembayaran Sebesar Rp.%d\n", totalHarga);
        printf("============================\n");
        printf("Konfirmasi Pembayaran (y/n): ");
        scanf("%s", &konfpem);

        if (konfpem == 'y' || konfpem == 'Y')
        {
            // lakukan checkout pembayaran
            if (totalHarga <= saldo)
            {
                tulisPesanan(idLogin);
            }else {
                printf("============================\n");
                printf("Saldo anda tidak mencukupi!! Silahkan lakukan TopUp!\n");
                printf("============================\n");
            }

        }else if(konfpem == 'n' || konfpem == 'N') {
            printf("============================\n");
            printf("Pembayaran Dibatalkan\n");
            printf("============================\n");
            return;
        }else {
            printf("Input Tidak Valid\n");
        }
    } else {
        printf("Metode pembayaran tidak valid!\n");
    }
}

//! tulis pesanan punya faiz

void tulisPesanan(int idLogin) {

    char username[50], password[50], phone[16], alamat[50], store_name[50];
    int rekening, role;
    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);

    Keranjang keranjang[100];
    int keranjangCount = bacaKeranjangDariFile(keranjang, idLogin);
    int totalHarga = 0;

    for (int i = 0; i < keranjangCount; i++) {
        totalHarga += keranjang[i].harga * keranjang[i].jumlah;
    }

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
    char tanggalPesanan[15]; // Array untuk menyimpan tanggal
    char waktuPesanan[10];   // Array untuk menyimpan waktu

    // Mendapatkan waktu saat ini
    time_t t = time(NULL);
    struct tm *waktu = localtime(&t);

    // Format tanggal dan waktu secara terpisah
    snprintf(tanggalPesanan, sizeof(tanggalPesanan), "%02d-%02d-%04d", waktu->tm_mday, waktu->tm_mon + 1, waktu->tm_year + 1900);

    snprintf(waktuPesanan, sizeof(waktuPesanan), "%02d:%02d:%02d", waktu->tm_hour, waktu->tm_min, waktu->tm_sec);

    // Randomize nomor pesanan
    srand(time(0));
    int noPesanan = rand() % 900 + 100; // Random angka 100-999

    
    for (int i = 0; i < keranjangCount; i++) {
        
        int hargaTotal = keranjang[i].harga * keranjang[i].jumlah;

        Product product[100];
        int productCount = bacaProductDariFile(product, keranjang[i].id_barang);

        //! ABIS DI CO KERANJANG BELOM ILANG
        //! ID KURIR JUGA BELOM BISAA
        //! nomor pemesanan kalo bisa formatnya tanggal terus increment contoh 03122401 - 031224 tanggal hari ini - 01 increment, jadi next nomor pemesanann di hari ini berarti jadi 02.

        fprintf(file, "%d,%d,%d,%d,%d,%s,", 
                last_id + 1, noPesanan, idLogin, product->id_penjual, 0, tanggalPesanan); 

        fprintf(file, "%d,%d,%d,%d,", keranjang[i].id_barang, keranjang[i].jumlah, keranjang[i].harga, hargaTotal);

        fprintf(file, "%s,%s,%s,%s\n", alamat, "GoRisaiz", "Belum Dikonfirmasi", "Belum Dikirim");

        last_id++;
    }

    fclose(file); 
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

// Fungsi untuk menghapus atau mengurangi qty barang di keranjang
void hapusDariKeranjang(int idLogin) {
    Keranjang keranjang[100];
    int keranjangCount = bacaKeranjangDariFile(keranjang, idLogin);

    if (keranjangCount == 0) {
        printf("Keranjang kosong.\n");
        return;
    }

    printf("\nDaftar barang dalam keranjang:\n");
    for (int i = 0; i < keranjangCount; i++) {
        printf("%d. ID Barang: %d, Jumlah: %d, Harga: %d\n", i + 1, keranjang[i].id_barang, keranjang[i].jumlah, keranjang[i].harga);
        printf("============================\n");
    }

    int hapusBarang;
    printf("Masukkan nomor barang yang ingin diubah (1-%d): ", keranjangCount);
    scanf("%d", &hapusBarang);

    if (hapusBarang >= 1 && hapusBarang <= keranjangCount) {
        int index = hapusBarang - 1;

        printf("Pilih tindakan:\n1. Hapus semua\n2. Kurangi qty\nMasukkan pilihan (1/2): ");
        int pilihan;
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            for (int i = index; i < keranjangCount - 1; i++) {
                keranjang[i] = keranjang[i + 1];
            }
            keranjangCount--;
            printf("Barang berhasil dihapus dari keranjang!\n");

        } else if (pilihan == 2) {
            int qtyKurang;
            printf("Masukkan jumlah qty yang ingin dikurangi (1-%d): ", keranjang[index].jumlah);
            scanf("%d", &qtyKurang);

            if (qtyKurang >= 1 && qtyKurang <= keranjang[index].jumlah) {
                keranjang[index].jumlah -= qtyKurang;
                if (keranjang[index].jumlah == 0) {
                    for (int i = index; i < keranjangCount - 1; i++) {
                        keranjang[i] = keranjang[i + 1];
                    }
                    keranjangCount--;
                    printf("Semua qty telah dikurangi, barang dihapus dari keranjang!\n");
                } else {
                    printf("Jumlah barang dikurangi menjadi %d.\n", keranjang[index].jumlah);
                }
            } else {
                printf("Jumlah pengurangan tidak valid.\n");
            }
        } else {
            printf("Pilihan tidak valid.\n");
        }

        simpanKeranjangKeFile(keranjang, keranjangCount);
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
        fprintf(file, "Nama Barang: %d\n", keranjang[i].id_barang);
        fprintf(file, "Jumlah: %d\n", keranjang[i].jumlah);
        fprintf(file, "Harga Satuan: Rp.%d\n", keranjang[i].harga);
        fprintf(file, "Harga Total: Rp.%d\n", totalHargaBarang);
        fprintf(file, "-----------------------\n");
    }
    fprintf(file, "\n");

    fclose(file); // Tutup file setelah selesai
    printf("Pesanan berhasil disimpan ke file %s.\n", pemesanan);
}
