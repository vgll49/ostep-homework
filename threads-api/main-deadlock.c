#include <stdio.h>

#include "common_threads.h"

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void* worker(void* arg) {
    if ((long long) arg == 0) {
    // p1 may have locked this 
	Pthread_mutex_lock(&m1); 
    // p1 may wait for this to be unlocked if p2 already locked this -> DEADLOCK
	Pthread_mutex_lock(&m2); 
    } else {
    // p2 may have locked this 
	Pthread_mutex_lock(&m2);
    // p2 may wait for this to be unlocked if p2 already locked this -> DEADLOCK
	Pthread_mutex_lock(&m1);
    }
    Pthread_mutex_unlock(&m1);
    Pthread_mutex_unlock(&m2);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    Pthread_create(&p1, NULL, worker, (void *) (long long) 0); // locks m1 then m2
    Pthread_create(&p2, NULL, worker, (void *) (long long) 1); // locks m2 then m1
    Pthread_join(p1, NULL); // waits for p1 -> so till m1 and m2 are unlocked again
    Pthread_join(p2, NULL); // waits for p2 -> so till m2 and m1 are unlocked again
    return 0;
}
