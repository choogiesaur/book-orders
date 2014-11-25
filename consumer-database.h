#ifndef CONSUMER_DATABASE_H
#define CONSUMER_DATABASE_H
/*
 * consumer-database.h
 */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct consumer_struct {
	char *category;
	pthread_mutex_t mutex;
	pthread_cond_t dataAvailable;
	pthread_cond_t spaceAvailable;
	struct queue *q;
};
typedef struct consumer_struct ConsumerStruct;

struct consumer_struct_array {
	int done;
	int numCons;
	int size;
	ConsumerStruct *consumerdata;
};
typedef struct consumer_struct_array* CSA;

int binarySearch2(CSA csa, char *key, int low, int high);

int consumercomp(const void *a, const void *b);

CSA CSACreate();

int CSADestroy(CSA csa);

int CSAInsert(CSA csa, char *category);

void PrintCSA(CSA csa);

#endif
