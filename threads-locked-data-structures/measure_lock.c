#include <stdio.h>
#include <time.h> // clock_gettime
#include <pthread.h>
#include <stdlib.h> // atoi


#define NUMCPUS 12    // lscpu
#define CACHE_LINE 64
#define RUNS 20
#define WARMUP_RUNS 3
#define MAX_THREADS 12


typedef struct counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

typedef struct arg_t {
    counter_t *c;
    int t_target;
    int threadID;
} arg_t;

void init_counter(counter_t *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value += 1;
    pthread_mutex_unlock(&c->lock);
}

int get (counter_t *c) {
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}

void *mythread(void *arg) {
    arg_t *args = (arg_t *) arg;

    for(int i = 0; i < args->t_target; i++) {
        increment(args->c);
    }

    return NULL;
}

static double now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}


double run_trial(int num_threads, int target) {
    arg_t args[MAX_THREADS];
    pthread_t threads[num_threads];
    

    counter_t c;

    int t_target = target / num_threads;
    int remainder = target % num_threads;

    init_counter(&c);

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

    int val = get(&c);

    
    if (val != target) {
        printf("ERROR: expected %d, got %d\n", target, val);
    }

    return time_post-time_pre;
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


int main(int argc, char* argv[])
{

    
    if (argc != 2) {
        printf("Usage: %s <target>\n", argv[0]);
        return 1;
    }
    
    const int target = atoi(argv[1]);
    
    printf("starting trials with target: %d\n", target);
    
    FILE *file = fopen("lock_benchmark.csv", "w");

    
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    
    int thread_counts[] = {1, 2, 4, 8, 12};

    // Only summary values (medians) are written to CSV
    fprintf(file, "threads,median_time_us,target\n");
    
    for (int i = 0; i < 5; i++) { // bc thread counts.size?
        int threads = thread_counts[i];

        double times[RUNS];

        // warmups due to caching 
        for (int i = 0; i < WARMUP_RUNS; i++) {
            run_trial(threads, target);
        }

        // measured trials
        for (int i = 0; i < RUNS; i++) {
            times[i] = run_trial(threads, target);
        }

        double med = median(times, RUNS);

        printf("%d,%f,%d\n", threads, med, target);
        fprintf(file,"%d,%f,%d\n", threads, med, target);

    }

    return 0;

}