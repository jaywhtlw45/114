// Jason Whitlow
// CSCI 114 Program 3
// 10/7/2023

// Program Explanation:
// This program accepts a matrix from A.txt and B.txt.
// The program then multiplies the two matrices using threads and outputs the result to console.

// Notes about Program:
// 1. There are multiple approaches to take when multiplying matrices using threads.
//    The approach this program takes is to create a thread for each cell in the resulting matrix, matrix C.
// 2. Each thread can only access the cell it is responsible for which means there is NO NEED for a lock on thread.
//    A lock is still used to demonstrate how to use a lock in a thread.
// 3. For simplicity, the program first loads the matrices from the files into vectors.
//    This is less efficient than loading the matrices directly into the resulting matrix, matrix C.

// Run Program:
// g++ -o a.out CSci114_P3.cpp;
// ./a.out M N K; where M, N, and K are the dimensions of the matrices A and B.

#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <mutex>

using namespace std;

// Matrices A, B, C
vector<vector<int>> A, B, C;

// M = (rows of A), N = (cols of A) = (rows of B), K = (cols of B)
int M, N, K; // i, j, k respectively
int threads = M * K;
mutex mt;

// Read files from A.txt and B.txt and store in matrix A and B
int files_to_matrices();

void init_matrix_C();
void print_matrices();

// Multiply cell i,k in matrix C
void multiply_cell(int i, int k)
{
    for (int j = 0; j < N; j++)
    {
        mt.lock();
        C[i][k] += A[i][j] * B[j][k];
        mt.unlock();
    }
}

int main(int argc, char **argv)
{
    // Accept command line arguments
    if (argc < 4)
    {
        cout << "Insufficient arguments to run" << endl;
        exit(1);
    }
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    K = atoi(argv[3]);

    // Copy files to matrices A and B
    if (files_to_matrices() == -1)
    {
        cout << "Error reading files" << endl;
        exit(1);
    }

    // Initialize Matrix C to 0
    init_matrix_C();

    // Create thread array
    thread th[M][K];

    // Start threads
    for (int i = 0; i < M; i++)
    {
        for (int k = 0; k < K; k++)
        {
            th[i][k] = thread(multiply_cell, i, k);
        }
    }

    // Join threads
    for (int i = 0; i < M; i++)
    {
        for (int k = 0; k < K; k++)
        {
            th[i][k].join();
        }
    }

    cout << endl;
    print_matrices();

    return 0;
}

void init_matrix_C()
{
    for (int j = 0; j < M; j++)
    {
        vector<int> row;
        for (int i = 0; i < K; i++)
        {
            row.push_back(0);
        }
        C.push_back(row);
    }
}

int files_to_matrices()
{
    ifstream file_a;
    ifstream file_b;

    // Copy file A to Matrix A
    file_a.open("./A.txt");
    if (!file_a.is_open())
    {
        cout << "Error opening matrix A!" << endl;
        return -1;
    }
    int num;
    for (int j = 0; j < M; j++)
    {
        vector<int> row;
        for (int i = 0; i < N; i++)
        {
            file_a >> num;
            row.push_back(num);
        }
        A.push_back(row);
    }
    file_a.close();

    // Copy file B to Matrix B
    file_b.open("./B.txt");
    if (!file_b.is_open())
    {
        cout << "Error opening matrix B!" << endl;
        file_a.close();
        return -1;
    }
    for (int j = 0; j < N; j++)
    {
        vector<int> row;
        for (int i = 0; i < K; i++)
        {
            file_b >> num;
            row.push_back(num);
        }
        B.push_back(row);
    }
    file_b.close();

    return 0;
}

void print_matrices()
{
    cout << "Matrix A " << endl;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << setw(4) << A[i][j];
        }
        cout << endl;
    }
    cout << "\nMatrix B " << endl;
    for (int j = 0; j < N; j++)
    {
        for (int k = 0; k < K; k++)
        {
            cout << setw(4) << B[j][k];
        }
        cout << endl;
    }

    cout << "\nMatrix C " << endl;
    for (int i = 0; i < M; i++)
    {
        for (int k = 0; k < K; k++)
        {
            cout << setw(4) << C[i][k];
        }
        cout << endl;
    }
    cout << endl;
}
