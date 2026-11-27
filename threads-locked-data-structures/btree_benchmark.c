#include <stdio.h>
#include <stdlib.h>
#include <time.h>     // clock_gettime
#include <pthread.h> 
#include <stdlib.h>   // atoi

#define NUMCPUS 12    // lscpu
#define RUNS 20
#define WARMUP_RUNS 3
#define MAX_THREADS 12

/*
TODOS:
benchmark

*/

typedef struct node
{ 
    int key; 
    struct node *left; 
    struct node *right;
} Node;

typedef struct tree {
    Node *root;
    pthread_mutex_t lock;
} tree_t;

typedef struct node_lock {
    int key;
    struct node_lock *left;
    struct node_lock *right;
    pthread_mutex_t lock;
} NodeLock;

typedef struct tree_lock {
    NodeLock *root;
    pthread_mutex_t root_lock;
} tree_lock_t;

void shuffle(int *values, int count) {
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = values[i];
        values[i] = values[j];
        values[j] = temp;
    }
}

Node* createNode(int value) {
    Node *temp = malloc(sizeof(*temp));
    if (temp == NULL) {
        printf("error allocating node memory \n");
        return NULL;
    }
    temp->key = value;
    temp->left = temp->right = NULL;
    return temp;
}

int initTree(tree_t* tree) {
    tree->root = NULL;

    if (pthread_mutex_init(&tree->lock, NULL) != 0) {
        return -1;
    }

    return 0;
}

Node *insertNode(Node *node, int value) {
    if (node == NULL)
        return createNode(value);

    if (value < node->key)
        node->left = insertNode(node->left, value);
    else if (value > node->key)
        node->right = insertNode(node->right, value);

    return node;
}

/* ---------------------------------------------------------
   Approach 1: Global locking
   --------------------------------------------------------- */

int initGLockTree(tree_lock_t* tree) {
    tree->root = NULL;

    if (pthread_mutex_init(&tree->root_lock, NULL) != 0) {
        return -1;
    }

    return 0;
} 

void insertGlobal(void *arg, int value) {
    tree_t *tree = arg;

    pthread_mutex_lock(&tree->lock);

    tree->root = insertNode(tree->root, value);

    pthread_mutex_unlock(&tree->lock);
}

/* ---------------------------------------------------------
   Approach 2: Node based locking
   --------------------------------------------------------- */

NodeLock *createNodeLock(int value) {
    NodeLock *temp = malloc(sizeof(*temp));
    if (temp == NULL) {
        printf("error allocating node memory \n");
        return NULL;
    }
    temp->key = value;
    temp->left = temp->right = NULL;

    if (pthread_mutex_init(&temp->lock, NULL) != 0) {
        printf("error initializing nodelock \n");
        free(temp);
        return NULL;    
    }
    
    return temp;
}


void insertNodeLocked(NodeLock *node, int value) {

    if (value < node->key) {
        if (node->left == NULL) {
            node->left = createNodeLock(value);
            pthread_mutex_unlock(&node->lock);
        } else {
            NodeLock *child = node->left;
            pthread_mutex_lock(&child->lock);
            pthread_mutex_unlock(&node->lock);
            insertNodeLocked(child, value);
        }
    }
    else if (value > node->key) {
        if (node->right == NULL) {
            node->right = createNodeLock(value);
            pthread_mutex_unlock(&node->lock);
        } else {
            NodeLock *child = node->right;
            pthread_mutex_lock(&child->lock);
            pthread_mutex_unlock(&node->lock);
            insertNodeLocked(child, value);
        }
    }
    else {
        // duplicate, just return
        pthread_mutex_unlock(&node->lock);
        return;
    }
}

void insertLocal(void *arg, int value) {
    tree_lock_t *tree = arg;

    pthread_mutex_lock(&tree->root_lock);

    if (tree->root == NULL) {
        tree->root = createNodeLock(value);
        pthread_mutex_unlock(&tree->root_lock);
        return;
    }

    NodeLock *root = tree->root;
    pthread_mutex_lock(&root->lock);
    
    pthread_mutex_unlock(&tree->root_lock);

    insertNodeLocked(tree->root, value);

}

void freeTree(Node *node) {
    if (node == NULL)
        return;

    freeTree(node->left);
    freeTree(node->right);

    free(node);
}

void destroyTree(tree_t *tree) {
    freeTree(tree->root);

    tree->root = NULL;
    pthread_mutex_destroy(&tree->lock);
}

void freeTreeLock(NodeLock *node) {
    if (node == NULL)
        return;

    freeTreeLock(node->left);
    freeTreeLock(node->right);

    pthread_mutex_destroy(&node->lock);
    free(node);
}

