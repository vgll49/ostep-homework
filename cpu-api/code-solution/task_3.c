#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

 int main(int argc, char *argv[]) {
    int rc = fork();
    if (rc < 0) { 
   // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
    } else if (rc == 0) {
 // child (new process)
        printf("hello\n");
    } else {
    // parent goes down thipath (main)
    sleep(1);
    printf("goodbye\n");
    return 0;
    }
}