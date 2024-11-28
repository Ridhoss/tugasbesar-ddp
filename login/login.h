#ifndef LOGIN_H
#define LOGIN_H

// Record akun
struct Akun {
    int id;
    char username[50];
    char password[50];
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
void CariAkun(char *username, char *password, int *role, int idLogin);

#endif