#ifndef QUEUE_H
#define QUEUE_H
#include "LPC17xx.h"
#include "lpc_types.h"

/*
  Define MAX_QUEUE_SIZE before including this header to have a different queue
  size in the current **translation unit**. A translation unit roughly maps to
  a file, with all the pre-processing performed
*/
#ifndef MAX_QUEUE_SIZE
#define MAX_QUEUE_SIZE (8)
#endif /* MAX_QUEUE_SIZE */

/*
  A type agnostic pointer queue.
*/
struct Queue {
    uint8_t head;
    uint8_t tail;
    void * queue[MAX_QUEUE_SIZE];
};

/*
  Push a new pointer onto the queue.
*/
static inline Status push(struct Queue *q, void *e) {
    __disable_irq();
    if ((q->tail == MAX_QUEUE_SIZE-1 && q->head == 0) || (q->tail == q->head - 1)) {
        return ERROR;
    } else {
        q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
        q->queue[q->tail] = e;
        return SUCCESS;
    }
    __enable_irq();
}


/*
  Pop a pointer off the queue
*/
static inline void * pop(struct Queue *q) {
    if (q->head == q->tail) {
        return NULL;
    } else {
        void * e = q->queue[q->head];
        q->head = (q->head + 1) % MAX_QUEUE_SIZE;
        return e;
    }
}

#endif /* QUEUE_H */
