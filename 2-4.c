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
