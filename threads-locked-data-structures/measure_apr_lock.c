    #include <stdio.h>
    #include <sys/time.h> // gettimebyday
    #include <pthread.h>
    #include <stdlib.h> // atoi

    #define NUMCPUS 12 //lscpu

    typedef struct counter_t {
        int global;
        pthread_mutex_t g_lock;
        int local[NUMCPUS];
        pthread_mutex_t l_lock[NUMCPUS];
        int treshold;

    } counter_t;

    typedef struct arg_t {
        counter_t *c;
        int t_target;
        int threadID;
    } arg_t;

    void init_counter(counter_t *c, int treshold) {
        c->treshold = treshold;
        c->global = 0;
        pthread_mutex_init(&c->g_lock, NULL);
        int i;
        for (i = 0; i < NUMCPUS; i++) {
            c->local[i] = 0;
            pthread_mutex_init(&c->l_lock[i],  NULL);
        }
    }

    void update(counter_t *c, int threadID, int amt) {
        int cpu = threadID % NUMCPUS;
        pthread_mutex_lock(&c->l_lock[cpu]);
        c->local[cpu] +=  amt;
        if(c->local[cpu] >= c->treshold) {
            pthread_mutex_lock(&c->g_lock);
            c->global += c->local[cpu];
            pthread_mutex_unlock(&c->g_lock);
            c->local[cpu] = 0;
        }
        pthread_mutex_unlock(&c->l_lock[cpu]);
    }

    int get (counter_t *c) {
        pthread_mutex_lock(&c->g_lock);
        int val = c->global;
        pthread_mutex_unlock(&c->g_lock);
        return val;
    }

    void *mythread(void *arg) {
        arg_t *args = (arg_t *) arg;

        for(int i = 0; i < args->t_target; i++) {
            update(args->c, args->threadID, 1);
        }

        return NULL;
    }

    int main(int argc, char* argv[])
    {
        
        if (argc != 3) {
            printf("Usage: %s <number_of_threads> <counter_target>\n", argv[0]);
            return 1;
        }
        
        long num_threads = atoi(argv[1]);

        const int target =  atoi(argv[2]);

        if(num_threads  > 12) {
            printf("Can only use up to 12 Threads. Exiting...\n");
            return 1;
        }
        
        printf("number of threads: %ld\n", num_threads);
        pthread_t threads[num_threads];
        
        const int t_target = target/num_threads;

        printf("counter target: %d\n", target); 
        printf("single thread target: %d\n", t_target);
        struct timeval tv;

        counter_t c;

        init_counter(&c, 10000);

        arg_t args[NUMCPUS];

        if (gettimeofday(&tv, NULL) != 0) {
            printf("error getting timeofday\n");
            return 1;
        }

        double time_pre = (tv.tv_sec * 1000000) + tv.tv_usec;


        for (int i = 0; i < num_threads; i++) {

            args[i] = (arg_t){
                .c = &c,
                .t_target = t_target,
                .threadID = i,
            };

            pthread_create(&threads[i], NULL, mythread, &args[i]);
        }
        
        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        if (gettimeofday(&tv, NULL) != 0) {
            printf("error getting timeofday\n");
            return 1;
        }
        
        double time_post = (tv.tv_sec * 1000000) + tv.tv_usec;
        
        double diff = time_post - time_pre;

        printf("shared counter after loop is: %d\n", c.global);
        printf("Timedelta is %lf \n", diff);
        
        return 0;

    }