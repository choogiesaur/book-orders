#ifndef CUSTOMER_DATABASE_H
#define CUSTOMER_DATABASE_H
/*
 * customer-database.h
 */

#include <stdlib.h>

struct sorder {
	char *bname;
	double price;
	double updatedbalance;
	struct sorder *next;
};
typedef struct sorder SOrder;

struct rorder {
	char *bname;
	double price;
	struct rorder *next;
};
typedef struct rorder ROrder;

struct customer {
	char *name;
	int id;
	double balance;
	char *address;
	char *state;
	char *zip;
	struct sorder *slist;
	struct rorder *rlist;
};
typedef struct customer Customer;

struct customerdatabase {
	int numCust;
	int dbSize;
	struct customer *dbarray;
};
typedef struct customerdatabase* CDB;

int SLDestroy(SOrder *front);

int RLDestroy(ROrder *front);

CDB CDCreate();

int CDDestroy(CDB cdb);

int CDInsert(CDB cdb, struct customer *cust);

int CDUpdate(CDB cdb, char *bookname, double price);

void PrintDB(CDB cdb);

#endif
