#include <stdio.h>
#include <string.h>
#include "function/penjual/penjual.h"
#include "function/masuk/masuk.h"
#include "function/pembeli/pembeli.h"
#include "function/kurir/kurir.h"

// Main Program
int main() {
    int loggedIn = 0, idLogin = 0, role =0;
    char username[50];
    char password[50];
    char store_name[50];
    char phone[14];
    char alamat[50];
    int rekening = 0;

    for (;;) {
        Masuk(&loggedIn, &idLogin);

        if (loggedIn == 1) {
            // mencari akun berdasarkan id yang login
            CariAkun(idLogin, username, password, phone, alamat, &rekening, &role, store_name);
            if(role == 1){
                halamanUser(&loggedIn, idLogin);
            }else if (role == 2)
            {
                halamanAdmin(&loggedIn, idLogin);
            }
            else if (role == 3)
            {
                halamanKurir(&loggedIn, idLogin);
            }
        } else if(loggedIn == 2){
            Clear(&idLogin, username, password, phone, alamat, &rekening, &role, store_name);
            printf("Terima kasih telah menggunakan program ini.\n");
            break;
        } 
    }

    // untuk cek
    printf("=======================\n");
    printf("Username = %s\n", username);
    printf("Password = %s\n", password);
    printf("Store = %s\n", store_name);
    printf("Password = %s\n", phone);
    printf("Store = %s\n", alamat);
    printf("Role = %d\n", role);
    printf("Rekening = %d\n", rekening);
    printf("=======================\n");


    return 0;
}

