#ifndef PEMBELI_H
#define PEMBELI_H

#include <stdio.h>
#include "../penjual/penjual.h"
#include "../masuk/masuk.h"

typedef struct {
    int id;
    int id_barang;      
    int harga;        
    int jumlah;    
    int id_pembeli;
} Keranjang;


extern const char *file_products;

void halamanUser(int *loggedIn, int idLogin);

void formatRibuan(int angka, char *output);

void BeliBarang(int idLogin, Product products[], int maxProducts);

int bacaFileProduk(Product products[], int maxProducts);

void tampilkanKeranjangDariFile(int idLogin);

int bacaKeranjangDariFile(Keranjang keranjang[], int idLogin);

void tampilkanKatalog(Product products[], int jumlahProduk, int idLogin);

void tambahKeKeranjang(int idLogin, Keranjang keranjang[], int *keranjangCount, Product products[], int pilihanProduk, int jumlahBeli);

void simpanKeranjangKeFile(Keranjang keranjang[], int keranjangCount);

void checkout(int idLogin);

void tulisPesanan(int idLogin, Keranjang *barangCheckout, int barangCount, int ongkir);

void upStokProduct(int idBarang, int jumlah);

void hapusDariKeranjang(int idLogin);

void tampilkanPesananUser(int idLogin);

void transferAfterConfirm(int idLogin, int idPes);

void updateSaldoUser(Akun akun[], int count);

int generateIdKeranjang();

#endif
