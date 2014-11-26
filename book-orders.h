#ifndef BOOK_ORDERS_H
#define BOOK_ORDERS_H
/*
 * book-orders.h
 */

CSA csa;

CDB cdb;

CDB read_customers(CDB cdb, char *filename);

CSA read_categories(CSA csa, char *filename);

void *producer(void *fn);

void *consumer(void *cs);

#endif

