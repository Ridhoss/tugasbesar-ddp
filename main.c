#include <stdio.h>
#include <string.h>
#include "function/penjual/penjual.h"
#include "function/masuk/masuk.h"
#include "function/pembeli/pembeli.h"

// Main Program
int main() {
    int loggedIn = 0, idLogin = 0, role =0;
    char username[50];
    char password[50];
    char store_name[50];
    char phone[14];
    char alamat[50];

    for (;;) {
        Masuk(&loggedIn, &idLogin);

        if (loggedIn == 1) {
            // mencari akun berdasarkan id yang login
                halamanUser(&loggedIn, idLogin, username, password, phone, alamat, role, store_name);
            if (role == 2)
            {
                halamanAdmin(&loggedIn, idLogin, username, password, phone, alamat, role, store_name);
            }
        } else if(loggedIn == 2){
            Clear(&idLogin, username, password, password, phone, &role, store_name);
            printf("Terima kasih telah menggunakan program ini.\n");
            break;
        } 
    }

    // untuk cek
    printf("Username = %s\n", username);
    printf("Password = %s\n", password);
    printf("Store = %s\n", store_name);
    printf("Password = %s\n", phone);
    printf("Store = %s\n", alamat);
    printf("Role = %d\n", role);
    printf("=======================\n");


    return 0;
}

