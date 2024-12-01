#ifndef PEMBELI_H
#define PEMBELI_H

#include <stdio.h>
#include "../penjual/penjual.h"

// Struktur untuk menyimpan informasi dalam keranjang
typedef struct {
    char nama[50];      
    int harga;        
    int jumlah;         // Jumlah barang dalam keranjang
} Keranjang;

extern const char *file_products;

void halamanUser(int *loggedIn, int idLogin);
void BeliBarang(int loggedIn, Product products[], int maxProducts);
int bacaFileProduk(Product products[], int maxProducts);
void tampilkanKatalog(Product products[], int jumlahProduk, Keranjang keranjang[], int keranjangCount);
void formatRibuan(int angka, char *output);
void tambahKeKeranjang(Keranjang keranjang[], int *keranjangCount, Product products[], int pilihanProduk, int jumlahBeli);
void tulisUlangFileProduk(Product products[], int jumlahProduk);
void tampilkanKeranjangDariFile();
void checkout(Keranjang keranjang[], int keranjangCount, int *statusPesanan, const char *pembeli, const char *kontak, const char *penjual, const char *alamat, const char *ekspedisi);
void tulisPesanan(Keranjang keranjang[], int keranjangCount, const char *pembeli, const char *kontak, const char *penjual, const char *alamat, const char *ekspedisi, int statusPembayaran);
void bacaFilePesanan();
void hapusDariKeranjang(Keranjang keranjang[], int *keranjangCount);
void simpanPesanan(Keranjang keranjang[], int keranjangCount);
void konfirmasiPesanan(int *statusPesanan);
void statusPengiriman(int *pengiriman);

#endif
