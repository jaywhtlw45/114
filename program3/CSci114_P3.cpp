#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

int M = 2, N = 3, K = 4;
// there will be M*K number of threads

vector<vector<int>> matrix_a, matrix_b;

void print_matrices();
void files_to_matrices();

// row, col
// int sum = 0;
// for (int i = 0; i < N; i++)
// {
// sum += matrix_a[row][0] * matrix_b[i][col];
// }

void *multiply_row_column(void *arg)
{
    int sum = 0;
    sum = *((int*)arg);
    cout << "sum: " << sum << endl;
    free(arg);
}

int main()
{
    pthread_t th[1];
    files_to_matrices();

    int *a = (int *)malloc(2 * sizeof(int));
    *a = 1;
    *(a + 1) = 2;
    cout << "we made it" << endl;
    pthread_create(th, NULL, &multiply_row_column, &a);

    // for (int j = 0; j < M; j++)
    // {
    // for (int i = 0; i < K; i++)
    //{
    // int *a = (int *)malloc(2 * sizeof(int));
    // *a = 1;
    // *(a+1)= 2;
    // cout << "we made it" << endl;
    // pthread_create(th + i, NULL, &multiply_row_column, &a);
    //}
    //}

    // for (int j = 0; j < M * K; j++)
    // {
    //     if (pthread_join(th[j], NULL) != 0)
    //         perror("Failed to join thread");
    // }
    // Copy A.txt and B.txt to matrices

    pthread_join(th[0], NULL);
    return 0;
}

void files_to_matrices()
{
    ifstream file_a;
    ifstream file_b;

    // Copy file A to Matrix
    file_a.open("./A.txt");
    if (!file_a.is_open())
    {
        cout << "Error opening matrix a!" << endl;
        return;
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
        matrix_a.push_back(row);
    }
    file_a.close();
    cout << "Finished copying matrix a" << endl;

    // Copy file B to Matrix
    file_b.open("./B.txt");
    if (!file_b.is_open())
    {
        cout << "Error opening matrix b!" << endl;
        file_a.close();
        return;
    }
    for (int j = 0; j < N; j++)
    {
        vector<int> row;
        for (int i = 0; i < K; i++)
        {
            file_b >> num;
            row.push_back(num);
        }
        matrix_b.push_back(row);
    }
    file_b.close();
    cout << "Finished copying matrix b" << endl
         << endl;

    return;
}

void print_matrices()
{
    cout << "Matrix A " << endl;
    for (int j = 0; j < M; j++)
    {
        for (int i = 0; i < N; i++)
        {
            cout << setw(4) << matrix_a[j][i];
        }
        cout << endl;
    }
    cout << "\nMatrix B " << endl;
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < K; i++)
        {
            cout << setw(4) << matrix_b[j][i];
        }
        cout << endl;
    }

    cout << endl;
}
