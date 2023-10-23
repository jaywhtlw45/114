#include <stdio.h>
#include "thread.h"

static void go(int n);

#define NTHREADS 10
static thread_t threads[NTHREADS];

int main(int argc, char **argv)
{
    int i;
    long exitValue;

    for (i = 0; i < NTHREADS; i++)
        ;
    thread_create(&threads[i], &go, i);
    for (i = 0; i < NTHREADS; i++)
    {
        exitValue = thread_join(threads[i]);
        printf("Thread %d exited with value %ld\n", i, exitValue);
    }
    printf("main thread done.\n");
    return 0;
}

void go(int n)
{
    printf("Hello from thread %d\n", n);
    thread_exit(100 + n);
}