#include "customer-database.h"
#include <string.h>
#include <stdio.h>

int SLDestroy(SOrder *front){
	if (front == NULL) {
		printf("Error: SList is NULL.\n");
		return 0;
	}
	SOrder *curr;
	SOrder *temp;
	curr = front;
	while (curr != NULL) {
		free(curr->bname);
		temp = curr;
		curr = curr->next;
		free(temp);
	}
	return 1;
}

int RLDestroy(ROrder *front){
	if (front == NULL) {
		printf("Error: RList is NULL.\n");
		return 0;
	}
	ROrder *curr;
	ROrder *temp;
	curr = front;
	while (curr != NULL) {
		free(curr->bname);
		temp = curr;
		curr = curr->next;
		free(temp);
	}
	return 1;
}

CDB CDCreate() {
	// Creating and mallocing the cdb to be returned via the pointer sl.
	CDB cdb;
	cdb = (struct customerdatabase *)malloc(sizeof(struct customerdatabase));
	// If malloc fails print an error statement and return NULL.
	if (cdb == NULL) {
		//printf("Error: Out of memory.\n");
		return NULL;
	}
	cdb->dbarray = (Customer *)malloc(10 * sizeof(Customer));
	cdb->numCust = 0;
	cdb->dbSize = 10;
	return cdb;
}

int CDDestroy(CDB cdb) {
	if (cdb == NULL) {
		printf("Error: CDB is NULL.\n");
		return 0;
	}
	int i;
	int flag = 0;
	for (i = 0; i < cdb->numCust; i++) {
		free(cdb->dbarray[cdb->numCust].name);
		free(cdb->dbarray[cdb->numCust].address);
		free(cdb->dbarray[cdb->numCust].state);
		free(cdb->dbarray[cdb->numCust].zip);
		if (!(flag = SLDestroy(cdb->dbarray[cdb->numCust].slist))) {
			printf("Error: Cannot free SList.\n");
		}
		if (!(flag = RLDestroy(cdb->dbarray[cdb->numCust].rlist))) {
			printf("Error: Cannot free RList.\n");
		}
	}
	return 1;
}

int CDInsert(CDB cdb, struct customer *cust) {
	if (cdb == NULL) {
		printf("Error: CDB is NULL.\n");
		return 0;
	}
	if (cust == NULL) {
		printf("Error: Customer in insert is NULL.\n");
		return 0;
	}
	if (cdb->numCust == cdb->dbSize) {
		cdb->dbarray = realloc(cdb->dbarray, 2 * cdb->dbSize);
		if (cdb->dbarray == NULL) {
			printf("Error: DB is NULL.\n");
		return 0;
		}
		cdb->dbSize = 2 * cdb->dbSize;
	}
	cdb->dbarray[cdb->numCust].name = cust->name;
	cdb->dbarray[cdb->numCust].id = cust->id;
	cdb->dbarray[cdb->numCust].credit = cust->credit;
	cdb->dbarray[cdb->numCust].address = cust->address;
	cdb->dbarray[cdb->numCust].state = cust->state;
	cdb->dbarray[cdb->numCust].zip = cust->zip;
	cdb->dbarray[cdb->numCust].slist = NULL;
	cdb->dbarray[cdb->numCust].rlist = NULL;
	return 1;
}

int CDUpdate(CDB cdb, char *bookname, double price) {
	return 1;
}

void PrintDB(CDB cdb) {
	
}
