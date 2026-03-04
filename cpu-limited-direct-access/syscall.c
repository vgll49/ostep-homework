#include <stdio.h>
#include <unistd.h> // read
#include <sys/time.h> //gettimeofdays
int main() {
    printf("start loop...\n");
    char buff[1];
    struct timeval tv;
    long timeDiff = 0;
    int numOfIterations = 99;
    for(int i = 0; i < numOfIterations; i++) {
        gettimeofday(&tv, NULL);
        int timePre = tv.tv_usec;
        printf("pre %ld\n", tv.tv_usec);
        read(0, &buff, 0);
        gettimeofday(&tv, NULL);
        int timePost = tv.tv_usec;
        timeDiff += timePost-timePre;
        printf("post: %ld\n", tv.tv_usec);
    }
    float averageSysCallCost = timeDiff/numOfIterations;
    printf("average cost of a systemcall: %f\n", averageSysCallCost);
    return 0;
}
