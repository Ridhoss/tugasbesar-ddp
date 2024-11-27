#include <stdio.h>
#include <string.h>

// Record akun
struct Akun {
    int id;
    char username[50];
    char password[5];
    int role;
};

// Deklarasi global
const char *users = "users.txt";

//* Procedure daftar user
void DaftarUser() {
    char username[50];
    char password[50];
    char password_check[50];
    int cek = 0;
    int last_id = 0;

    // cari id terakhir
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

    for(;;){
        printf("Masukan Username : ");
        scanf("%s", username);
        printf("=======================\n");

        int duplicate_check = 0;

        // cari username
        FILE *file = fopen(users, "r");
        if (file != NULL) {
            char line[200];
            while (fgets(line, sizeof(line), file)) {
                char file_username[50];
                sscanf(line, "%*d,%49[^,]", &file_username);
                if (strcmp(username, file_username) == 0) {
                    duplicate_check = 1;
                    break;
                }
            }
            fclose(file);
        }

        if (duplicate_check == 0)
        {
            break;
        }else {
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

    fprintf(file, "%d,%s,%s,%d\n", last_id + 1, username, password,1);
    fclose(file);

    printf("Registrasi berhasil!");
}

//* Procedure daftar
void Daftar() {
    int pilihan;

    printf("=======================\n");
    printf("=== Daftar ===\n");
    printf("=======================\n");
    printf("1. Buat Akun\n");
    printf("2. Buat Akun Penjual\n");
    printf("Pilih : ");
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        DaftarUser();
    } else if (pilihan == 2) {
        printf("Fitur untuk penjual belum tersedia.\n");
    } else {
        printf("Error 404. Input Tidak Diketahui\n");
    }
}

//* Procedure masuk
void Masuk(int *loggedIn, int *idLogin) {
    int masuk;

    printf("Apakah Sudah Memiliki Akun?\n");
    printf("1. Masuk\n");
    printf("2. Daftar\n");
    printf("Pilih : ");
    scanf("%d", &masuk);

    if (masuk == 1) {
        //! Procedure login
        printf("Fitur login belum tersedia.\n");
    } else if (masuk == 2) {
        Daftar();
    } else {
        printf("Error 404. Input Tidak Diketahui\n");
    }
}

//* procedure login 


//* Main Program
int main() {
    int loggedIn = 0, idLogin = -1;

    printf("=======================\n");
    printf("=== Selamat Datang ===\n");
    printf("=======================\n");

    Masuk(&loggedIn, &idLogin);

    return 0;
}





//* Function aksi login
// int aksiLogin(char inputNama[50], char inputPassword[5]) {
//     for (int i = 0; i < sizeof(akun) / sizeof(akun[0]); i++) {
//         if (strcmp(inputNama, akun[i].nama) == 0 && strcmp(inputPassword, akun[i].password) == 0) {
//             printf("Login berhasil!\n");

//             if (akun[i].role == 0) {
//                 printf("Selamat datang, Admin!\n");
//                 return 2;
//             } else if (akun[i].role == 1) {
//                 printf("Selamat datang, %s!\n", akun[i].nama);
//                 return 1;
//             }
//         }
//     }
// }

//* Fungsi utama
// int main() {
//     char inputNama[50];
//     char inputPassword[5];
//     int loggedIn = 0;

//     printf("=== Selamat Datang ===\n");
//     // input login
//     printf("Masukkan nama: ");
//     scanf("%s", inputNama);
//     printf("Masukkan password: ");
//     scanf("%s", inputPassword);
//     printf("=======================\n");

//     // login
//     loggedIn = aksiLogin(inputNama, inputPassword);

//     if (loggedIn == 2) {
//         // untuk admin
//         printf("admin.\n");
//     }else if(loggedIn == 1){
//         // untuk user
//         printf("User.\n");
//     }else {
//         // gagal login
//         printf("Login gagal! Nama atau password salah.\n");
//     }

//     return 0;
// }

