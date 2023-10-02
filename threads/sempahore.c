#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t semaphore;
// key difference between mutex and lock is that semaphore can be initialized to 2 or more
// this allows 2 or more threads to access the critical section at the same time

void *routine(void *args)
{
    // semwait checks the sempaphores value, if it is zero and can no longer be decremented, the thread will be blocked
    // if the value is greater than zero, it will be decremented and the thread will continue
    sem_wait(&semaphore);
    sleep(1);
    printf("Hello from thread %d\n", *(int *)args);
    sem_post(&semaphore);
    // sempost increments the sempaphore value, if the value is zero and there are threads waiting, one of them will be unblocked
    free(args);
}

int main(int argc, char *argv[])
{
    pthread_t th[THREAD_NUM];
    sem_init(&semaphore, 0, 1); // 0 is for threads, 1 is for the initial value of the semaphore
    int i;
    for (i = 0; i < THREAD_NUM; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0)
        {
            perror("Failed to create thread ");
        }
    }

    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semaphore);
    return 0;
}

// two operations can be done on a semaphore, wait and post
// wait is similar to lock on mutex, post is similar to unlock on mutex