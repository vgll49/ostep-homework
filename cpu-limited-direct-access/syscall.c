#include <stdio.h>
#include <unistd.h> // read
#include <sys/time.h> //gettimeofdays
int main() {
    printf("start loop...\n");
    char buff[1];
    struct timeval tv;
    double timeDiff = 0;
    int numOfIterations = 99;
    for(int i = 0; i < numOfIterations; i++) {
        gettimeofday(&tv, NULL);
        double timePre = (tv.tv_sec * 1000000) + tv.tv_usec;
        read(0, &buff, 0);
        gettimeofday(&tv, NULL);
        double timePost = (tv.tv_sec * 1000000) + tv.tv_usec;
        timeDiff += timePost-timePre;
    }
    float averageSysCallCost = timeDiff/numOfIterations;
    printf("average cost of a systemcall: %f\n", averageSysCallCost);
    return 0;
}
