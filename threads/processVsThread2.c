#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// processes have separate memory space
    int x = 2;

// file handlers are duplicated for processes, not for threads
int main(int argc, char *argv[]) {\

   int pid = fork();
   if (pid ==-1){
    return 1;
   }

   if (pid ==0)
   {
    x++;
   }
   sleep(2);
    printf("x = %d\n", x);

   printf("Process id %d\n", getpid());
   if (pid!=0)
   {
    wait(NULL);
   }

    return 0;
}