void destroyTreeLock(tree_lock_t *tree) {
    freeTreeLock(tree->root);

    tree->root = NULL;
    pthread_mutex_destroy(&tree->root_lock);
}

void printGlobal(Node* root) {
    if(root != NULL)
    {
        printf("node val: %d\n", root->key);
        printGlobal(root->left);
        printGlobal(root->right);

    }
}

void printLocal(NodeLock* root) {
    if(root != NULL)
    { 
        printf("node val: %d\n", root->key);
        printLocal(root->left);
        printLocal(root->right);

    }
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

typedef void (*insert_fn)(void *, int);

typedef struct __args_t {
    void *tree;
    int *values;
    int inserts;
    int num_threads;
    int threadID;
    insert_fn insertFunc;
} args_t;

void *worker(void *arg) {
    args_t *args = (args_t *)arg;

    for (int i = args->threadID; i < args->inserts; i += args->num_threads) {
        args->insertFunc(args->tree, args->values[i]);
    }

    return NULL;
}

static double now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

double run_trial(void *tree, int num_threads, insert_fn insert_func,
                 int *values, int inserts) {

    pthread_t threads[MAX_THREADS];
    args_t args[MAX_THREADS];

    double time_pre = now_us();

    for (int i = 0; i < num_threads; i++) {
        args[i] = (args_t) {
            .tree = tree,
            .values = values,
            .inserts = inserts,
            .num_threads = num_threads,
            .threadID = i,
            .insertFunc = insert_func
        };

        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double time_post = now_us();

    return time_post - time_pre;
}

double runGlobalFresh(int num_threads, int *values, int inserts) {
    tree_t tree;

    if (initTree(&tree) != 0) {
        fprintf(stderr, "Could not initialize global tree\n");
        exit(EXIT_FAILURE);
    }

    double elapsed = run_trial(&tree, num_threads, insertGlobal, values, inserts);

    destroyTree(&tree);

    return elapsed;
}


double runLocalFresh(int num_threads, int *values, int inserts) {
    tree_lock_t tree;

    if (initGLockTree(&tree) != 0) {
        fprintf(stderr, "Could not initialize local tree\n");
        exit(EXIT_FAILURE);
    }

    double elapsed = run_trial(&tree, num_threads, insertLocal, values, inserts);

    destroyTreeLock(&tree);

    return elapsed;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <number_of_insertons> \n", argv[0]);
        return 1;
    }

    tree_t tree;
    tree_lock_t tree_lock;

    if (initTree(&tree) != 0) {
        fprintf(stderr, "Error initializing tree\n");
        return 1;
    }

    if(initGLockTree(&tree_lock) != 0) {
        fprintf(stderr, "Error initializing tree\n");
        return 1;
    }


    int thread_counts[] = {1, 2, 4, 8, 12};
    const int inserts = atoi(argv[1]);

    if (inserts <= 0) {
        fprintf(stderr, "Number of inserts must be positive\n");
        return EXIT_FAILURE;
    }

    int *values = malloc((size_t)inserts * sizeof(*values));

    if (values == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < inserts; i++) {
        values[i] = i;
    }

    srand(42);
    shuffle(values, inserts);
    char *names[] = {
        "global",
        "local"
    };

    FILE *file = fopen("btree_benchmark.csv", "w");

    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fprintf(file, "lock_type,threads,median_time_us,target\n");

    for (int t = 0; t < 5; t++) {

        int num_threads = thread_counts[t];

        double global_samples[RUNS];
        double local_samples[RUNS];

        printf("\nThreads: %d\n", num_threads);

        // Warm-up runs
        for (int w = 0; w < WARMUP_RUNS; w++) {
            runGlobalFresh(num_threads, values, inserts);
            runLocalFresh(num_threads, values, inserts);
        }
        // Benchmark runs
        for (int r = 0; r < RUNS; r++) {
            if (r % 2 == 0) {

                global_samples[r] = runGlobalFresh(num_threads, values, inserts);
                local_samples[r] = runLocalFresh(num_threads, values, inserts);

            } else {

                local_samples[r] = runLocalFresh(num_threads, values, inserts);
                global_samples[r] = runGlobalFresh(num_threads, values, inserts);

            }
        }

        // Calculate medians
        double global_median = median(global_samples, RUNS);
        double local_median = median(local_samples, RUNS);

        // Print summary
        printf("Global locking: %.2f us\n", global_median);
        printf("Local node lock: %.2f us\n", local_median);

        // Write summary to CSV
        fprintf(file, "%s,%d,%.2f,%d\n",
                names[0], num_threads, global_median, inserts);

        fprintf(file, "%s,%d,%.2f,%d\n",
                names[1], num_threads, local_median, inserts);

        fflush(file);
    }

    fclose(file);


    printGlobal(tree.root);

    printLocal(tree_lock.root);

    
    return 0;
}
