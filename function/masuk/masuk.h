#ifndef MASUK_H
#define MASUK_H

// Record akun
struct Akun {
    int id;
    char username[50];
    char password[50];
    char phone[14];
    char alamat[50];
    char store_name[50];
    int role;
};

//* Baca file akun
int BacaFileAkun(struct Akun *akun);
//* Procedure daftar user
void DaftarAkun(int role);
//* Procedure login
void Login(int *loggedIn, int *idLogin);
//* Procedure daftar
void Daftar();
//* Procedure masuk
void Masuk(int *loggedIn, int *idLogin);
//* Procedure CariAkun
void CariAkun(int idLogin, char *username, char *password, char *phone, char *alamat, int *role, char *store_name);
//* procedure logout
void Logout(int *con);
//* Clear Account
void Clear(int *idLogin, char *username, char *password, char *phone, char *alamat, int *role, char *store_name);

#endif