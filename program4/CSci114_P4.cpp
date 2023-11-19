// Jason Whitlow
// CSci 114 - Program 4

// This program simulates the Banker's Algorithm for deadlock avoidance.
// Accepts a list of requests from requests.txt.
// Each request is a pair of integers (threadId, resourceId).
// The program will process the requests concurrently using threads, while avoiding deadlock.

// Program assumes the correct number of threads are entered as a command line argument.
// Also assumes the correct number of resources are entered as a command line argument

// Run Program:
// g++ -o a.out CSci114_P4.cpp;
// ./a.out T R; where T and R are the number of threads and number of resources.

#include <iostream>
#include <thread>
#include <vector>
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
mutex mt;              // mutex
condition_variable cv; // condition variable

int T;     // Number of threads
int R;     // Number of resources
int avail; // instances of resource i available

vector<int> maxn;             // maximum need of resource i for thread j, this is loaded from request vector
vector<int> alloc;            // allocated resource i to thread j
vector<int> need;             // need of resource i for thread j
vector<int> state;            // state of thread j, 0 = running, 1 = blocked, 2 = terminated
vector<vector<int>> requests; // stores the requests from the input file

// processRequests() is the main function for each thread, it will continue until the request vector is empty
void processRequests(int threadId);  // starts each thread, continue until request vector is empty
void getNextRequest(int threadId);   // get the next request from the request vector
void releaseResources(int threadId); // release resources allocated to thread j

// Banker's Algorithm functions
bool isSafe(int threadId);      // checks the state of the system
void request(int threadId);     // request resource i for thread j
bool wouldBeSafe(int threadId); // if request is granted, would the system be in a safe state?

// helper functions
void initializeVectors(); // initialize vectors to zero
void readFile();
void printVector(vector<int> v, string name);
void printVector(vector<vector<int>> v, string name);

int main(int argc, char *argv[])
{
    // Accept command line arguments
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " <R> <T>" << endl;
        return 1;
    }
    const char *arg1 = argv[1];
    const char *arg2 = argv[2];
    T = atoi(arg1);
    R = atoi(arg2);

    // read from file and initialize vectors
    readFile();
    initializeVectors();

    // Create threads
    thread th[T];
    for (int i = 0; i < T; i++)
        th[i] = thread(processRequests, i);

    // Join threads
    for (int i = 0; i < T; i++)
        th[i].join();

    return 0;
}

// the main function for each thread. continue until the request vector is empty
void processRequests(int threadId)
{
    while (!requests[threadId].empty())
    {
        getNextRequest(threadId);
        while (alloc[threadId] < maxn[threadId])
        {
            request(threadId);
            sleep(.1);
        }
        releaseResources(threadId);
    }
}

// get the next request from the request vector
void getNextRequest(int threadId)
{
    int req = requests[threadId].front();
    maxn[threadId] = req;
    requests[threadId].erase(requests[threadId].begin());
}

void releaseResources(int threadId)
{
    unique_lock<mutex> mlock(mt);
    avail += alloc[threadId];
    alloc[threadId] = 0;
    maxn[threadId] = 0;

    printVector(alloc, "");

    cv.notify_all();
}

// A state is safe if there exists a safe sequence of grants that are sufficient
// to allow all threads to eventually receive their maximum resource needs.
void request(int threadId)
{
    unique_lock<mutex> mlock(mt);

    // Grant request if safe, otherwise wait
    assert(isSafe(threadId));

    while (!wouldBeSafe(threadId))
        cv.wait(mlock);

    // Grant request
    alloc[threadId] += 1;
    avail -= 1;
    assert(isSafe(threadId));
    printVector(alloc, "");

    cv.notify_all();
}

// checks if the system is in a safe state
bool isSafe(int threadId)
{
    int toBeAvail = avail;    // toBeAvail is a temporary copy used to check if a thread can finish
    int threads_finished = 0; // number of threads that will eventualy finish

    vector<bool> finish; // finish[i] = true if thread i can finish
    finish.resize(T);    // false indicates the thread cannot finish, true indicates it can

    // calculate need for each thread
    for (int i = 0; i < T; i++)
        need[i] = maxn[i] - alloc[i];

    while (true)
    {
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
                return true;
            // otherwise there exists a thread that cannot finish. system is not in a safe state
            else
                return false;
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

// determines if the system would be safe given one resource is granted to threadId
bool wouldBeSafe(int threadId)
{
    bool result = false;

    // subtract one from available resources and add one to allocated thread.
    avail -= 1;
    alloc[threadId] += 1;

    if (avail < 0)
    {
        avail += 1;
        alloc[threadId] -= 1;

        return false;
    }

    // check if the state "would be safe"
    if (isSafe(threadId))
        result = true;

    // return the resources to their original state
    avail++;
    alloc[threadId] -= 1;
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
        cout << "requests.txt is empty" << endl;
        exit(1);
    }

    inFile.close();
}

void printVector(vector<int> v, string name)
{
    cout << name << "[";
    for (int value : v)
        cout << value << " ";
    cout << "]" << endl
         << endl;
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
