#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> // getpid


int main(int argc, char* argv[]) {

    int pid = getpid();
    printf("pid of memory-user.c is: %d\n", pid);
    if(argc < 2) {
        printf("no argument provided...\n");
        return 1;
    }
        
    long mem = atoi(argv[1]);

    if(mem < 1) {
        printf("at least 1 byte has to be allocated...\n");
        return 1;
    }

    long bytes = mem * 1024 * 1024;

    char* arr = malloc(sizeof(char) * bytes);

    if (arr == NULL) {
        printf("Error: memory allocation failed\n");
        return 1;
    }

    printf("starting infinite loop with %ld megabytes...\n", bytes/(1024*1024));
    for(long j = 0;; j++) {
        for(long i = 0; i<bytes; i++) {
            arr[i] = 0;
        
        }
    }
  
}