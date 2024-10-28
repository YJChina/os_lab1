# os_lab1

## 步骤一 (直接测试)

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

![1-1-1-1](https://github.com/YJChina/os_lab1/blob/main/1-1-1-1.png?raw=true)

程序创建一个子进程，父进程和子进程分别输出pid和pid1，pid1为各自进程的PID,子进程的PID为fork()的返回值0,父进程的PID为子进程的PID,其中父进程的PID=2602,子进程的PID=2603；因此输出父进程的pid=2603,pid1=2602;子进程的pid=0,pid1=2603。

![1-1-1-2](https://github.com/YJChina/os_lab1/blob/main/1-1-1-2.png?raw=true)

## 步骤二 (去除wait)

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
![1-1-2-1](https://github.com/YJChina/os_lab1/blob/main/1-1-2-1.png?raw=true)

wait()父进程等待子进程结束,避免他成为僵尸进程而占用系统资源,父进程在没有调用wait()的情况下，会继续执行，可能会在子进程结束之前结束，会导致子进程变成孤儿进程。去掉wait,父进程将不会等待子进程结束，同时输出顺序可能会变得不可预测，导致不容易理解程序的执行流程。

![1-1-2-2](https://github.com/YJChina/os_lab1/blob/main/1-1-2-2.png?raw=true)
输出顺序变得不可预测

## 步骤三 (全局变量)

代码

```c

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


```
运行结果

![1-1-3-1](https://github.com/YJChina/os_lab1/blob/main/1-1-3-1.png)

全局变量的地址：在父进程和子进程中，全局变量counter的地址是相同的。虽然fork()创建了父进程的副本，但由于每个进程的内存空间是独立的，因此它们各自持有counter的独立副本，访问相同的内存地址。

独立性：父进程和子进程对counter的操作是独立的。在子进程中，counter的值被增加到1，而在父进程中，counter的值被减少到-1。这表明，尽管它们访问的是同一地址，但在fork()后，父子进程各自的counter值是独立的。

输出顺序：由于父进程使用wait()函数等待子进程结束，因此子进程的输出会在父进程的输出之前。

## 步骤四 

代码

```c

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
        counter += 2; // 再次修改子进程的counter值
        printf("child: counter after second increment = %d\n", counter); // 输出修改后的counter值
    } 
    else { /* parent process */
        counter += -1; // 父进程对全局变量进行操作
        wait(NULL); // 等待子进程结束
        printf("parent: counter after increment = %d\n", counter); // 输出父进程的counter值
        printf("parent: counter address = %p\n", (void*)&counter); // 输出父进程中counter的地址
        counter += -2; // 修改父进程的counter值
        printf("parent: counter after second increment = %d\n", counter); // 输出修改后的counter值
    }

    return 0;
}


```

运行结果

![1-1-4-1](https://github.com/YJChina/os_lab1/blob/main/1-1-4-1.png)

在子进程中，counter 的初始值 0 增加到 1，然后又增加到 3。这些修改仅存在于子进程中。
父进程对 counter 的操作则是从初始值 0 先减少到 -1，然后再减少到 -3。同样，这些修改仅存在于父进程中。
无论在父进程还是子进程中，counter 的地址是相同的。这是因为 fork() 创建了父进程的副本，子进程获得了父进程的整个地址空间的一个拷贝，包括所有全局变量的地址。

## 步骤五(调用函数)

### system()

代码

主程序

```c

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


```

子程序

```c

#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Executing child_program...\n");
    printf("Process PID: %d\n", getpid());
    printf("Parent process PID: %d\n", getppid());
    return 0;
}

```

运行结果

![1-1-5-1](https://github.com/YJChina/os_lab1/blob/main/1-1-5-1.png)

在子进程中，输出的父进程 PID 是主程序的 PID
由于 1-5-1(system_call) 是由子进程调用的，因此它的父进程 PID 与子进程的 PID 相同。


### exec族函数

代码

主程序

```c
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


```

运行结果

![1-1-5-2](https://github.com/YJChina/os_lab1/blob/main/1-1-5-2.png)


在主程序中，使用 fork() 创建子进程，输出子进程的 PID 和父进程的 PID。

exec 调用：在子进程中调用 execv() 函数执行 1-5-1。execv() 需要传入一个参数数组，最后一个元素必须为 NULL。

当执行 1-5-1 后，1-5-1 的父进程 PID 仍然是主程序的 PID，因为 1-5-1 是由子进程创建的。

