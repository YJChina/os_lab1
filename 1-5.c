#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } 
    else if (pid == 0) { /* child process */
        printf("Child process PID: %d\n", getpid());
        printf("Child process Parent PID: %d\n", getppid());
        system("./1-5-1"); // 调用子程序
    } 
    else { /* parent process */
        printf("Parent process PID: %d\n", getpid());
        wait(NULL); // 等待子进程结束
    }

    return 0;
}
