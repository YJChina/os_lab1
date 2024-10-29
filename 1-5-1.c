#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Executing child_program...\n");
    printf("Process PID: %d\n", getpid());
    printf("Parent process PID: %d\n", getppid());
    return 0;
}
