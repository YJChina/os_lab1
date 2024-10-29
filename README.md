# os_lab1

## 实验1-1

### 步骤一 (直接测试)

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

程序创建一个子进程，父进程和子进程分别输出pid和pid1，pid1为各自进程的PID,子进程的PID为fork()的返回值0,父进程的PID为子进程的PID,

其中父进程的PID=2602,子进程的PID=2603；因此输出父进程的pid=2603,pid1=2602;子进程的pid=0,pid1=2603。

![1-1-1-2](https://github.com/YJChina/os_lab1/blob/main/1-1-1-2.png?raw=true)

### 步骤二 (去除wait)

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

wait()父进程等待子进程结束,避免他成为僵尸进程而占用系统资源,父进程在没有调用wait()的情况下，会继续执行，可能会在子进程结束之前结束，会导致子进程变成孤儿进程。

![1-1-2-2](https://github.com/YJChina/os_lab1/blob/main/1-1-2-2.png?raw=true)

去掉wait,父进程将不会等待子进程结束，同时输出顺序可能会变得不可预测，导致不容易理解程序的执行流程。


### 步骤三 (全局变量)

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

### 步骤四 

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

### 步骤五(调用函数)

#### system()

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

子程序 '1-5-1'

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


#### exec族函数

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

当执行 1-5-1 后，1-5-1 的父进程 PID 仍然是主程序的 PID，因为 1-5-1 不会创建新进程，而是直接替换当前进程的映像。

## 实验1-2

### 步骤一

代码

```c

#include <stdio.h>
#include <pthread.h> // 引入 pthread 库以支持多线程

#define NUM_THREADS 2         // 定义线程数量为 2
#define NUM_OPERATIONS 1000000 // 每个线程执行的操作次数

int shared_variable = 0;      // 定义一个共享变量，初始值为 0

// 线程函数，执行自增操作
void* thread_function(void* thread_id) {
    long tid = (long)thread_id; // 将传入的线程 ID 转换为 long 类型
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        shared_variable++;       // 对共享变量进行自增操作
    }
    // 打印线程创建成功的信息
    printf("thread%ld create success!\n", tid);
    pthread_exit(NULL); // 退出线程
}

int main() {
    pthread_t threads[NUM_THREADS]; // 定义线程数组以存储线程 ID

    // 创建多个线程
    for (long i = 0; i < NUM_THREADS; i++) {
        // 创建新线程，并指定线程函数和线程 ID
        int result = pthread_create(&threads[i], NULL, thread_function, (void*)i);
        if (result) {
            // 如果线程创建失败，打印错误信息并退出
            printf("Error creating thread %ld. Return code: %d\n", i, result);
            return 1;
        }
    }

    // 等待所有线程完成
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // 阻塞主线程，直到指定线程结束
    }

    // 输出最终的共享变量值
    printf("variable result: %d\n", shared_variable);

    return 0; // 正常结束程序
}


```

运行结果

![1-2-1](https://github.com/YJChina/os_lab1/blob/main/1-2-1.png)

目标是两个线程分别对shared_variable加 `NUM_OPERATIONS`次

然而程序的实现并没有对共享变量的访问进行同步，这导致数据竞争和不确定的结果,所以得到的`variable result`不确定

### 步骤二

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 2         // 定义线程数量
#define NUM_OPERATIONS 1000000 // 每个线程执行的操作次数

int shared_variable = 0;      // 共享变量
sem_t semaphore;              // 定义信号量

// 线程函数，执行自增操作
void* thread_function(void* thread_id) {
    long tid = (long)thread_id; // 将线程 ID 转换为 long 类型
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        // P 操作：等待信号量，确保只有一个线程可以访问临界区
        if (sem_wait(&semaphore) != 0) {
            perror("sem_wait failed"); // 如果 sem_wait 失败，打印错误信息
            pthread_exit(NULL);         // 退出线程
        }
        shared_variable++;              // 自增共享变量
        // V 操作：释放信号量，允许其他线程访问
        if (sem_post(&semaphore) != 0) {
            perror("sem_post failed"); // 如果 sem_post 失败，打印错误信息
            pthread_exit(NULL);         // 退出线程
        }
    }
    printf("thread%ld create success!\n", tid); // 打印线程创建成功的信息
    pthread_exit(NULL); // 退出线程
}

int main() {
    sem_init(&semaphore, 0, 1); // 初始化信号量，初值为1，表示互斥访问

    pthread_t threads[NUM_THREADS]; // 定义线程数组

    // 创建多个线程
    for (long i = 0; i < NUM_THREADS; i++) {
        int result = pthread_create(&threads[i], NULL, thread_function, (void*)i);
        if (result) {
            // 如果线程创建失败，打印错误信息并退出
            printf("Error creating thread %ld. Return code: %d\n", i, result);
            return 1;
        }
    }

    // 等待所有线程完成
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // 阻塞主线程，直到指定线程结束
    }

    sem_destroy(&semaphore); // 销毁信号量，释放资源

    // 输出最终的共享变量值
    printf("variable result: %d\n", shared_variable);

    return 0; // 正常结束程序
}


```

运行结果

![1-2-2](https://github.com/YJChina/os_lab1/blob/main/1-2-2.png)

使用信号量来保护共享变量, `sem_init()` 初始化一个二值信号量，初值为1，保证了只有一个线程可以访问共享变量。

在 `thread_function` 中，使用 `sem_wait()` 等待信号量，然后进行共享变量的操作，之后使用 `sem_post()` 发信号量。

这样没有竞态条件的影响下, 共享变量的值为2000000

### 步骤三

代码

```c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>

#define NUM_THREADS 2

sem_t semaphore;

// 线程函数，执行子程序
void* thread_function(void* thread_id) {
    long tid = (long)thread_id; // 将传入的线程 ID 转换为 long 类型

    // 打印线程创建成功的信息以及线程和进程 ID
    printf("Thread %ld create success!\n", tid);
    printf("Thread %ld tid = %ld, pid = %d\n", tid, (long int)syscall(SYS_gettid), getpid());

    // 调用系统命令
    system("./1-5-1"); 
    printf("Thread %ld system call return\n", tid); // 打印系统调用返回的信息
    
    pthread_exit(NULL); // 退出线程
}

int main() {
    sem_init(&semaphore, 0, 1); // 初始化信号量，初值为 1，表示互斥访问

    pthread_t threads[NUM_THREADS]; // 定义线程数组以存储线程 ID

    // 创建多个线程
    for (long i = 0; i < NUM_THREADS; i++) {
        // 创建新线程，并指定线程函数和线程 ID
        int result = pthread_create(&threads[i], NULL, thread_function, (void*)i);
        if (result) {
            // 如果线程创建失败，打印错误信息并退出
            printf("Error creating thread %ld. Return code: %d\n", i, result);
            return 1;
        }
    }

    // 等待所有线程完成
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // 阻塞主线程，直到指定线程结束
    }

    sem_destroy(&semaphore); // 销毁信号量，释放资源
    return 0; // 正常结束程序
}

