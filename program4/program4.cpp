#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <condition_variable>
#include <mutex>

using namespace std;

// Global Vars
mutex mt;
condition_variable cv;
bool debug = true; // set to true to print debug statements

//-!!!!!!!!!!!!!!!!!!!!!!!!!! REMEMBER TO SWITCH TO COMMAND LINE ARGS

int T = 5; // Number of threads
int R = 6; // Number of resources
int avail; // instances of resource i available

vector<int> maxn;  // maximum need of resource i for thread j, this is loaded from request vector
vector<int> alloc; // allocated resource i to thread j
vector<int> need;  // need of resource i for thread j
vector<int> state; // state of thread j, 0 = running, 1 = blocked, 2 = terminated
vector<vector<int>> requests;

void initializeVectors();           // initialize vectors to zero
void processRequests(int threadId); // starts each thread, continue until request vector is empty
bool isSafe(int threadId);          // checks the state of the system
void request(int threadId);         // request resource i for thread j
bool wouldBeSafe(int threadId);     // if request is granted, would the system be in a safe state?

void test()
{
    cout << "test" << endl;
    sleep(1);
}

// helper functions
void readFile();
void printVector(vector<int> v, string name);
void printVector(vector<vector<int>> v, string name);

int main(int argc, char *argv[])
{
    // Accept command line arguments
    // if (argc < 3)
    // {
    //     cout << "Usage: " << argv[0] << " <R> <T>" << endl;
    //     return 1;
    // }
    // const char *arg1 = argv[1];
    // const char *arg2 = argv[2];
    // T = atoi(arg1);
    // R = atoi(arg2);

    // read from file and initialize vectors
    readFile();
    initializeVectors();

    // Create threads
    thread th[T];
    for (int i = 0; i < T; i++)
        th[i] = thread(processRequests, i);

    // printVector(requests, "req");
    // printVector(maxn, "maxn");
    // printVector(alloc, "alloc");
    // printVector(need, "need");

    for (int i = 0; i < T; i++)
        th[i].join();

    return 0;
}

void getNextRequest(int threadId)
{
    unique_lock<mutex> mlock(mt);

    if (!requests[threadId].empty())
    {
        int req = requests[threadId].front();
        maxn[threadId] = req;
        requests[threadId].erase(requests[threadId].begin());

        if (debug)
            printf("maxn[%d] = %d\n", threadId, maxn[threadId]);
    }

    // cv.notify_one();
}

void releaseResources(int threadId)
{
    unique_lock<mutex> mlock(mt);
    if (debug)
        printf("Thread %d is releasing %d\n", threadId, alloc[threadId]);
    avail += alloc[threadId];
    alloc[threadId] = 0;

    maxn[threadId] = 0;
    cv.notify_all();
}

void processRequests(int threadId)
{
    while (!requests[threadId].empty())
    {
        getNextRequest(threadId);
        // Request resources
        while (alloc[threadId] < maxn[threadId])
        {
            request(threadId);
            printVector(alloc, "alloc");
            sleep(.01);
        }
        releaseResources(threadId);
    }
    if (debug)
        printf("Thread %d has finished\n", threadId);
}

// A state is safe if there exists a safe sequence of grants that are sufficient
// to allow all threads to eventually receive their maximum resource needs.
void request(int threadId)
{
    unique_lock<mutex> mlock(mt);

    // Grant request if safe, otherwise wait
    assert(isSafe(threadId));

    if (debug)
        cout << "\tth[" << threadId << "] req1 " << endl;

    bool flag = false;
    int num = 0;
    while (!wouldBeSafe(threadId))
    {
        if (debug)
            cout << "\tth[" << threadId << "] waiting*************************" << endl;
        flag = true;
        num = 1;
        cv.wait(mlock);
    }

    if (num == 1)
        cout << "\tth[" << threadId << "] done waiting----------------------------" << endl;
    if (debug)
        cout << "\tth[" << threadId << "] granted" << endl;

    // Grant request
    alloc[threadId] += 1;
    avail -= 1;
    // assert(isSafe(threadId));

    // Print state
    cout << "alloc"
         << ":" << setw(6);
    for (int value : alloc)
        cout << value << " ";
    cout << endl;

    cv.notify_all();
}

