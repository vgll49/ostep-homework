#include <stdio.h>
#include <time.h>     // clock_gettime
#include <pthread.h>
#include <stdlib.h>   // atoi

#define NUMCPUS 12    // lscpu
#define CACHE_LINE 64
#define RUNS 20
#define WARMUP_RUNS 3
#define MAX_THREADS 12


typedef struct {
    int value;
    pthread_mutex_t lock;
} __attribute__((aligned(CACHE_LINE))) slot_t;

typedef struct counter_t {
    int global;
    pthread_mutex_t g_lock;
    int treshold;
    slot_t slot[NUMCPUS];   // starts on new cache line
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


double run_trial(int num_threads, int treshold, int target) {
    arg_t args[MAX_THREADS];
    pthread_t threads[num_threads];

    counter_t c;

    int t_target = target / num_threads;
    int remainder = target % num_threads;

    init_counter(&c, treshold);

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
    double elapsed = time_post - time_pre;

    int exact = get_exact(&c);

    if (exact != target) {
        printf("ERROR: expected %d, got %d\n", target, exact);
    }

    // Cleanup
    for (int i = 0; i < NUMCPUS; i++) {
        pthread_mutex_destroy(&c.slot[i].lock);
    }

    pthread_mutex_destroy(&c.g_lock);

    return elapsed;
}




/* ---------------------------------------------------------
   Median
   --------------------------------------------------------- */

int compare_double(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;

    return (x > y) - (x < y);
}


double median(double values[], int count) {

    qsort(values, count, sizeof(double), compare_double);

    if (count % 2 == 0) {
        return (values[count / 2 - 1] + values[count / 2]) / 2.0;
    }

    return values[count / 2];
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <number_of_threads> <counter_target>\n", argv[0]);
        return 1;
    }

    const int target = atoi(argv[1]);

    FILE *file = fopen("apr_lock_benchmark.csv", "w");

    
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    int thread_counts[] = {1, 2, 4, 8, 12};
    int thresholds[] = {100, 1000, 10000};

    // Only summary values (medians) are written to CSV
    fprintf(file, "threads,treshold,median_time_us,target\n");

    for (int i = 0; i < 5; i++) {
        int threads = thread_counts[i];

        for (int j = 0; j < 3; j++) {
            int threshold = thresholds[j];


            double times[RUNS];

            // warmups due to caching 
            for (int i = 0; i < WARMUP_RUNS; i++) {
                run_trial(threads, threshold, target);
            }

            // measured trials
            for (int i = 0; i < RUNS; i++) {
                times[i] = run_trial(threads, threshold, target);
            }

            double med = median(times, RUNS);

            printf("%d,%d,%f,%d\n", threads, threshold, med, target);
            fprintf(file, "%d,%d,%f,%d\n", threads, threshold, med, target);
        }
    }

    fclose(file);

    printf("\nMedian results written to lookup_benchmark.csv\n");


    return 0;
}