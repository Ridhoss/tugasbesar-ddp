#include <stdio.h>
#include <string.h>
#include "masuk.h"

// Deklarasi global
const char *users = "database/users.txt";


//* Baca file akun
int BacaFileAkun(Akun *akun) {
    FILE *file = fopen(users, "r");
    if (file == NULL) return 0;

    int index = 0;
    while (fscanf(file, "%d,%49[^,],%49[^,],%15[^,],%49[^,],%d,%d,%49[^\n]\n", 
                &akun[index].id, akun[index].username, 
                akun[index].password, akun[index].phone, 
                akun[index].alamat, &akun[index].rekening, &akun[index].role, 
                akun[index].store_name) == 8) {

        // Jika store_name kosong dan role 1, set default string "null"
        if (akun[index].role == 1 && akun[index].store_name[0] == '\0') {
            strcpy(akun[index].store_name, "null");
        }

        index++;
    }
    fclose(file);
    return index;
}

//* Procedure daftar user
void DaftarAkun(int role) {
    char username[50];
    char password[50];
    char password_check[50];
    char phone[14];
    char alamat[50] = "null";
    char store_name[50] = "null";
    int last_id = 0;

    // Cari id terakhir
    FILE *file = fopen(users, "r");
    if (file != NULL) {
        char line[200];
        while (fgets(line, sizeof(line), file)) {
            int temp_id;
            sscanf(line, "%d,", &temp_id);
            if (temp_id > last_id) {
                last_id = temp_id;
            }
        }
        fclose(file);
    }

    for (;;) {
        printf("Masukan Username : ");
        scanf("%s", username);
        printf("=======================\n");

        int duplicate_check = 0;

        // Cari username
        file = fopen(users, "r");
        if (file != NULL) {
            char line[200];
            while (fgets(line, sizeof(line), file)) {
                char file_username[50];
                sscanf(line, "%*d,%49[^,]", file_username);
                if (strcmp(username, file_username) == 0) {
                    duplicate_check = 1;
                    break;
                }
            }
            fclose(file);
        }

        if (duplicate_check == 0) {
            break;
        } else {
            printf("Username Sudah Digunakan!\n");
            printf("=======================\n");
        }
    }

    for (;;) {
        printf("Username : %s\n", username);
        printf("=======================\n");
        printf("Masukan Password : ");
        scanf("%s", password);
        printf("Konfirmasi Password : ");
        scanf("%s", password_check);

        if (strcmp(password, password_check) == 0) {
            break;
        } else {
            printf("=======================\n");
            printf("Konfirmasi Password Salah!\n");
            printf("=======================\n");
        }
    }

    printf("=======================\n");
    printf("Masukan Nomor Telepon : ");
    scanf("%s", phone);

    // Meminta alamat hanya jika bukan role 3
    if (role != 3) {
        printf("Masukan Alamat: ");
        getchar();
        fgets(alamat, sizeof(alamat), stdin);
        alamat[strcspn(alamat, "\n")] = '\0'; // Hapus newline
    }

    // Membaca nama toko (jika role == 2)
    if (role == 2) {
        printf("Masukkan Nama Toko: ");
        fgets(store_name, sizeof(store_name), stdin);
        store_name[strcspn(store_name, "\n")] = '\0'; // Hapus newline
    }

    // Tambahkan data pengguna ke file
    file = fopen(users, "a");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    if (role == 1){
        // Jika role adalah user biasa, simpan store_name sebagai "null"
        fprintf(file, "%d,%s,%s,%s,%s,%d,%d,%s\n", last_id + 1, username, password, phone, alamat, 0, 1, store_name); 
    }else if(role == 2) {
       // Jika role adalah penjual, simpan store_name sesuai input
        fprintf(file, "%d,%s,%s,%s,%s,%d,%d,%s\n", last_id + 1, username, password, phone, alamat, 0, 2, store_name); 
    }else {
        fprintf(file, "%d,%s,%s,%s,%s,%d,%d,%s\n", last_id + 1, username, password, phone, alamat, 0, 3, store_name);
    }
    fclose(file);

    printf("Registrasi berhasil!\n");
}

//* Procedure login
void Login(int *loggedIn, int *idLogin) {
    char username[50];
    char password[50];
    Akun akun[100];
    int total_akun = BacaFileAkun(akun);

    printf("=======================\n");
    printf("======== MASUK ========\n");
    printf("=======================\n");

        printf("Masukan Username: ");
        scanf("%s", username);
        printf("Masukan Password: ");
        scanf("%s", password);

        for (int i = 0; i < total_akun; i++) {
            if (strcmp(username, akun[i].username) == 0 && strcmp(password, akun[i].password) == 0) {
                *loggedIn = 1;
                *idLogin = akun[i].id;
                return;
            }
        }

        printf("=============================\n");
        printf("Username atau Password salah!\n");   
        printf("=============================\n");
    
}

//* Procedure daftar
void Daftar() {
    int pilihan;

    for (;;)
    {
        printf("=======================\n");
        printf("======== Daftar =======\n");
        printf("=======================\n");
        printf("1. Buat Akun User\n");
        printf("2. Buat Akun Penjual\n");
        printf("3. Buat Akun Kurir\n");
        printf("4. Kembali\n");
        printf("Pilih:(1/4) ");
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            DaftarAkun(1);
            break;
        }else if(pilihan == 2){
            DaftarAkun(2);
            break;
        }else if(pilihan == 3) {
            DaftarAkun(3);
            break;
        }else if(pilihan == 4) {
            break;
        }else {
            printf("Error 404. Input Tidak Diketahui\n");
        }
    }
}

//* Procedure masuk
void Masuk(int *loggedIn, int *idLogin) {
    int masuk = 0;

    for (;;)
    {
        printf("====================================\n");
        printf("=== Selamat Datang Di E-Commerce ===\n");
        printf("====================================\n");

        printf("Apakah Sudah Memiliki Akun?\n");
        printf("1. Masuk\n");
        printf("2. Daftar\n");
        printf("3. Keluar Dari Aplikasi\n");
        printf("Pilih:(1-3) ");
        scanf("%d", &masuk);

        if (masuk == 1) {
            Login(loggedIn, idLogin);
            break;
        } else if (masuk == 2) {
            Daftar();
        } else if (masuk == 3) {   
            *loggedIn = 2;
            break;
        } else {
            printf("Error 404. Input Tidak Diketahui\n");
        }
    }
}

// Procedure CariAkun
void CariAkun(int idLogin, char *username, char *password, char *phone, char *alamat, int *rekening, int *role, char *store_name) {
    Akun akun[100];
    int totalAkun;

    totalAkun = BacaFileAkun(akun);
    for (int i = 0; i < totalAkun; i++) {
        if (akun[i].id == idLogin) {
            *role = akun[i].role;
            *rekening = akun[i].rekening;
            strcpy(username, akun[i].username);
            strcpy(password, akun[i].password);
            strcpy(phone, akun[i].phone);
            strcpy(alamat, akun[i].alamat);
            if (*role == 2) {
                strcpy(store_name, akun[i].store_name); // Ambil store_name jika penjual
            }
            break;
        }
    }
}

// procedure Logout
void Logout(int *con){
    *con = 0;
}

void Clear(int *idLogin, char *username, char *password, char *phone, char *alamat, int *rekening, int *role, char *store_name){
    strcpy(username,"");
    strcpy(password,"");
    strcpy(store_name,"");
    strcpy(phone,"");
    strcpy(alamat,"");
    *rekening = 0;
    *role = 0;
    *idLogin = 0;
}