// Invariant: the system is in a safe state
bool isSafe(int threadId)
{
    cout << "\t\t" << threadId << " isSafe() enter" << endl;
    cout << "\t\t\t";
    printVector(alloc, "alloc");

    int toBeAvail = avail;    // toBeAvail is a temporary copy used to check if a thread can finish
    int threads_finished = 0; // number of threads that will eventualy finish

    vector<bool> finish; // finish[i] = true if thread i can finish
    finish.resize(T);    // false indicates the thread cannot finish, true indicates it can

    // cout << "\t\t-finish: " << finish[0] << finish[1] << finish[2] << finish[3] << finish[4] << endl;

    // calculate the need of each thread
    for (int i = 0; i < T; i++)
        need[i] = maxn[i] - alloc[i];

    if (debug)
    {
        cout << "\t\t\tneed:  " << need[0] << need[1] << need[2] << need[3] << need[4] << endl;
        cout << "\t\t\tmaxn:  " << maxn[0] << maxn[1] << maxn[2] << maxn[3] << maxn[4] << endl;
        cout << "\t\t\talloc: " << alloc[0] << alloc[1] << alloc[2] << alloc[3] << alloc[4] << endl;
        cout << "\t\t\ttoBeAvail: " << toBeAvail << endl;
    }

    while (true)
    {
        // cout << "enter" << endl;
        //  find a thread j that has not finished... but can finish because its need is less than toBeAvail
        bool found = false;
        int j;
        for (j = 0; j < T; j++)
        {
            if (!finish[j] && need[j] <= toBeAvail)
            {
                found = true;
                break;
            }
        }

        // if no thread was found that meets the above condition
        if (found == false)
        {
            // if all threads can finish then the system is in a safe state
            if (threads_finished == T)
            {
                if (debug)
                    cout << "\t\t\t" << threadId << " isSafe() exit true" << endl;
                return true;
            }
            // otherwise there exists a thread that cannot finish. system is not in a safe state
            else
            {
                if (debug)
                    cout << "\t\t\t" << threadId << " isSafe() exit false" << endl;
                return false;
            }
        }
        // else the thread will eventually finish
        else if (found == true)
        {
            finish[j] = true;
            threads_finished++;
            toBeAvail += alloc[j];
        }
    }
}

// Hypothetically grant request and see if resulting state is safe.
bool wouldBeSafe(int threadId)
{
    cout << "\t" << threadId << " wbs() enter" << endl;
    bool result = false;

    // subtarct one from available resources and add to allocated thread.
    avail -= 1;
    alloc[threadId] += 1;

    if (avail < 0)
    {
        avail += 1;
        alloc[threadId] -= 1;
        if (debug)
            cout << "\t" << threadId << " wbs() exit false" << endl;
        return false;
    }

    // check if the state "would be safe"
    if (isSafe(threadId))
        result = true;

    // return the resources to their original state
    avail++;
    alloc[threadId] -= 1;

    if (debug && result)
        cout << "\t" << threadId << " wbs() exit true" << endl;
    else if (debug && !result)
        cout << "\t" << threadId << " wbs() exit false" << endl;
    return result;
}

void initializeVectors()
{
    // all resources are initially available
    avail = R;

    // all vectors are initialized to zero
    maxn.resize(T);
    need.resize(T);
    alloc.resize(T);
    state.resize(T);
}

void readFile()
{
    ifstream inFile;
    inFile.open("requests.txt", ios::in);
    if (!inFile.is_open())
    {
        cout << "Error opening input file!" << endl;
        exit(1);
    }

    for (int i = 0; i < T; i++)
    {
        vector<int> threadVector;
        requests.push_back(threadVector);
    }

    // Read from requests.txt and write to requests vector
    vector<int> row = {-1, -1};
    while (!inFile.eof())
    {
        inFile >> row[0];
        inFile >> row[1];
        requests[row[0] - 1].push_back(row[1]);
    }

    if (row[0] == -1)
    {
        requests.pop_back();
        cout << "requests.txt was empty" << endl;
        exit(1);
    }

    inFile.close();
}

void printVector(vector<int> v, string name)
{
    cout << name << ":" << setw(6);
    for (int value : v)
        cout << value << " ";
    cout << endl;
}

void printVector(vector<vector<int>> v, string name)
{
    cout << name << ":" << endl;
    for (vector<int> row : v)
    {
        for (int value : row)
            cout << value << " ";
        cout << endl;
    }
    cout << endl;
}
