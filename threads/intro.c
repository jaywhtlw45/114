#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

// threads execute in parralel

int mails = 0;
pthread_mutex_t mutex;

void *routine()
{
    // mails is a shared resource, so we need to lock it
    for (int i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex); // lock mutex
        mails++;
        pthread_mutex_unlock(&mutex); // unlock mutex
    }
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL); // initialize mutex

    // pass function to thread to execute, & is used to pass address of t1 and routine
    if (pthread_create(&t1, NULL, &routine, NULL) != 0)
        return EXIT_FAILURE;

    if (pthread_create(&t2, NULL, &routine, NULL) != 0)
        return EXIT_FAILURE;

    // need thread to finish its execution before main thread exits
    if (pthread_join(t1, NULL) != 0)
        return EXIT_FAILURE;

    if (pthread_join(t2, NULL))
        return EXIT_FAILURE;

    pthread_mutex_destroy(&mutex); // destroy mutex

    printf("number of mails = %d\n", mails);
    return 0;
}