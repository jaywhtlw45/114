#include <unistd.h>    /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <errno.h>     /* Errors */
#include <stdio.h>     /* Input/Output */
#include <sys/wait.h>  /* Wait for Process Termination */
#include <stdlib.h>    /* General Utilities */
#include <string.h>

#include <fstream>
#include <iostream>

using namespace std;

const int SIZE = 1;

int fd[2];
// fd[0] read
// fd[1] write

int main()
{

  // Open Files
  ifstream inFile;
  ofstream outFile;

  inFile.open("sample.pdf", ios::in | ios::binary);
  outFile.open("sample2.pdf", ios::out | ios::binary);

  if (!inFile.is_open())
  {
    cout << "Error opening input file!" << endl;
    return 1;
  }

  if (!outFile.is_open())
  {
    cout << "Error opening output file!" << endl;
    inFile.close();
    return 1;
  }

  // Create Pipe
  pipe(fd);
  if (pipe(fd) == -1)
  {
    cout << "Error creating pipe" << endl;
    return 2;
  }

  char buffer[SIZE];
  int num_bytes;

  // Fork process
  int id = fork();
  if (id == -1)
  {
    cout << "Error with fork" << endl;
    return 3;
  }

  // Child Process
  if (id == 0)
  {
    close(fd[0]);

    while (inFile.read((char *)&buffer, sizeof(buffer)))
    {
      num_bytes = write(fd[1], buffer, sizeof(buffer));
      if (num_bytes == -1)
      {
        cout << "Error writing to file" << endl;
        return 4;
      }
      else
      {
        // cout << "Child: " << buffer << ", " << num_bytes << " bytes" << endl;
        sleep(0.0001);
      }
    }

    close(fd[1]);
    inFile.close();
    outFile.close();

    return 0;
  }
  // Parent Process
  else
  {
    close(fd[1]);
    while (num_bytes = read(fd[0], buffer, sizeof(buffer)))
    {

      if (num_bytes == -1)
      {
        cout << "Error reading from pipe" << endl;
        return 5;
      }
      else
      {
        // cout << "buffer: " << buffer[0] << endl;
        outFile.write((char *)&buffer, sizeof(buffer));
        sleep(0.0001);
      }
    }

    inFile.close();
    outFile.close();
    return (0);
  }

  inFile.close();
  outFile.close();

  int status;
  wait(&status);
  wait(&status);

  printf("END\n");
}

// ask chat gpt to solve and compare answers
// also ask to solve program 1
