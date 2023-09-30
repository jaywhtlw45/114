// Jason Whitlow
// Program2
// CSCI 114
// 9/20/2023

// This program will read data from a file and write it to another file using a pipe.
// To run the program use the following commands:

// g++ -o a.out CSci114_P2.cpp
// ./a.out

#include <unistd.h>    // Symbolic Constants
#include <sys/types.h> // Primitive System Data Types
#include <errno.h>     // Errors
#include <stdio.h>     // Input/Output
#include <sys/wait.h>  // Wait for Process Termination
#include <stdlib.h>    // General Utilities
#include <iostream>    // Write to console
#include <fstream>     // Read/Write to files

using namespace std;

const int SIZE = 1024; // Buffer Size

int main()
{
    // Open Files
    ifstream inFile("data.in", ios::in | ios::binary);
    ofstream outFile("data.out", ios::out | ios::binary);

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
    int fd[2];
    if (pipe(fd) == -1)
    {
        cout << "Error creating pipe" << endl;
        inFile.close();
        outFile.close();
        return 2;
    }

    // Fork Process
    pid_t id = fork();
    if (id == -1)
    {
        cout << "Error with fork" << endl;
        inFile.close();
        outFile.close();
        close(fd[0]);
        close(fd[1]);
        return 3;
    }

    char buffer[SIZE];
    int num_bytes;

    // Child Process
    if (id == 0)
    {
        close(fd[0]); // Close the read end of the pipe in the child process

        while ((num_bytes = inFile.read(buffer, sizeof(buffer)).gcount()) > 0)
        {
            if (write(fd[1], buffer, num_bytes) == -1)
            {
                cout << "Error writing to pipe" << endl;
                close(fd[1]);
                inFile.close();
                outFile.close();
                return 4;
            }
        }

        close(fd[1]); // Close the write end of the pipe in the child process
        inFile.close();
        outFile.close();
        return 0;
    }
    // Parent Process
    else
    {
        close(fd[1]); // Close the write end of the pipe in the parent process

        while ((num_bytes = read(fd[0], buffer, sizeof(buffer))) > 0)
        {
            if (outFile.write(buffer, num_bytes).fail())
            {
                cout << "Error writing to output file" << endl;
                close(fd[0]);
                inFile.close();
                outFile.close();
                return 5;
            }
        }

        close(fd[0]); // Close the read end of the pipe in the parent process
        inFile.close();
        outFile.close();

        int status;
        waitpid(id, &status, 0); // Wait for the child process to finish
        return 0;
    }
}
