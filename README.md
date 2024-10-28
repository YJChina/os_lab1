# os_lab1

## 步骤一

代码

```c
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
  pid_t pid, pid1;

  /* fork a child process */
  pid = fork();  
  if (pid < 0) { /* error occurred */
    fprintf(stderr, "Fork Failed");
    return 1;
  }
  else if (pid == 0) { /* child process */
    pid1 = getpid();
    printf("child: pid = %d",pid); /* A */
    printf("child: pid1 = %d",pid1); /* B */
  }
  else { /* parent process */
    pid1 = getpid();
    printf("parent: pid = %d",pid); /* C */
    printf("parent: pid1 = %d",pid1); /* D */
    wait(NULL);
  }

return 0;
}
```

运行结果

![1-1-1-1](https://github.com/YJChina/os_lab1/blob/main/1-1.png?raw=true)
![1-1-1-2](



程序创建一个子进程，父进程和子进程分别输出pid和pid1，pid1为各自进程的PID,子进程的PID为fork()的返回值0,父进程的PID为子进程的PID,其中父进程的PID=2602,子进程的PID=2603；因此输出父进程的pid=2603,pid1=2602;子进程的pid=0,pid1=2603。

## 步骤二

代码

```c
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
  pid_t pid, pid1;

  /* fork a child process */
  pid = fork();  
  if (pid < 0) { /* error occurred */
    fprintf(stderr, "Fork Failed");
    return 1;
  }
  else if (pid == 0) { /* child process */
    pid1 = getpid();
    printf("child: pid = %d",pid); /* A */
    printf("child: pid1 = %d",pid1); /* B */
  }
  else { /* parent process */
    pid1 = getpid();
    printf("parent: pid = %d",pid); /* C */
    printf("parent: pid1 = %d",pid1); /* D */
    
  }

return 0;
}
```

运行结果
![1-1-2-1](https://github.com/YJChina/os_lab1/blob/main/1-2.png?raw=true)

wait()父进程等待子进程结束,避免他成为僵尸进程而占用系统资源,父进程在没有调用wait()的情况下，会继续执行，可能会在子进程结束之前结束，会导致子进程变成孤儿进程。去掉wait,父进程将不会等待子进程结束，同时输出顺序可能会变得不可预测，导致不容易理解程序的执行流程。

![1-1-2-2](https://github.com/YJChina/os_lab1/blob/main/1-2-2.png?raw=true)
输出顺序变得不可预测
