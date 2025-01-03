#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topup.h"
#include "../masuk/masuk.h"
#include "../penjual/penjual.h"
#include "../pesanan/pesanan.h"
#include "../pembeli/pembeli.h"


void CekRekening(int idLogin) {
    char username[50], password[50], phone[16], alamat[50], store_name[50];
    int rekening, role;
    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);
    int pilih = 0;

    int saldo = HanyaTampilkanSaldo(idLogin);

    for (;;) {
        printf("1. Topup Rekening\n");
        printf("2. Kembali Ke Halaman Utama\n");
        printf("Pilih:(1-2) ");
        scanf("%d", &pilih);

        if (pilih == 1) {
            TopUp(idLogin);
            break;
        } else if (pilih == 2) {
            break;
        } else {
            printf("=======================\n");
            printf("Input Tidak Diketahui!\n");   
            printf("=======================\n");
        }
    }
}

int HanyaTampilkanSaldo(int idLogin) {
    char username[50], password[50], phone[16], alamat[50], store_name[50];
    int rekening, role;

    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);

    Pesanan pesanan[100];
    int pesananCount = bacaFilePesanan(pesanan);
    int totalHargaPesanan = 0;

    if (role == 1)
    {
        for (int i = 0; i < pesananCount; i++) {
            if (pesanan[i].id_pembeli == idLogin) {
                if (strcmp(pesanan[i].status_pengiriman,"Pesanan Selesai") != 0)
                {
                    totalHargaPesanan += pesanan[i].harga;
                }
            }
        }
    }

    int totalSaldo = rekening - totalHargaPesanan;

    // Format harga menjadi ribuan
    char totalSaldoFor[100];
    formatRibuan(totalSaldo, totalSaldoFor);

    printf("===========================\n");
    printf("Saldo Rekening Anda = Rp.%s\n", totalSaldoFor);

    return totalSaldo;
}


// Fungsi untuk memperbarui saldo pengguna di file
void UpdateSaldo(int idLogin, int tambahanSaldo) {
    Akun akun[100];
    int totalAkun = BacaFileAkun(akun);

    // Update saldo pada akun yang sesuai
    for (int i = 0; i < totalAkun; i++) {
        if (akun[i].id == idLogin) {
            akun[i].rekening += tambahanSaldo;
            break;
        }
    }

    // Simpan perubahan ke file
    FILE *file = fopen(users, "w");
    if (file != NULL) {
        for (int i = 0; i < totalAkun; i++) {
            fprintf(file, "%d,%s,%s,%s,%s,%d,%d,%s\n", 
                    akun[i].id, akun[i].username, akun[i].password, 
                    akun[i].phone, akun[i].alamat, akun[i].rekening, 
                    akun[i].role, akun[i].store_name);
        }
        fclose(file);
    }
}

void TopUp(int idLogin) {
    char username[50], password[50], phone[16], alamat[50], store_name[50];
    int rekening, role;

    // Cari data akun
    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);


    int saldo_sekarang = HanyaTampilkanSaldo(idLogin);

    // Format harga menjadi ribuan
    char totalSaldoSekarang[100];
    formatRibuan(saldo_sekarang, totalSaldoSekarang);

    // Tampilkan informasi pengguna
    printf("=======================\n");
    printf("Username: %s\n", username);
    printf("Saldo Saat Ini: %s\n", totalSaldoSekarang);
    printf("=======================\n");

    // Minta input jumlah top-up
    int jumlahTopUp;
    printf("Masukkan jumlah top-up: ");
    scanf("%d", &jumlahTopUp);

    if (jumlahTopUp <= 0) {
        printf("Jumlah top-up harus lebih besar dari 0!\n");
        return;
    }

    // Update saldo
    UpdateSaldo(idLogin, jumlahTopUp);

    printf("===========================\n");
    printf("Top-up berhasil! Saldo baru: %d\n", rekening + jumlahTopUp);
    printf("===========================\n");
}
