#ifndef KURIR_H
#define KURIR_H

#include "../pesanan/pesanan.h"

#define MAX_PESANAN 100
#define MAX_STRING 50

// ke halaman admin
void halamanKurir(int *loggedIn, int idLogin);

void lihatPesananKurir(Pesanan pesanan[], int idKurir);

void updateStatusPengiriman(Pesanan pesanan[], int idKurir);

void simpanFilePesanan(Pesanan pesanan[], int count);

#endif
