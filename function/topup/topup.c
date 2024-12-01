#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topup.h"
#include "../masuk/masuk.h"

void CekRekening(int idLogin) {
    char username[50], password[50], phone[16], alamat[50], store_name[50];
    int rekening, role;

    CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);

    int pilih = 0;

    printf("===========================\n");
    printf("Saldo Rekening Anda = %d\n", rekening);
    printf("===========================\n");

    for (;;) {
        printf("1. Topup Rekening\n");
        printf("2. Kembali Ke Halaman Admin\n");
        printf("Pilih : \n");
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


// Fungsi untuk memperbarui saldo pengguna di file
void UpdateSaldo(int idLogin, int tambahanSaldo) {
    struct Akun akun[100];
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

    // Tampilkan informasi pengguna
    printf("=======================\n");
    printf("Username: %s\n", username);
    printf("Saldo Saat Ini: %d\n", rekening);
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
