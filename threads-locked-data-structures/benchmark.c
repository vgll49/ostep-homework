
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define LIST_SIZE 100
#define OPS_PER_THREAD 2000
#define RUNS 20
#define WARMUP_RUNS 3
#define MAX_THREADS 12

/* ---------------------------------------------------------
   Linked list
   --------------------------------------------------------- */

typedef struct __node_t {
    int key;
    pthread_mutex_t lock;
    struct __node_t *next;
} node_t;

typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock;
} list_t;


void List_Init(list_t *L) {
    L->head = NULL;

    int rc = pthread_mutex_init(&L->lock, NULL);

    if (rc != 0) {
        fprintf(stderr, "pthread_mutex_init: %s\n", strerror(rc));
        exit(EXIT_FAILURE);
    }
}


int List_Insert(list_t *L, int key) {
    node_t *new_node = malloc(sizeof(node_t));

    if (new_node == NULL) {
        perror("malloc");
        return -1;
    }

    new_node->key = key;
    new_node->next = NULL;

    int rc = pthread_mutex_init(&new_node->lock, NULL);

    if (rc != 0) {
        fprintf(stderr, "pthread_mutex_init: %s\n", strerror(rc));
        free(new_node);
        return -1;
    }

    pthread_mutex_lock(&L->lock);

    new_node->next = L->head;
    L->head = new_node;

    pthread_mutex_unlock(&L->lock);

    return 0;
}


/* ---------------------------------------------------------
   Lookup 1: Global locking
   --------------------------------------------------------- */

int List_Lookup_Global(list_t *L, int key) {
    int rv = -1;

    pthread_mutex_lock(&L->lock);

    node_t *curr = L->head;

    while (curr != NULL) {
        if (curr->key == key) {
            rv = 0;
            break;
        }

        curr = curr->next;
    }

    pthread_mutex_unlock(&L->lock);

    return rv;
}


/* ---------------------------------------------------------
   Lookup 2: Hand-over-hand locking
   --------------------------------------------------------- */

int List_Lookup_HOH(list_t *L, int key) {
    int rv = -1;

    pthread_mutex_lock(&L->lock);

    node_t *curr = L->head;

    if (curr != NULL) {
        pthread_mutex_lock(&curr->lock);
    }

    pthread_mutex_unlock(&L->lock);

    while (curr != NULL) {
        if (curr->key == key) {
            rv = 0;
            pthread_mutex_unlock(&curr->lock);
            break;
        }

        node_t *next = curr->next;

        if (next != NULL) {
            pthread_mutex_lock(&next->lock);
        }

        pthread_mutex_unlock(&curr->lock);

        curr = next;
    }

    return rv;
}


/* ---------------------------------------------------------
   Timing
   --------------------------------------------------------- */

double now_us(void) {
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}


/* ---------------------------------------------------------
   Worker setup
   --------------------------------------------------------- */

typedef int (*lookup_fn)(list_t *, int);

typedef struct __args_t {
    list_t *list;
    int ops;
    int threadID;
    lookup_fn lookup;
    int hits;
} args_t;


void *worker(void *arg) {
    args_t *args = (args_t *)arg;

    int hit_index = 0;

    for (int i = 0; i < args->ops; i++) {

        int key;

        // 20% misses, 80% hits
        if (i % 5 == 0) {
            key = -1;
        } else {
            key = (hit_index + args->threadID * 17) % LIST_SIZE;
            hit_index++;
        }

        if (args->lookup(args->list, key) == 0) {
            args->hits++;
        }
    }

    return NULL;
}


/* ---------------------------------------------------------
   Run one benchmark trial
   --------------------------------------------------------- */

double run_trial(list_t *list, int num_threads, lookup_fn lookup) {

    pthread_t threads[MAX_THREADS];
    args_t args[MAX_THREADS];

    for (int i = 0; i < num_threads; i++) {
        args[i] = (args_t) {
            .list = list,
            .ops = OPS_PER_THREAD,
            .threadID = i,
            .lookup = lookup,
            .hits = 0
        };
    }

    double start = now_us();

    for (int i = 0; i < num_threads; i++) {
        int rc = pthread_create(
            &threads[i], NULL, worker, &args[i]
        );

        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        int rc = pthread_join(threads[i], NULL);

        if (rc != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(rc));
            exit(EXIT_FAILURE);
        }
    }

    double elapsed = now_us() - start;

    // Verify lookup results
    int expected_hits = OPS_PER_THREAD -
                        (OPS_PER_THREAD + 4) / 5;

    for (int i = 0; i < num_threads; i++) {
        if (args[i].hits != expected_hits) {
            fprintf(stderr, "Unexpected lookup result in thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

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


int main(void) {

    list_t list;
    List_Init(&list);

    // Build list before benchmarking
    for (int i = 0; i < LIST_SIZE; i++) {
        if (List_Insert(&list, i) != 0) {
            return EXIT_FAILURE;
        }
    }

    int thread_counts[] = {1, 2, 4, 8, 12};

    lookup_fn functions[] = {
        List_Lookup_Global,
        List_Lookup_HOH
    };

    char *names[] = {
        "global",
        "hand_over_hand"
    };

    FILE *file = fopen("lookup_benchmark.csv", "w");

    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    // Only summary values (medians) are written to CSV
    fprintf(file, "lock_type,threads,median_time_us\n");

    for (int t = 0; t < 5; t++) {

        int num_threads = thread_counts[t];

        double global_samples[RUNS];
        double hoh_samples[RUNS];

        printf("\nThreads: %d\n", num_threads);

        // Warm-up runs
        for (int w = 0; w < WARMUP_RUNS; w++) {
            run_trial(&list, num_threads, functions[w % 2]);
            run_trial(&list, num_threads, functions[(w + 1) % 2]);
        }

        // Actual measurements
        for (int r = 0; r < RUNS; r++) {

            // Alternate execution order
            if (r % 2 == 0) {
                global_samples[r] =
                    run_trial(&list, num_threads, List_Lookup_Global);

                hoh_samples[r] =
                    run_trial(&list, num_threads, List_Lookup_HOH);
            } else {
                hoh_samples[r] =
                    run_trial(&list, num_threads, List_Lookup_HOH);

                global_samples[r] =
                    run_trial(&list, num_threads, List_Lookup_Global);
            }
        }

        // Calculate medians
        double global_median = median(global_samples, RUNS);
        double hoh_median = median(hoh_samples, RUNS);

        // Print summary
        printf("Global locking:    %.2f us\n", global_median);
        printf("Hand-over-hand:    %.2f us\n", hoh_median);

        // Write summary to CSV
        fprintf(file, "%s,%d,%.2f\n",
                names[0], num_threads, global_median);

        fprintf(file, "%s,%d,%.2f\n",
                names[1], num_threads, hoh_median);

        fflush(file);
    }

    fclose(file);

    // Cleanup
    node_t *curr = list.head;

    while (curr != NULL) {
        node_t *next = curr->next;

        pthread_mutex_destroy(&curr->lock);
        free(curr);

        curr = next;
    }

    pthread_mutex_destroy(&list.lock);

    printf("\nMedian results written to lookup_benchmark.csv\n");

    return 0;
}

// Answer to 