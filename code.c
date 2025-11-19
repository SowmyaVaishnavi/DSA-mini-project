#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CUSTOMERS 10

// ---------- LINEAR QUEUE (ARRAY BASED) ----------
typedef struct {
    int items[MAX_CUSTOMERS];
    int front, rear;
} LinearQueue;

void initQueue(LinearQueue* q) {
    q->front = q->rear = -1;
}

int isEmpty(LinearQueue* q) {
    return q->front == -1;
}

int isFull(LinearQueue* q) {
    return q->rear == MAX_CUSTOMERS - 1;
}

void enqueue(LinearQueue* q, int data) {
    if (isFull(q)) return;
    if (isEmpty(q)) q->front = 0;
    q->items[++q->rear] = data;
}

int dequeue(LinearQueue* q) {
    if (isEmpty(q)) return -1;
    int data = q->items[q->front];
    if (q->front == q->rear) q->front = q->rear = -1;
    else q->front++;
    return data;
}

// ---------- NON-LINEAR QUEUE (MIN-HEAP BASED PRIORITY QUEUE) ----------
typedef struct {
    int time;  // smaller time = higher priority
} Node;

typedef struct {
    Node heap[MAX_CUSTOMERS];
    int size;
} PriorityQueue;

void initPQ(PriorityQueue* pq) {
    pq->size = 0;
}

void swap(Node* a, Node* b) {
    Node temp = *a; *a = *b; *b = temp;
}

void insertPQ(PriorityQueue* pq, int time) {
    pq->heap[++pq->size].time = time;
    int i = pq->size;
    while (i > 1 && pq->heap[i].time < pq->heap[i / 2].time) {
        swap(&pq->heap[i], &pq->heap[i / 2]);
        i /= 2;
    }
}

int removePQ(PriorityQueue* pq) {
    if (pq->size == 0) return -1;
    int min = pq->heap[1].time;
    pq->heap[1] = pq->heap[pq->size--];
    int i = 1;
    while (i * 2 <= pq->size) {
        int smallest = i * 2;
        if (smallest + 1 <= pq->size && pq->heap[smallest + 1].time < pq->heap[smallest].time)
            smallest++;
        if (pq->heap[i].time < pq->heap[smallest].time) break;
        swap(&pq->heap[i], &pq->heap[smallest]);
        i = smallest;
    }
    return min;
}

// ---------- SIMULATION ----------
void simulateLinear() {
    LinearQueue q;
    initQueue(&q);
    printf("\n--- LINEAR QUEUE (SINGLE LINE) ---\n");

    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        int serviceTime = rand() % 10 + 1;
        enqueue(&q, serviceTime);
        printf("Customer %d arrived (service time = %d)\n", i + 1, serviceTime);
    }

    int totalTime = 0;
    while (!isEmpty(&q)) {
        int service = dequeue(&q);
        totalTime += service;
        printf("Customer served in %d units, total time = %d\n", service, totalTime);
    }
    printf("Total time (linear): %d units\n", totalTime);
}

void simulateNonLinear() {
    PriorityQueue pq;
    initPQ(&pq);
    printf("\n--- NON-LINEAR QUEUE (PRIORITY / MIN-HEAP) ---\n");

    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        int serviceTime = rand() % 10 + 1;
        insertPQ(&pq, serviceTime);
        printf("Customer %d arrived (service time = %d)\n", i + 1, serviceTime);
    }

    int totalTime = 0;
    while (pq.size > 0) {
        int service = removePQ(&pq);
        totalTime += service;
        printf("Customer (fastest first) served in %d units, total time = %d\n", service, totalTime);
    }
    printf("Total time (non-linear): %d units\n", totalTime);
}

// ---------- MAIN ----------
int main() {
    srand(time(0));
    simulateLinear();
    simulateNonLinear();
    return 0;
}
