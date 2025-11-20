#include <stdio.h>
#include <stdlib.h>

#define MAX 50

typedef struct {
    int arr[MAX];
    int front, rear;
} LinearQ;

void initLQ(LinearQ *q) {
    q->front = q->rear = -1;
}

int isEmptyLQ(LinearQ *q) {
    return q->front == -1;
}

int queueLength(LinearQ *q) {
    if (q->front == -1) return 0;
    return q->rear - q->front + 1;
}

void enqueueLQ(LinearQ *q, int x) {
    q->arr[++q->rear] = x;
    if (q->front == -1) q->front = 0;
}

int dequeueLQ(LinearQ *q) {
    if (isEmptyLQ(q)) return -1;
    int val = q->arr[q->front];
    if (q->front == q->rear) q->front = q->rear = -1;
    else q->front++;
    return val;
}

typedef struct {
    int heap[MAX];
    int size;
} MinHeap;

void initPQ(MinHeap *pq) {
    pq->size = 0;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertPQ(MinHeap *pq, int x) {
    pq->heap[pq->size] = x;
    int i = pq->size;
    pq->size++;
    while (i > 0 && pq->heap[(i - 1) / 2] > pq->heap[i]) {
        swap(&pq->heap[(i - 1) / 2], &pq->heap[i]);
        i = (i - 1) / 2;
    }
}

int removePQ(MinHeap *pq) {
    if (pq->size == 0) return -1;
    int root = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    int i = 0;
    while (1) {
        int left = 2*i + 1;
        int right = 2*i + 2;
        int smallest = i;
        if (left < pq->size && pq->heap[left] < pq->heap[smallest])
            smallest = left;
        if (right < pq->size && pq->heap[right] < pq->heap[smallest])
            smallest = right;
        if (smallest == i) break;
        swap(&pq->heap[i], &pq->heap[smallest]);
        i = smallest;
    }
    return root;
}

int calculateServiceTime(int items) {
    if (items <= 10) return items * 2;
    return 25 + (items - 10) * 1;
}

void customerJockeys(LinearQ *fromQ, LinearQ *toQ, int serviceTime) {
    int fromLen = queueLength(fromQ);
    int toLen = queueLength(toQ);
    if (fromLen <= toLen || fromLen == 0) return;
    int pos = -1;
    for (int i = fromQ->front; i <= fromQ->rear; i++)
        if (fromQ->arr[i] == serviceTime) { pos = i; break; }
    if (pos == -1) return;
    int temp[MAX], t = 0;
    for (int i = fromQ->front; i <= fromQ->rear; i++)
        if (i != pos) temp[t++] = fromQ->arr[i];
    fromQ->front = fromQ->rear = -1;
    for (int i = 0; i < t; i++) enqueueLQ(fromQ, temp[i]);
    enqueueLQ(toQ, serviceTime);
    printf("Customer with service %d moved to express lane.\n", serviceTime);
}

void openExpressLane(int itemLimit, LinearQ *mainQ, LinearQ *expressQ) {
    int serviceLimit = calculateServiceTime(itemLimit);
    if (mainQ->front == -1) return;
    int temp[MAX], t = 0;
    for (int i = mainQ->front; i <= mainQ->rear; i++) {
        if (mainQ->arr[i] <= serviceLimit) {
            enqueueLQ(expressQ, mainQ->arr[i]);
            printf("Customer with service %d moved to express lane.\n", mainQ->arr[i]);
        } else {
            temp[t++] = mainQ->arr[i];
        }
    }
    mainQ->front = mainQ->rear = -1;
    for (int i = 0; i < t; i++) enqueueLQ(mainQ, temp[i]);
}

int main() {
    LinearQ mainQ, expressQ;
    MinHeap pq;
    initLQ(&mainQ);
    initLQ(&expressQ);
    initPQ(&pq);

    int n, items;
    int expressItemLimit = 5;

    printf("Enter number of customers: ");
    scanf("%d", &n);

    int allServices[MAX], count = 0;

    for (int i = 0; i < n; i++) {
        printf("Enter number of items for customer %d: ", i + 1);
        scanf("%d", &items);
        int service = calculateServiceTime(items);
        allServices[count++] = service;
        enqueueLQ(&mainQ, service);
        openExpressLane(expressItemLimit, &mainQ, &expressQ);
    }

    while (queueLength(&mainQ) - queueLength(&expressQ) > 1)
        customerJockeys(&mainQ, &expressQ, mainQ.arr[mainQ.front]);

    for (int i = 0; i < count; i++)
        insertPQ(&pq, allServices[i]);

    int waiting = 0, totalWaitMain = 0;
    while (!isEmptyLQ(&mainQ)) {
        int s = dequeueLQ(&mainQ);
        printf("Waiting: %d | Service: %d\n", waiting, s);
        totalWaitMain += waiting;
        waiting += s;
    }
    printf("TOTAL WAITING TIME (Main Queue): %d\n", totalWaitMain);

    waiting = 0;
    int totalWaitExpress = 0;
    while (!isEmptyLQ(&expressQ)) {
        int s = dequeueLQ(&expressQ);
        printf("Waiting: %d | Service: %d\n", waiting, s);
        totalWaitExpress += waiting;
        waiting += s;
    }
    printf("TOTAL WAITING TIME (Express Lane): %d\n", totalWaitExpress);

    waiting = 0;
    int totalWaitPQ = 0;
    while (pq.size > 0) {
        int s = removePQ(&pq);
        totalWaitPQ += waiting;
        waiting += s;
    }
    printf("TOTAL WAITING TIME (Non-Linear): %d\n", totalWaitPQ);

    printf("Main Queue: %d, Express Lane: %d, Priority Queue: %d\n",
           totalWaitMain, totalWaitExpress, totalWaitPQ);

    return 0;
}
