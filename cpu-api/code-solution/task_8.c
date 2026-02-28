#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

 int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);

    pid_t pid1 = fork();

    if (pid1 < 0) { 
   // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
    } 
    
    if (pid1 == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        printf("child1 output\n");
        // use write end of the pipe, pipefd1

        exit(0);
    } 

    pid_t pid2 = fork();

    if (pid2 < 0) {
        fprintf(stderr, "fork failed\n");
    }

    if(pid2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        // use read end of pipe, pipefd0
        char buffer[100];
        ssize_t n = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
        if (n > 0) {
            buffer[n] = '\0'; // null terminate
            printf("child2 read: %s", buffer);
        }


        exit(0);
    } else {
        printf("parent\n");
        close(fd[0]);
        close(fd[1]);
    }
}