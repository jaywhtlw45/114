

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
using namespace std;
int M, N, K;
mutex mt;
int A[100][100], B[100][100];
void threadCallback(int x, int y)
{
    mt.lock();
    cout << "Cell " << x << "," << y << endl;
    mt.unlock();
}
int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout << "insufficient arguments to run" << endl;
        exit(1);
    }
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    K = atoi(argv[3]);
    thread myThreads[M][K];
    string str = "Sample String";
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            myThreads[i][j] = thread(threadCallback, i, j);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            myThreads[i][j].join();
    return 0;
}
