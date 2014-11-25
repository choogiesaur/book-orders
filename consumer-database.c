#include "consumer-database.h"
#include "queue.h"
#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int binarySearch2(CSA csa, char *key, int low, int high) {
    if (high < low) {
        return -1;
	}
	int mid;
    mid = low + ((high - low) / 2);
    if (csa->consumerdata[mid].category == key) {
		return mid;
	} else if (strcmp(csa->consumerdata[mid].category, key) < 0) {
		return binarySearch2(csa, key, (mid + 1), high);
	} else if (strcmp(csa->consumerdata[mid].category, key) > 0) {
		return binarySearch2(csa, key, low, (mid - 1));
	}
	return -1;
}

int consumercomp(const void *a, const void *b) {
	ConsumerStruct *tempa = (ConsumerStruct *)a;
	ConsumerStruct *tempb = (ConsumerStruct *)b;
	if (strcmp(tempa->category, tempb->category) > 0) {
		return 1;
	}
	else if (strcmp(tempa->category, tempb->category) < 0) {
		return -1;
	}
	else {
		return 0;
	}
}

CSA CSACreate() {
	// Creating and mallocing the CSA to be returned via the pointer csa.
	CSA csa;
	csa = (struct consumer_struct_array *)malloc(sizeof(struct consumer_struct_array));
	// If malloc fails print an error statement and return NULL.
	if (csa == NULL) {
		printf("Error: Out of memory.\n");
		return NULL;
	}
	csa->consumerdata = (ConsumerStruct *)malloc(10 * sizeof(ConsumerStruct));
	csa->numCons = 0;
	csa->size = 10;
	csa->done = 0;
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
	if (pthread_mutex_init(&(csa->consumerdata[csa->numCons].mutex), 0) != 0) {
		printf("Error: Cannot initialize mutex.\n");
	}
	if (pthread_cond_init(&(csa->consumerdata[csa->numCons].dataAvailable), 0) != 0) {
		printf("Error: Cannot initialize conditional variable dataAvailable.\n");
	}
	if (pthread_cond_init(&(csa->consumerdata[csa->numCons].spaceAvailable), 0) != 0) {
		printf("Error: Cannot initialize conditional variable spaceAvailable.\n");
	}
	csa->numCons++;
	return 1;
}

void PrintCSA(CSA csa) {
	if (csa == NULL) {
		printf("Error: CSA is NULL.\n");
		return;
	}
	int i;
	for (i = 0; i < csa->numCons; i++) {
		printf("=== BEGIN CONSUMER INFO ===\n");
		printf("Consumer category: %s\n", csa->consumerdata[i].category);
		printf("=== END CONSUMER INFO ===\n");
		printf("\n\n");
	}
}
