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
[link](file:///C:\Users\闫铭博\Pictures\Screenshots\屏幕截图 2024-10-28 164118.png)
![1-1](file:///C:\Users\闫铭博\Pictures\Screenshots\屏幕截图 2024-10-28 164118.png)
