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
