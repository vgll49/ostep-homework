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
    int buf;
    cpu_set_t set;
    int pipe1 [2];
    int pipe2 [2];
    struct timeval tv;
    long timeDiff = 0;
    int timProcess1 = 0;
    int timProcess2 = 0;
    int numOfIterations = 99;

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
            gettimeofday(&tv, NULL);
            timProcess2 = tv.tv_usec;
            // Process 1: write pipe 1, wait read pipe2

            write(pipe1[1], "s", strlen("s"));
            read(pipe2[0], &buf ,strlen("s"));
        }

        exit(0);
    } else {
            
        close(pipe1[1]); // close pipe1 write end
        close(pipe2[0]); // close pipe2 read end

        for(int i = 0; i < numOfIterations; i++) {
            gettimeofday(&tv, NULL);
            timProcess1 = tv.tv_usec;
            timeDiff += timProcess1 - timProcess2;
            read(pipe1[0], &buf, strlen("s"));
            write(pipe2[1], "s", strlen("s"));
        }
        wait(NULL);
        float averageContextSwitchCost = timeDiff/numOfIterations;    
        printf("average cost of a systemcall: %f\n", averageContextSwitchCost);    

        exit(0);

    }

    return 0;
}