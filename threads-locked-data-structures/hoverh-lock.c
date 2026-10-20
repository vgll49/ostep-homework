#include <stdio.h>
#include <time.h> // clock_getttime
#include <pthread.h> // threads
#include <stdlib.h> // atoi for cli args

// basic node structure
typedef struct __node_t {
    int key;
    struct __node_t *next;
} node_t;

// basic list structure (one used per list)
typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock;
} list_t;

void List_Init(list_t *L) {
    pthread_mutex_init(&L->lock, NULL);
    L->head = NULL;
}

int List_Insert(list_t *L, int key) {
    // synchronization not needed
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL) {
    perror("malloc");
    return -1;
    }
    new->key = key;
    // just lock critical section
    pthread_mutex_lock(&L->lock);
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->lock);
    return 0; // success
}

int List_Lookup(list_t *L, int key) {
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t *curr = L->head;
    while (curr) {
    if (curr->key == key) {
    rv = 0;
    break;
    }
    curr = curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return rv; // now both success and failure
}


int main(int argc, char *argv[]) {
    printf("HelloWorld\n");
    printf("Usage: %s <thread_count>\n", argv[0]);
      if(argc != 2) { }
    return 0;}