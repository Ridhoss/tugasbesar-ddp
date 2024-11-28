#ifndef PRODUCT_H
#define PRODUCT_H

typedef struct {
    int id;
    char name[50];
    char category[30];
    float price;
    int stock;
} Product;

void addProduct(Product *products, int *count, int max_count);
void listProducts(Product *products, int count);
void saveProducts(Product *products, int count);
int loadProducts(Product *products, int max_count);
void editProduct(Product *products, int count);
void deleteProduct(Product *products, int *count);

#endif
