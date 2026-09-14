    #include <stdio.h>
    #include <sys/time.h> // gettimebyday
    #include <pthread.h>
    #include <stdlib.h> // atoi

    typedef struct counter_t {
        int value;
        pthread_mutex_t lock;
    } counter_t;

    typedef struct arg_t {
        counter_t *c;
        const int t_target;
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

    int main(int argc, char* argv[])
    {
        const int target = 100000;
        
        
        if (argc != 2) {
            printf("Usage: %s <number_of_threads>\n", argv[0]);
            return 1;
        }
        
        long num_threads = atoi(argv[1]);
        
        pthread_t threads [num_threads];
        
        const int t_target = target/num_threads;

        struct timeval tv;

        counter_t c;

        init_counter(&c);

        arg_t args = {
            .c = &c,
            .t_target = t_target,
        };

        if (gettimeofday(&tv, NULL) != 0) {
            printf("error getting timeofday\n");
            return 1;
        }


        for (int i = 0; i < num_threads; i++) {
            pthread_create(&threads[i], NULL, mythread, &args);
        }

        printf("tv_sec: %ld, tv_usec: %ld\n",
            (long)tv.tv_sec,
            (long)tv.tv_usec);


        double time_pre = (tv.tv_sec * 1000000) + tv.tv_usec;


            if (gettimeofday(&tv, NULL) != 0) {
            printf("error getting timeofday\n");
            return 1;
        }
        
        double time_post = (tv.tv_sec * 1000000) + tv.tv_usec;
        
        double diff = time_post - time_pre;

        printf("Timedelta is %lf \n", diff);
        
        return 0;


        


    }