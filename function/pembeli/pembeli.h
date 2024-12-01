#ifndef PEMBELI_H
#define PEMBELI_H

#include <stdio.h>
#include "penjual.h"

// Struktur untuk menyimpan informasi dalam keranjang
typedef struct {
    char nama[50];      
    float harga;        
    int jumlah;         // Jumlah barang dalam keranjang
} Keranjang;

void halamanUser(int *loggedIn, int idLogin, char *username, char *password, char *phone, char *alamat, int role, char *store_name);
int bacaFileProduk(const char *filename, Product products[], int maxProducts);
void tampilkanKatalog(Product products[], int jumlahProduk);
void formatRibuan(int angka, char *output);
void tambahKeKeranjang(Keranjang keranjang[], int *keranjangCount, Product products[], int pilihanProduk, int jumlahBeli);
void tampilkanKeranjang(Keranjang keranjang[], int keranjangCount);
void checkout(Keranjang keranjang[], int keranjangCount, int *statusPesanan, 
              const char *pembeli, const char *kontak, const char *penjual, 
              const char *alamat, const char *ekspedisi);
void tulisPesanan(Keranjang keranjang[], int keranjangCount, const char *filename, 
                  const char *pembeli, const char *kontak, const char *penjual, 
                  const char *alamat, const char *ekspedisi, int statusPembayaran);
void bacaFilePesanan(const char *filename);
void hapusDariKeranjang(Keranjang keranjang[], int *keranjangCount);
void simpanPesanan(Keranjang keranjang[], int keranjangCount, const char *filename);
void konfirmasiPesanan(int *statusPesanan);
void statusPengiriman(int *pengiriman);

#endif
