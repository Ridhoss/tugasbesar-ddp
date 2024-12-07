#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kurir.h"
#include "../penjual/penjual.h"
#include "../masuk/masuk.h"
#include "../topup/topup.h"
#include "../pembeli/pembeli.h"

void halamanKurir(int *loggedIn, int idLogin) {
    char username[50], password[50], phone[16], alamat[50];
    int rekening, role;
    Pesanan pesanan[100];

    // Ambil data akun kurir
    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, NULL);

    int conlog = *loggedIn;

    printf("===========================\n");
    printf("==== Selamat Datang %s ====\n", username); // Gunakan username untuk kurir
    printf("===========================\n");

    int choice;
    do {
        printf("====== Halaman Kurir ======\n");
        printf("===========================\n");
        printf("1. Lihat Pesanan\n");
        printf("2. Konfirmasi Pengiriman\n");
        printf("3. Cek Rekening\n");
        printf("4. Logout\n");
        printf("Masukan pilihan:(1-4) ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                lihatPesananKurir(pesanan, idLogin); // Panggil fungsi dengan idLogin
                break;
            case 2:
                updateStatusPengiriman(pesanan, idLogin); // Tambahkan idLogin sebagai parameter
                break;
            case 3:
                CekRekening(idLogin);
                break;
            case 4:
                Logout(&conlog);
                *loggedIn = conlog;
                break;
            default:
                printf("Pilihan tidak ada\n");
        }
    } while (*loggedIn && choice != 4); // Keluar jika logout
}

// Fungsi untuk menyimpan data pesanan ke file
void simpanFilePesanan(Pesanan pesanan[], int count) {
    FILE *file = fopen("database/pemesanan.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file pemesanan.txt untuk menulis.\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%d,%d,%d,%s,%d,%d,%d,%d,%d,%s,%s,%s,%s\n",
                pesanan[i].id_pesanan, pesanan[i].nomorPesanan, pesanan[i].id_pembeli,
                pesanan[i].id_penjual, pesanan[i].id_kurir, pesanan[i].tanggalPesanan,
                pesanan[i].id_barang, pesanan[i].jumlah, pesanan[i].harga, pesanan[i].ongkir,
                pesanan[i].total, pesanan[i].alamat, pesanan[i].expedisi,
                pesanan[i].status_pembayaran, pesanan[i].status_pengiriman);
    }

    fclose(file);
    printf("Data pesanan berhasil disimpan.\n");
}

// Fungsi untuk melihat pesanan berdasarkan ID kurir
void lihatPesananKurir(Pesanan pesanan[], int idKurir) {
    int count = bacaFilePesanan(pesanan);
    int found = 0;

    printf("=========================================\n");
    printf("--- Daftar Pesanan untuk Kurir ID: %d ---\n", idKurir);
    printf("=========================================\n");

    for (int i = 0; i < count; i++) {
        if (pesanan[i].id_kurir == idKurir) {
            if (strcmp(pesanan[i].status_pengiriman,"Pesanan Selesai") != 0)
            {
                char pesananTanggal[3], pesananBulan[3], pesananTahun[5];
                int pTanggal, pBulan, pTahun;
                char totalOngkirFor[100];

                strncpy(pesananTanggal, pesanan[i].tanggalPesanan, 2);
                strncpy(pesananBulan, pesanan[i].tanggalPesanan + 2, 2);
                strncpy(pesananTahun, pesanan[i].tanggalPesanan + 4, 4);
                pesananTanggal[2] = pesananBulan[2] = pesananTahun[4] = '\0';
                pTanggal = atoi(pesananTanggal);
                pBulan = atoi(pesananBulan);
                pTahun = atoi(pesananTahun);
                formatRibuan(pesanan[i].ongkir, totalOngkirFor);

                // Menampilkan data pesanan
                printf("Barang %d:\n", i + 1);
                printf("  ID Pesanan        : %d\n", pesanan[i].id_pesanan);
                printf("  Nomor Pesanan     : %s\n", pesanan[i].nomorPesanan);
                printf("  Tanggal           : %02d-%02d-%04d\n", pTanggal, pBulan, pTahun);
                printf("  Ongkir            : Rp.%s\n", totalOngkirFor);
                printf("  Status Pengiriman : %s\n", pesanan[i].status_pengiriman);
                printf("----------------------------\n");
                found = 1;
            }
        }
    }

    if (!found) {
        printf("Tidak ada pesanan untuk kurir ini.\n");
    }
}

// Fungsi untuk memperbarui status pengiriman
void updateStatusPengiriman(Pesanan pesanan[], int idKurir) {
    int count = bacaFilePesanan(pesanan);

    printf("\nMasukkan ID Pesanan yang ingin diperbarui status pengirimannya: ");
    int idPesanan;
    scanf("%d", &idPesanan);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (pesanan[i].id_pesanan == idPesanan && pesanan[i].id_kurir == idKurir) {
            printf("Pesanan ditemukan: ID %d, Status saat ini: %s\n", idPesanan, pesanan[i].status_pengiriman);

            printf("Masukkan status pengiriman baru (contoh: Paket sudah diantar): ");
            char statusBaru[MAX_STRING];
            getchar(); // Menghapus newline dari buffer
            fgets(statusBaru, MAX_STRING, stdin);
            statusBaru[strcspn(statusBaru, "\n")] = '\0'; // Menghapus newline di akhir

            strcpy(pesanan[i].status_pengiriman, statusBaru);
            simpanFilePesanan(pesanan, count);

            printf("Status pengiriman berhasil diperbarui menjadi: %s\n", pesanan[i].status_pengiriman);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Pesanan dengan ID %d tidak ditemukan untuk kurir ID %d.\n", idPesanan, idKurir);
    }
}
