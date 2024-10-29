#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int counter = 0; // 全局变量

int main() {
    pid_t pid;

    /* fork a child process */
    pid = fork();  
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } 
    else if (pid == 0) { /* child process */
        counter += 1; // 子进程对全局变量进行操作
        printf("child: counter after increment = %d\n", counter); // 输出子进程的counter值
        printf("child: counter address = %p\n", (void*)&counter); // 输出子进程中counter的地址
    } 
    else { /* parent process */
        counter += -1; // 父进程对全局变量进行操作
        wait(NULL); // 等待子进程结束
        printf("parent: counter after increment = %d\n", counter); // 输出父进程的counter值
        printf("parent: counter address = %p\n", (void*)&counter); // 输出父进程中counter的地址
    }

    return 0;
}
