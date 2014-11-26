#include "customer-database.h"
#include <string.h>
#include <stdio.h>

int binarySearch(CDB cdb, long key, int low, int high) {
    if (high < low) {
        return -1;
	}
	int mid;
    mid = low + ((high - low) / 2);
    if (cdb->dbarray[mid].id == key) {
		return mid;
	} else if (cdb->dbarray[mid].id < key) {
		return binarySearch(cdb, key, (mid + 1), high);
	} else if (cdb->dbarray[mid].id > key) {
		return binarySearch(cdb, key, low, (mid - 1));
	}
	return -1;
}

int customercomp(const void *a, const void *b) {
	Customer *tempa = (Customer *)a;
	Customer *tempb = (Customer *)b;
	if (tempa->id - tempb->id > 0) {
		return 1;
	}
	else if (tempa->id - tempb->id < 0) {
		return -1;
	}
	else {
		return 0;
	}
} 

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
	// Creating and mallocing the CDB to be returned via the pointer cdb.
	CDB cdb;
	cdb = (struct customerdatabase *)malloc(sizeof(struct customerdatabase));
	// If malloc fails print an error statement and return NULL.
	if (cdb == NULL) {
		printf("Error: Out of memory.\n");
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

int CDInsert(CDB cdb, Customer *cust) {
	if (cdb == NULL) {
		printf("Error: Customer DataBase is NULL.\n");
		return 0;
	}
	if (cust == NULL) {
		printf("Error: Customer in insert is NULL.\n");
		return 0;
	}
	if (cdb->numCust == cdb->dbSize) {
		cdb->dbarray = realloc(cdb->dbarray, 2 * cdb->dbSize);
		if (cdb->dbarray == NULL) {
			printf("Error: Realloc of DataBase is NULL.\n");
			return 0;
		}
		cdb->dbSize = 2 * cdb->dbSize;
	}
	if (pthread_mutex_init(&(cdb->dbarray[cdb->numCust].queue_mutex), 0) != 0) {
		printf("Error: Cannot initialize mutex.\n");
	}
	cdb->dbarray[cdb->numCust].name = cust->name;
	cdb->dbarray[cdb->numCust].id = cust->id;
	cdb->dbarray[cdb->numCust].balance = cust->balance;
	cdb->dbarray[cdb->numCust].address = cust->address;
	cdb->dbarray[cdb->numCust].state = cust->state;
	cdb->dbarray[cdb->numCust].zip = cust->zip;
	cdb->dbarray[cdb->numCust].slist = NULL;
	cdb->dbarray[cdb->numCust].rlist = NULL;
	cdb->numCust++;
	printf("IN CUSTOMER.c\n\n");
	printCustomer(&(cdb->dbarray[cdb->numCust - 1]));
	//free(cust);
	return 1;
}

int CDUpdate(CDB cdb, QNode *order, int indx) {
	int index;
	Customer *cust;
	index = indx;
	if (index < 0) {
		printf("Error: customer not found in CDUpdate.");
		return 0;
	}
	cust = &(cdb->dbarray[index]);
	if (order->price > cdb->dbarray[index].balance) {
		ROrder *rejected;
		char *bookname;
		rejected = (ROrder *)malloc(sizeof(ROrder));
		bookname = (char *)malloc(sizeof(order->bname) + 1);
		strcpy(bookname, order->bname);
		bookname[strlen(bookname)] = '\0';
		rejected->bname = bookname;
		rejected->price = order->price;
		rejected->next = NULL;
		if (cdb->dbarray[index].rlist == NULL) {
			cdb->dbarray[index].rlist = rejected;
			cdb->dbarray[index].rlistback = rejected;
		} 
		else {
			cdb->dbarray[index].rlistback->next = rejected;
			cdb->dbarray[index].rlistback = rejected;
		}
	}
	else {
		SOrder *accepted;
		char *bookname;
		accepted = (SOrder *)malloc(sizeof(SOrder));
		bookname = (char *)malloc(sizeof(order->bname) + 1);
		strcpy(bookname, order->bname);
		bookname[strlen(bookname)] = '\0';
		accepted->bname = bookname;
		accepted->price = order->price;
		accepted->updatedbalance = cdb->dbarray[index].balance - order->price;
		accepted->next = NULL;
		if (cdb->dbarray[index].slist == NULL) {
			cdb->dbarray[index].slist = accepted;
			cdb->dbarray[index].slistback = accepted;
		} 
		else {
			cdb->dbarray[index].slistback->next = accepted;
			cdb->dbarray[index].slistback = accepted;
		}
		cdb->dbarray[index].balance = accepted->updatedbalance;
		pthread_mutex_lock(&cdb->revenuemutex);
		cdb->revenue += order->price;
		pthread_mutex_unlock(&cdb->revenuemutex);
	}
	return 1;
}

void PrintDB(CDB cdb) {
	if (cdb == NULL) {
		printf("Error: CDB is NULL.\n");
		return;
	}
	printf("=== TOTAL REVENUE GENERATED: %f ===\n\n", cdb->revenue);
	int i;
	for (i = 0; i < cdb->numCust; i++) {
		printf("=== BEGIN CUSTOMER INFO ===\n");
		printf("### BALANCE ###\n");
		printf("Customer name: %s\n", cdb->dbarray[i].name);
		printf("Customer ID number: %ld\n", cdb->dbarray[i].id);
		printf("Remaining credit balance after all purchases (a dollar amount): %f\n", cdb->dbarray[i].balance);
		printf("### SUCCESSFUL ORDERS ###\n");
		if (cdb->dbarray[i].slist != NULL) {
			SOrder *curr;
			curr = cdb->dbarray[i].slist;
			while (curr != NULL) {
				printf("%s|%f|%f\n", curr->bname, curr->price, curr->updatedbalance);
				curr = curr->next;
			}
		}
		else {
			printf("=== EMPTY LIST ===\n");
		}
		printf("### REJECTED ORDERS ###\n");
		if (cdb->dbarray[i].rlist != NULL) {
			ROrder *curr;
			curr = cdb->dbarray[i].rlist;
			while (curr != NULL) {
				printf("%s|%f\n", curr->bname, curr->price);
				curr = curr->next;
			}
		}
		else {
			printf("=== EMPTY LIST ===\n");
		}
		printf("=== END CUSTOMER INFO ===\n");
		printf("\n\n");
	}
}

void printCustomer(Customer *dude){
	printf("---PRINTING CUSTOMER %s---\n", dude->name);
	printf("Customer Name: %s\n", dude->name);
	printf("Customer ID: %ld\n", dude->id);
	printf("Customer Balance: %f\n", dude->balance);
	printf("Customer Address: %s\n", dude->address);
	printf("Customer State: %s\n", dude->state);
	printf("Customer Zip: %s\n", dude->zip);
	printf("---END PRINTING CUSTOMER---\n");
}
