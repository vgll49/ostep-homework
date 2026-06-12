//  Inputs to the program should be: the number of pages to touch and the number of trials.
// What is the size of the pages? the number?


/*

bash script starts this loop in another loop, each iter is nubmber of pages accessed + time

if pages have to get accessed multiple times, increase index per individual page access
to use different VPNs and not load an adress into the tlb. 

int jump = PAGESIZE / sizeof(int); // calc size of jump based on page size
for (i = 0; i < NUMPAGES * jump; i += jump) // jump to new page every iteration
a[i] += 1; // update int in new page

*/


#include <stdio.h>
#include <unistd.h> // getpagesize
#include <errno.h> // errno
#include <stdlib.h> // strtol()
#include <limits.h> // INT_MIN/MAX
#include <string.h> // strlen
#include <sys/time.h> // gettimeofday


static const double MICROSEC_PER_SEC = 1000000.0;

int strtoi(const char *arg, int *out);

int main(int argc, char **argv) {

    if(argc < 3)  {
        printf("pass NUMPAGES and TRIALS as commandline arguments.\n");
        return -1;
    }
    
    struct timeval tv_pre, tv_post;

    int NUMPAGES;
    int TRIALS;
    int PAGESIZE = getpagesize();
    printf("PAGESIZE: %d\n", PAGESIZE);

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

    printf("NUMPAGES: %d\n", NUMPAGES);
    
    printf("TRIALS: %d\n", TRIALS);

    // init array big enough for the pages 
    int elems = (NUMPAGES * PAGESIZE) / sizeof(int);
    int * a = malloc(elems * sizeof(int));
    
    if (a == NULL) {
        printf("error allocating memory\n");
        return EXIT_FAILURE;
    }


    int jump = PAGESIZE / sizeof(int);  
    gettimeofday(&tv_pre, NULL); 
    double pre = tv_pre.tv_sec * MICROSEC_PER_SEC + tv_pre.tv_usec; // TIME 1 var
    for(int j = 0; j<TRIALS; j++) {
        for (int i = 0; i < NUMPAGES * jump; i += jump) {// jump to new page every iteration
            a[i] += 1; // update int in new page

            printf("access of index: %d at time: %ld\n", i, tv_pre.tv_sec+tv_pre.tv_usec);
        }
    }
    gettimeofday(&tv_post, NULL); // TIME 1 var
    double post = tv_post.tv_sec * MICROSEC_PER_SEC + tv_post.tv_usec;
    double diff = post - pre;
    printf("diff is: %f\n", diff);

   
    

    return 0;

    free(a);
}

// getconf PAGE_SIZE = 4096,
// manpage: getpagesize

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

/*
 1. For timing, you’ll need to use a timer (e.g., gettimeofday()).
 How precise is such a timer? How long does an operation have
 to take in order for you to time it precisely? (this will help determine how many times, in a loop, you’ll have to repeat a page access
in order to time it successfully) 

Microseconds

*/