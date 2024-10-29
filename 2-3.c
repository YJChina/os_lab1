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
