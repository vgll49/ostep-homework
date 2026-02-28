#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

 int main(int argc, char *argv[]) {
    printf("hello (pid:%d)\n", (int) getpid());
    int x = 100;
    printf("value of x in parent is %d,\n", x);
    x = 17;
    printf("parent changed val to 17: %d\n", x);
    int rc = fork();

    if (rc < 0) {
    // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
    } else if (rc == 0) {
 // child (new process)
        printf("child (pid:%d)\n", (int) getpid());
        printf("value of x in child is %d,\n", x);
        x = 20;
        printf("child changed val to 20: %d\n", x);
    } else {
    // parent goes down this path (main)
    printf("parent of %d (pid:%d)\n",
         rc, (int) getpid());
     }
    return 0;
 }