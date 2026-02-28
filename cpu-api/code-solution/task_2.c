#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

 int main(int argc, char *argv[]) {
    printf("hello (pid:%d)\n", (int) getpid());
    int fd = open("./task2.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    int rc = fork();
    if (rc < 0) { 
   // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
    } else if (rc == 0) {
 // child (new process)
        write(fd, "child writing\n", 13);
    } else {
    // parent goes down thipath (main)
    write(fd, "parent\n", 7);
    return 0;
    }
}