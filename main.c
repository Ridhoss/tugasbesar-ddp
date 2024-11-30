#include <stdio.h>
#include <string.h>
#include "function/penjual/penjual.h"
#include "function/masuk/masuk.h"

// Main Program
int main() {
    int loggedIn = 0, idLogin = 0, role =0;
    char username[50];
    char password[50];
    char store_name[50];

    for (;;) {
        Masuk(&loggedIn, &idLogin);

        if (loggedIn == 1) {
            // mencari akun berdasarkan id yang login
            CariAkun(idLogin, username, password, &role, store_name);
            if (role == 2)
            {
                halamanAdmin(&loggedIn, idLogin, username, password, role, store_name);
            }
        } else if(loggedIn == 2){
            Clear(&idLogin, username, password, &role, store_name);
            printf("Terima kasih telah menggunakan program ini.\n");
            break;
        } 
    }

    // untuk cek
    printf("Username = %s\n", username);
    printf("Password = %s\n", password);
    printf("Store = %s\n", store_name);
    printf("Role = %d\n", role);
    printf("=======================\n");


    return 0;
}

