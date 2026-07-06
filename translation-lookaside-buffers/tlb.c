#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h> // getpagesize
#include <errno.h> // errno
#include <stdlib.h> // strtol()
#include <limits.h> // INT_MIN/MAX
#include <string.h> // strlen
#include <sys/time.h> // gettimeofday
#include <sched.h> // set affinity

static const double MICROSEC_PER_SEC = 1000000.0;

int strtoi(const char *arg, int *out);
double calc_avg_page_access(int page_size, int trials, int num_pages);

int main(int argc, char **argv) {
    int NUMPAGES;
    int TRIALS;
    int PAGESIZE = getpagesize();
    int cpu_number = 0;
    cpu_set_t set;

    if(argc < 3)  {
        printf("pass NUMPAGES and TRIALS as commandline arguments.\n");
        return -1;
    }

    if(strtoi(argv[1], &NUMPAGES) != 0) {
        return -1;
    }
    
    if(NUMPAGES < 1) {
        printf("NUMPAGES has to be bigger than 1.\n");
        return -1;
    }
    
    if(strtoi(argv[2], &TRIALS) != 0) {
        return -1;
    }
    
    if(TRIALS < 1) {
        printf("TRIALS has to be bigger than 1.\n");
        return -1;
    }
    
    CPU_ZERO(&set); // clears set, so that it contains no cpus
    CPU_SET(cpu_number, &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1) {
        fprintf(stderr, "error setting cpu affinity: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    double avg_time_per_access = calc_avg_page_access(PAGESIZE, TRIALS, NUMPAGES);

    if (avg_time_per_access < 0) {
        fprintf(stderr, "measurement failed\n");
        return EXIT_FAILURE;
    }

    printf("%f\n", avg_time_per_access);
    
    return 0;

}

int strtoi(const char *arg, int *out) {
    char *p;
    errno = 0;

    long val = strtol(arg, &p, 10);

    if (*p != '\0' || errno != 0)
        return 1;

    if (val < INT_MIN || val > INT_MAX)
        return 2;

    *out = (int)val;
    return 0;
}

double calc_avg_page_access(int page_size, int trials, int num_pages) {
    
    struct timeval tv_pre, tv_post;
    int elems = (num_pages * page_size) / sizeof(int);
    int *a = calloc(elems, sizeof(int));
    volatile int *va = a; // to reduce compiler optimization 
    
    if (a == NULL) {
        fprintf(stderr, "error allocating memory: %s\n", strerror(errno));
        return -1.0; 
    }

    int jump = page_size / sizeof(int);  

    for (int i = 0; i < num_pages * jump; i += jump) {
        a[i] = 0;  // touch each page without measuring
    }


    gettimeofday(&tv_pre, NULL); 
    double pre = tv_pre.tv_sec * MICROSEC_PER_SEC + tv_pre.tv_usec; // TIME 1 var
    for(int j = 0; j<trials; j++) {
        for (int i = 0; i < num_pages * jump; i += jump) { // jump to new page every iteration
            va[i] += 1; // update int in new page
        }
    }
    gettimeofday(&tv_post, NULL); // TIME 1 var
    double post = tv_post.tv_sec * MICROSEC_PER_SEC + tv_post.tv_usec;
    double diff = post - pre;
    free(a);
    return diff/ (double) (num_pages * trials);
}