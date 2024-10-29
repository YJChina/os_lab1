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

        // 使用 exec 函数执行子程序
        char *args[] = {"./1-5-1", NULL}; // 注意传入的参数
        execv(args[0], args); // 使用 execv 执行子程序
        perror("execv failed"); // execv 执行失败的处理
        exit(1); // 如果 execv 失败，退出子进程
    } 
    else { /* parent process */
        printf("Parent process PID: %d\n", getpid());
        wait(NULL); // 等待子进程结束
    }

    return 0;
}
