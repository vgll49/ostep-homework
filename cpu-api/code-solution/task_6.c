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
        printf("first\n");
    } else {
    // parent goes down thipath (main)
    w = waitpid(rc, NULL, 0);
    // wait returns pid_t, id of process that is finished 
    printf("second\n");
    return 0;
    }
}