#include "consumer-database.h"
#include <string.h>
#include <stdio.h>

int binarySearch(CSA csa, char *key, int low, int high) {
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

int consumercomp(const void *a, const void *b) {
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

CSA CSACreate() {
	// Creating and mallocing the CSA to be returned via the pointer csa.
	CSA csa;
	csa = (CSA *)malloc(sizeof(CSA));
	// If malloc fails print an error statement and return NULL.
	if (csa == NULL) {
		printf("Error: Out of memory.\n");
		return NULL;
	}
	csa->consumerdata = (ConsumerStruct *)malloc(10 * sizeof(ConsumerStruct));
	csa->numCons = 0;
	csa->size = 10;
	return csa;
}

int CSADestroy(CSA csa) {
	if (csa == NULL) {
		printf("Error: CSA is NULL.\n");
		return 0;
	}
	int i;
	for (i = 0; i < csa->numCons; i++) {
		free(csa->consumerdata[csa->numCons].category);
		QDestroy(csa->consumerdata[csa->numCons].q);
	}
	return 1;
}

int CSAInsert(CSA csa, char *category) {
	if (csa == NULL) {
		printf("Error: Consumer Struct Array is NULL.\n");
		return 0;
	}
	if (category == NULL) {
		printf("Error: Category name is NULL.\n");
		return 0;
	}
	if (csa->numCons == csa->size) {
		csa->consumerdata = realloc(csa->consumerdata, 2 * csa->size);
		if (csa->consumerdata == NULL) {
			printf("Error: Realloc of Array is NULL.\n");
			return 0;
		}
		csa->size = 2 * csa->size;
	}
	csa->consumerdata[csa->numCons].category = category;
	csa->consumerdata[csa->numCons].q = QCreate();
	csa->consumerdata[csa->numCons].mutex = pthread_mutex_init(&(csa->consumerdata[csa->numCons].mutex), 0);
	csa->consumerdata[csa->numCons].empty = pthread_cond_init(&(csa->consumerdata[csa->numCons].empty), 0);
	csa->consumerdata[csa->numCons].full = pthread_cond_init(&(csa->consumerdata[csa->numCons].full), 0);
	csa->numCons++;
	return 1;
}
