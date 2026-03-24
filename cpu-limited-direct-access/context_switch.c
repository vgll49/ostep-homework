#define _GNU_SOURCE // set affinity and cpu 
#include <stdio.h>
#include <err.h> // err 
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // pipe, fork, read
#include <sched.h> // set affinity, cpu coqre
#include <sys/wait.h> // wait
#include <sys/time.h> // gettimeofdays
int main() {

    printf("Hello World\n");
    // ensure scontext switch on same processor: sched_setaffinity()
    int cpuNumber = 0;
    cpu_set_t set;
    int pipe1 [2];
    int pipe2 [2];
    struct timeval tvParent;
    struct timeval tvChild;
    double timeDiff = 0;
    int numOfIterations = 10000;

    CPU_ZERO(&set); // clears set, so that it contains no cpus
    CPU_SET(cpuNumber, &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1) {
        err(EXIT_FAILURE, "sched_setaffinity");
    }    
    

    if(pipe(pipe1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if(pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid_t  pid1 = fork();

    
    if (pid1 < 0) {

        fprintf(stderr, "fork failed\n");
    }

    if (pid1 == 0) {

        close(pipe1[0]); // close pipe1 read end
        close(pipe2[1]); // close pipe2 write end
        
        for(int i = 0; i < numOfIterations; i++) {
            // Process 1: write pipe 1, wait read pipe2
            gettimeofday(&tvChild, NULL);
            write(pipe1[1], &tvChild, sizeof(tvChild));
            read(pipe2[0], &tvParent ,sizeof(tvParent));
        }

        exit(0);
    } else {
            
        close(pipe1[1]); // close pipe1 write end
        close(pipe2[0]); // close pipe2 read end

        for(int i = 0; i < numOfIterations; i++) {
            read(pipe1[0], &tvChild, sizeof(tvChild));
            gettimeofday(&tvParent, NULL);
            write(pipe2[1], &tvParent, sizeof(tvParent));
            double parentTime = (tvParent.tv_sec * 1000000) + tvParent.tv_usec;
            double childTime = (tvChild.tv_sec * 1000000) + tvChild.tv_usec;
            timeDiff += parentTime - childTime;
        }
        wait(NULL);
        float averageContextSwitchCost = timeDiff/numOfIterations;    
        printf("average cost of a context switch: %f\n", averageContextSwitchCost);    

        exit(0);

    }

    return 0;
}