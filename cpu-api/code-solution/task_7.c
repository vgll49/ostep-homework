#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

 int main(int argc, char *argv[]) {
    int rc = fork();
    pid_t w; 
    if (rc < 0) { 
   // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
    } else if (rc == 0) {
        close(STDOUT_FILENO);
        printf("child\n");
        // works bc only fd one is closed 
        fprintf(stderr, "this works\n");
    } else {
    // parent goes down thipath (main)
    printf("parent\n");
    return 0;
    }
}