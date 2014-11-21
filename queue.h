#ifndef QUEUE_H
#define QUEUE_H
/*
 * queue.h
 */

#include <stdlib.h>

struct qnode {
	char *bname;
	double price;
	long id;
	char *category;
	struct qnode *next;
};
typedef struct qnode QNode;

struct queue {
	int numElem;
	int max;
	struct qnode *front;
	struct qnode *back;
};
typedef struct queue* Queue;

Queue QCreate();

int QDestroy(Queue q);

int push(Queue q, QNode *n);

QNode *pop(Queue q);

#endif
