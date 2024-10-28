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
![1-1-1-2](https://github.com/YJChina/os_lab1/blob/main/1-1-1-2.png?raw=true)



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

## 步骤三

运行结果

![1-1-3-1]()
![1-1-3-2]()

子进程和父进程地址空间相同,子进程将value修改为1,地址空间和父进程相同

## 步骤四

运行结果

![1-1-4-1]()
![1-1-4-2]()

运行结果与步骤三相同最后在return前给value值加5父进程和子进程分别返回4和6, 地址空间都保持不变

## 步骤五

### system()

代码

```c
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
pid_t pid, pid1;
int value = 0;
/* fork a child process */
pid = fork();
if (pid < 0) { /* error occurred */
fprintf(stderr, "Fork Failed");
return 1;
}
else if (pid == 0) { /* child process */
pid1 = getpid();
printf("child process1 PID: %d\n",pid1);
system("./system");
printf("child process PID: %d\n",pid1);
}
else { /* parent process */
pid1 = getpid();
printf("parent process PID = %d\n",pid1);}
wait(NULL);return 0;}
```

system_call代码

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();
    printf("system_call PID：%d\n", pid);
    return 0;
}
```

运行结果

![1-1-5-1]()

父子进程各自输出自己的PID,子进程调用system系统调用,输出当前(system)进程的PID,system调用完成,子进程继续执行,输出子进程的PID(再次)

### exec族函数

代码

```c
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
pid_t pid, pid1;
int value = 0;
/* fork a child process */
pid = fork();
if (pid < 0) { /* error occurred */
fprintf(stderr, "Fork Failed");
return 1;
}
else if (pid == 0) { /* child process */
pid1 = getpid();
printf("child process1 PID: %d\n",pid1);
execl("./system","system",NULL);
printf("child process PID: %d\n",pid1);
}
else { /* parent process */
pid1 = getpid();
printf("parent process PID = %d\n",pid1);}
wait(NULL);
return 0;}
```

运行结果

![1-1-5-2]()



子进程调用 `execl()` 函数来执行 `system` ，该程序的路径是 `/system`。

`execl()` 执行成功，子进程被 `/system` 替代，后续的代码不会被执行, 只会输出system程序的PID, 即子进程的PID
