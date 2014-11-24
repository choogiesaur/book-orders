#ifndef BOOK_ORDERS_H
#define BOOK_ORDERS_H
/*
 * book-orders.h
 */

//aint nuttin here yet...

CDB read_customers(CDB cdb, char *filename);

CSA read_categories(CSA csa, char *filename);

int read_orders(char *filename);

void printCustomer(Customer *dude);

#endif
