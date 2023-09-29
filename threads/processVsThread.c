#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// threads execute in parralel
// threads share memory space

void* routine() {
    printf("Process id %d\n", getpid());
}


int main(int argc, char *argv[]) {
    pthread_t t1, t2;

    // pass function to thread to execute, & is used to pass address of t1 and routine
    if (pthread_create(&t1, NULL, &routine, NULL) !=0)
        return EXIT_FAILURE;

    if (pthread_create(&t2, NULL, &routine, NULL)!=0)
        return EXIT_FAILURE;

    // need thread to finish its execution before main thread exits
    if (pthread_join(t1,NULL)!=0)
        return EXIT_FAILURE;
    
    if (pthread_join(t2,NULL))
        return EXIT_FAILURE;

    return 0;
}
