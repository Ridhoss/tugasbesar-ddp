#ifndef PESANAN_H
#define PESANAN_H

#include <stdio.h>
extern const char *file_pemesanan;;

typedef struct {
    int id_pesanan; 
    char nomorPesanan[20];
    int id_pembeli;
    int id_penjual;
    int id_kurir;
    char tanggalPesanan[10];
    int id_barang;
    int jumlah;
    int harga;
    int total;
    char expedisi[20];
    char alamat[50];
    char metode_pengiriman[20];
    char status_pembayaran[20]; // 0 = Belum Dikonfirmasi, 1 = Dikonfirmasi
    char status_pesanan[20];
    char status_pengiriman[20];
} Pesanan;

#endif
