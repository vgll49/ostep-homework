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
        //execl("/bin/ls", "ls");
        //execle("/bin/ls", "ls");
        //execlp("/bin/ls", "ls");
        //char *args[] = {"ls", NULL};
        //execv("/bin/ls", args);
        execl("/bin/ls", "ls");
        printf("this shold not print");
    } else {
    // parent goes down thipath (main)
    printf("goodbye\n");
    return 0;
    }
}