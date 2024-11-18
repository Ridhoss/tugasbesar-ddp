#include <stdio.h>
#include <string.h>

// Record barang
struct Barang {
    char nama[50];
    int harga;
};

// Prosedur untuk memformat angka dengan pemisah ribuan
void formatRibuan(int angka, char *output) {
    char angkas[20];
    
    // Konversi angka ke string
    sprintf(angkas, "%d", angka); 
    int len = strlen(angkas);
    int sisa = len % 3;

    int pos = 0;

    // Tambahkan karakter sebelum tanda pemisah ribuan
    for (int i = 0; i < len; i++) {
        if (i > 0 && (i - sisa) % 3 == 0) {
            output[pos++] = '.';
        }
        output[pos++] = angkas[i];
    }

    output[pos] = '\0';
}

int main() {
    // Inisialisasi barang
    struct Barang barang[] = {
        {"Keyboard", 500000},
        {"Mouse", 200000},
        {"Laptop", 10000000}
    };

    char hargaFormatted[20]; // Variabel untuk menyimpan hasil format harga

    // Menampilkan data barang
    printf("====================\n");
    for (int i = 0; i < 3; i++) {
        formatRibuan(barang[i].harga, hargaFormatted); // Format harga
        printf("Barang %d\n", i + 1);
        printf("Nama: %s\n", barang[i].nama);
        printf("Harga: Rp.%s\n", hargaFormatted); // Cetak harga yang sudah diformat
        printf("====================\n");
    }

    return 0;
}
