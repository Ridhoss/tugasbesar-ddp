#ifndef TOPUP_H
#define TOPUP_H

extern const char *users;

void CekRekening(int idLogin);
int HanyaTampilkanSaldo(int idLogin);
void UpdateSaldo(int idLogin, int tambahanSaldo);
void TopUp(int idLogin);


#endif
