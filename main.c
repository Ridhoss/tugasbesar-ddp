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

    // insialisasi kegiatan
    int plhkgtn;

    // alogritma memilih kegiatan
    printf("====================\n");
    printf("Selamat Datang Di E-commerce kami\n");
    printf("1.Belanja\n");
    printf("2.Lihat Keranjang\n");
    printf("====================\n");
    printf("Pilih = ");
    scanf("%d", &plhkgtn);

    if(plhkgtn == 1){
        // algoritma menampilkan barang
        // Inisialisasi barang
        struct Barang barang[] = {
            {"Keyboard", 500000},
            {"Mouse", 200000},
            {"Laptop", 10000000}
        };
        char hargaFormatted[20];

        // Menampilkan data barang
        printf("====================\n");
        printf("SELAMAT BERBELANJA!\n");
        printf("====================\n");
        for (int i = 0; i < 3; i++) {
            formatRibuan(barang[i].harga, hargaFormatted);
            printf("Barang %d\n", i + 1);
            printf("Nama: %s\n", barang[i].nama);
            printf("Harga: Rp.%s\n", hargaFormatted);
            printf("====================\n");
        }

        // algoritma memilih barang
        // inisialisasi
        int plhbarang,jmlhbrg;

        printf("Pilih Barang yang akan dibeli = ");
        scanf("%d", &plhbarang);

        if(plhbarang == 1){
            printf("====================\n");
            formatRibuan(barang[0].harga, hargaFormatted);
            printf("Anda memilih = %s\n",barang[0].nama);
            printf("Harga: Rp.%s\n", hargaFormatted);
            printf("====================\n");
            printf("Masukan jumlah barang yang akan dibeli = ");             
            scanf("%d", &jmlhbrg);
        }else if(plhbarang == 2){
            printf("====================\n");
            formatRibuan(barang[1].harga, hargaFormatted);
            printf("Anda memilih = %s\n",barang[1].nama);
            printf("Harga: Rp.%s\n", hargaFormatted);
            printf("====================\n");
            printf("Masukan jumlah barang yang akan dibeli = ");             
            scanf("%d", &jmlhbrg);
        }else if(plhbarang == 3){
            printf("====================\n");
            formatRibuan(barang[2].harga, hargaFormatted);
            printf("Anda memilih = %s\n",barang[2].nama);
            printf("Harga: Rp.%s\n", hargaFormatted);
            printf("====================\n");
            printf("Masukan jumlah barang yang akan dibeli = ");             
            scanf("%d", &jmlhbrg);
        }else {
            printf("Maaf Input tidak diketahui!\n");
        }

    } else if (plhkgtn == 2){
        
    }else{
        printf("Maaf Input tidak diketahui!\n");
    }

    return 0;
}
