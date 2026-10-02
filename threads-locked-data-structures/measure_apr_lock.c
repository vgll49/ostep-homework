#include <stdio.h>
#include <time.h>     // clock_gettime
#include <pthread.h>
#include <stdlib.h>   // atoi

#define NUMCPUS 12    // lscpu
#define CACHE_LINE 64

// Jeder Slot (lokaler Zähler + Lock) belegt eine eigene Cache-Line -> kein False Sharing
typedef struct {
    int value;
    pthread_mutex_t lock;
} __attribute__((aligned(CACHE_LINE))) slot_t;

typedef struct counter_t {
    int global;
    pthread_mutex_t g_lock;
    int treshold;
    slot_t slot[NUMCPUS];   // beginnt automatisch auf einer neuen Cache-Line
} __attribute__((aligned(CACHE_LINE))) counter_t;

typedef struct arg_t {
    counter_t *c;
    int t_target;
    int threadID;
} arg_t;

void init_counter(counter_t *c, int treshold) {
    c->treshold = treshold;
    c->global = 0;
    pthread_mutex_init(&c->g_lock, NULL);
    for (int i = 0; i < NUMCPUS; i++) {
        c->slot[i].value = 0;
        pthread_mutex_init(&c->slot[i].lock, NULL);
    }
}

void update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % NUMCPUS;
    pthread_mutex_lock(&c->slot[cpu].lock);
    c->slot[cpu].value += amt;
    if (c->slot[cpu].value >= c->treshold) {
        pthread_mutex_lock(&c->g_lock);
        c->global += c->slot[cpu].value;
        pthread_mutex_unlock(&c->g_lock);
        c->slot[cpu].value = 0;
    }
    pthread_mutex_unlock(&c->slot[cpu].lock);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->g_lock);
    int val = c->global;
    pthread_mutex_unlock(&c->g_lock);
    return val;
}

int get_exact(counter_t *c) {
    int total = get(c);
    for (int i = 0; i < NUMCPUS; i++) {
        pthread_mutex_lock(&c->slot[i].lock);
        total += c->slot[i].value;
        pthread_mutex_unlock(&c->slot[i].lock);
    }
    return total;
}

void *mythread(void *arg) {
    arg_t *args = (arg_t *) arg;

    for (int i = 0; i < args->t_target; i++) {
        update(args->c, args->threadID, 1);
    }

    return NULL;
}

static double now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <number_of_threads> <counter_target>\n", argv[0]);
        return 1;
    }

    long num_threads = atoi(argv[1]);
    const int target = atoi(argv[2]);

    if (num_threads < 1 || num_threads > NUMCPUS) {
        printf("Can only use 1 to %d Threads. Exiting...\n", NUMCPUS);
        return 1;
    }

    printf("number of threads: %ld\n", num_threads);
    pthread_t threads[num_threads];

    const int t_target = target / num_threads;
    const int remainder = target % num_threads;   // geht an den letzten Thread

    printf("counter target: %d\n", target);
    printf("single thread target: %d\n", t_target);

    counter_t c;
    init_counter(&c, 10000);

    arg_t args[NUMCPUS];

    double time_pre = now_us();

    for (int i = 0; i < num_threads; i++) {
        args[i] = (arg_t){
            .c = &c,
            .t_target = t_target + (i == num_threads - 1 ? remainder : 0),
            .threadID = i,
        };

        pthread_create(&threads[i], NULL, mythread, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double time_post = now_us();
    double diff = time_post - time_pre;

    printf("shared counter (approx, global): %d\n", get(&c));
    printf("shared counter (exact):          %d\n", get_exact(&c));
    printf("Timedelta is %lf us\n", diff);

    return 0;
}