```

运行结果

![1-2-3](https://github.com/YJChina/os_lab1/blob/main/1-2-3.png)



代码

```c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>        // 引入 pthread 库以支持多线程
#include <semaphore.h>      // 引入信号量库
#include <sys/syscall.h>   // 引入 syscall 函数以获取线程 ID

#define NUM_THREADS 2      // 定义线程数量为 2
#define NUM_OPERATIONS 1000000 // 定义每个线程执行的自增次数

sem_t semaphore;            // 定义信号量
int shared_variable = 0;    // 定义共享变量

// 线程函数，执行自增操作并模拟子程序功能
void* thread_function(void* thread_id) {
    long tid = (long)thread_id; // 将传入的线程 ID 转换为 long 类型

    for (int i = 0; i < NUM_OPERATIONS; i++) {
        sem_wait(&semaphore);    // P 操作：等待信号量，确保只有一个线程可以访问临界区
        shared_variable++;        // 自增共享变量
        sem_post(&semaphore);     // V 操作：释放信号量，允许其他线程访问
    }

    // 打印线程创建成功的信息以及线程和进程 ID
    printf("Thread %ld create success!\n", tid);
    printf("Thread %ld TID = %ld, PID = %ld\n", tid, (long int)syscall(SYS_gettid), getpid());
    
    // 模拟 exec 函数的调用
    char *args[] = {"./1-5-1", NULL}; // 子程序的参数
    printf("Thread %ld executing child_program...\n", tid);
    
    // 使用 execv 执行子程序
    execv(args[0], args); // 使用 execv 执行子程序
    perror("execv failed"); // execv 执行失败的处理
    pthread_exit(NULL); // 退出线程
}

int main() {
    sem_init(&semaphore, 0, 1); // 初始化信号量，初值为 1，表示互斥访问

    pthread_t threads[NUM_THREADS]; // 定义线程数组以存储线程 ID

    // 创建多个线程
    for (long i = 0; i < NUM_THREADS; i++) {
        // 创建新线程，并指定线程函数和线程 ID
        int result = pthread_create(&threads[i], NULL, thread_function, (void*)i);
        if (result) {
            // 如果线程创建失败，打印错误信息并退出
            printf("Error creating thread %ld. Return code: %d\n", i, result);
            return 1;
        }
    }

    // 等待所有线程完成
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // 阻塞主线程，直到指定线程结束
    }

    sem_destroy(&semaphore); // 销毁信号量，释放资源
    return 0; // 正常结束程序
}


```

运行结果

![1-2-4](https://github.com/YJChina/os_lab1/blob/main/1-2-4.png)

在同一进程中, 不同线程的`tid`不同,但`pid`相同,调用`execl()`程序,  来执行 `system` ，该程序的路径是 `/1-5-1`。

`execl()` 执行成功，子进程被 `/system` 替代，后续的代码不会被执行, 只会输出system程序的PID, 即子进程的PID。

因此子进程未执行的另一个线程也不会执行而是直接被`system`程序替代,  故只会输出一个线程`tid`和`pid`,另一个线程将不会被创建
