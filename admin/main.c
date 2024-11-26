    #include <stdio.h>
    #include "product.h"

    int main() {
        const int MAX_PRODUCTS = 100;
        Product products[MAX_PRODUCTS];
        int count = 0;

        count = loadProducts(products, MAX_PRODUCTS);

        int choice;
        do {
            printf("\n--- Admin ---\n");
            printf("1. Tambah Produk\n");
            printf("2. List Produk\n");
            printf("3. Simpan Produk\n");
            printf("4. Edit Produk\n");
            printf("5. Hapus Produk\n");
            printf("6. Selesai\n");
            printf("Masukan pilihan: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    addProduct(products, &count, MAX_PRODUCTS);
                    break;
                case 2:
                    listProducts(products, count);
                    break;
                case 3:
                    saveProducts(products, count);
                    break;
                case 4:
                    editProduct(products, count);
                    break;
                case 5:
                    deleteProduct(products, &count);
                    break;
                case 6:
                    printf("Keluar...\n");
                    break;
                default:
                    printf("Pilihan tidak ada\n");
            }
        } while (choice != 6);

        return 0;
    }
