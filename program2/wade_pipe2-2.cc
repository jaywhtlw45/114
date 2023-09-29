#include <unistd.h>    /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <errno.h>     /* Errors */
#include <stdio.h>     /* Input/Output */
#include <sys/wait.h>  /* Wait for Process Termination */
#include <stdlib.h>    /* General Utilities */
#include <string.h>

int A_to_B[2];

int main()
{
  int status;

  pipe(A_to_B);
  int i = 0, j = 0;
  if (fork() == 0)
  {
    char buf1[100] = "good"; // buf1="good"+'\0'+"95 character of anything"
    printf("IN PA\n");
    close(A_to_B[0]); // it is closed in this process but does not close for other processes
    for (i = 0; i < 10; i++)
    {
      printf("PA: %s\n", buf1);
      write(A_to_B[1], buf1, sizeof(buf1));
      printf("sizeof(buf1): %d\n", sizeof(buf1));
      sleep(1);
    }
    strcpy(buf1, "done");
    write(A_to_B[1], buf1, sizeof(buf1));
    close(A_to_B[1]);
    return 0;
  }
  if (fork() == 0)
  {
    char buf2[100];
    printf("IN PB\n");
    close(A_to_B[1]);
    for (j = 0; j < 15; j++)
    {
      sleep(1);
      int nbytes = read(A_to_B[0], buf2, sizeof(buf2));
      if (nbytes <= 0)
        printf("pipe empty %s\n", buf2);
      else
        printf("PB bytes:%d %s\n", nbytes, buf2);

      if (strcmp(buf2, "done") == 0)
      {
        printf("Done!\n");
        break;
      }
    }
    return (0);
  }
  wait(&status);
  wait(&status);
  printf("END\n");
}
