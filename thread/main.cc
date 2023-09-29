#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* routine(){
    printf("Test from thread\n")
}

int main(int argc, char* argv[]){
    pthread_t t1;
    pthread_create();
    return 0;
}