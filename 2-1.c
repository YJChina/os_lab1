#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 2
#define NUM_OPERATIONS 1000000

int shared_variable = 0;

void* thread_function(void* thread_id) {
    long tid = (long)thread_id;
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        shared_variable++;
    }
    printf("thread%ld create success!\n", tid);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        int result = pthread_create(&threads[i], NULL, thread_function, (void*)i);
        if (result) {
            printf("Error creating thread %ld. Return code: %d\n", i, result);
            return 1;
        }
    }

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("variable result: %d\n", shared_variable);

    return 0;
}