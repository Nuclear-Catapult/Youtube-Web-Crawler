#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include "queue.h"

struct Q_Node {
    struct Q_Node* next;
    int64_t data;
};

struct Q_Node* front = NULL;
struct Q_Node* back = NULL;

uint64_t Q_Count = 0;

pthread_mutex_t key;

// Don't enqueue in the value 0, since dequeue() returns 0 when the queue is empty
void enqueue(int64_t data)
{
    if (data == 0)
        fprintf(stderr, "Error: Queue prohibits zero value to be enqueueed.\n"), exit(1);
    pthread_mutex_lock(&key);
    if (front == NULL) {
        back = calloc(sizeof(struct Q_Node), 1);
        front = back;
    } else {
        back->next = calloc(sizeof(struct Q_Node), 1);
        back = back->next;
    }
    back->data = data;

    Q_Count++;

    pthread_mutex_unlock(&key);
}

// returns 0 if queue is empty
// else, returns dequeued value
int64_t dequeue()
{
    pthread_mutex_lock(&key);
    if (front == NULL) {
        pthread_mutex_unlock(&key);
        return 0;
    }
    int64_t to_return = front->data;
    struct Q_Node* to_delete = front;
    if (front == back) {
        front = NULL;
        back = NULL;
    } else {
        front = front->next;
    }
    free(to_delete);

    Q_Count--;

    pthread_mutex_unlock(&key);
    return to_return;
}
