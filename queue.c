#include "queue.h"
#include <string.h>
#include <stdio.h>

Queue QCreate() {
	Queue q;
	q = (struct queue *)malloc(sizeof(struct queue));
	if (q == NULL) {
		printf("Error: Out of memory.\n");
		return NULL;
	}
	q->numElem = 0;
	q->front = NULL;
	q->back = NULL;
	q->max = 256;
	return q;
}

int QDestroy(Queue front) {
	if (front == NULL) {
		printf("Error: Queue is NULL.\n");
		return 0;
	}
	SOrder *curr;
	SOrder *temp;
	curr = front;
	while (curr != NULL) {
		temp = curr;
		curr = curr->next;
		free(temp);
	}
	return 1;
}

int push(Queue q, QNode *n) {
	if (q->front == NULL) {
		q->front = n;
		q->back = n;
		n->next = NULL;
		q->numElem++;
		return 1;
	}
	q->back->next = n;
	q->back = n;
	n->next = NULL;
	q->numElem++;
	return 1;
}

QNode pop(Queue q) {
	QNode r;
	if (numElem == 0) {
		return NULL;
	}
	r = q->front;
	q->front = q->front->next;
	q->numElem--;
	return r;
}
