#include <stdio.h>
#include <string.h>
#include "masuk.h"

// Deklarasi global
const char *users = "database/users.txt";


//* Baca file akun
int BacaFileAkun(struct Akun *akun) {
    FILE *file = fopen(users, "r");
    if (file == NULL) return 0;
    int index = 0;
    while (fscanf(file, "%d,%49[^,],%49[^,],%d\n", &akun[index].id, akun[index].username, akun[index].password, &akun[index].role) == 4) {
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

    // Tambahkan data pengguna ke file
    file = fopen(users, "a");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    if (role == 1)
    {
        fprintf(file, "%d,%s,%s,%d\n", last_id + 1, username, password, 1);
    }else if(role == 2) {
        fprintf(file, "%d,%s,%s,%d\n", last_id + 1, username, password, 2);
    }else {
        fprintf(file, "%d,%s,%s,%d\n", last_id + 1, username, password, 3);
    }
    fclose(file);

    printf("Registrasi berhasil!\n");
}

//* Procedure login
void Login(int *loggedIn, int *idLogin) {
    char username[50];
    char password[50];
    struct Akun akun[100];
    int total_akun = BacaFileAkun(akun);

    printf("=======================\n");
    printf("MASUK\n");
    printf("=======================\n");

    for (;;) {
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

        printf("=======================\n");
        printf("Username atau Password salah!\n");   
        printf("=======================\n");
    }
}

//* Procedure daftar
void Daftar() {
    int pilihan;

    printf("=======================\n");
    printf("=== Daftar ===\n");
    printf("=======================\n");
    printf("1. Buat Akun User\n");
    printf("2. Buat Akun Penjual\n");
    printf("3. Buat Akun Kurir\n");
    printf("Pilih : ");
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        DaftarAkun(1);
    }else if(pilihan == 2){
        DaftarAkun(2);
    }else if(pilihan == 3) {
        DaftarAkun(3);
    }else {
        printf("Error 404. Input Tidak Diketahui\n");
    }
}

//* Procedure masuk
void Masuk(int *loggedIn, int *idLogin) {
    int masuk;

    printf("===========================\n");
    printf("=== Selamat Datang Di E-Commerce ===\n");
    printf("===========================\n");

    printf("Apakah Sudah Memiliki Akun?\n");
    printf("1. Masuk\n");
    printf("2. Daftar\n");
    printf("3. Keluar Dari Aplikasi\n");
    printf("Pilih : ");
    scanf("%d", &masuk);

    if (masuk == 1) {
        Login(loggedIn, idLogin);
    } else if (masuk == 2) {
        Daftar();
        Login(loggedIn, idLogin);
    } else if (masuk == 3)
    {   
        *loggedIn = 2;
    } else {
        printf("Error 404. Input Tidak Diketahui\n");
    }
}

// Procedure CariAkun
void CariAkun(int idLogin, char *username, char *password, int *role) {
    struct Akun akun[100];
    int totalAkun;

    totalAkun = BacaFileAkun(akun);
    for (int i = 0; i < totalAkun; i++) {
        if (akun[i].id == idLogin) {
            *role = akun[i].role;
            strcpy(username, akun[i].username);
            strcpy(password, akun[i].password);
            break;
        }
    }
}

// procedure Logout
void Logout(int *con){
    *con = 0;
}