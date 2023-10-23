class ResourceMgr
{
private:
    Lock lock;
    CV cv;
    int r;         // Number of resources
    int t;         // Number of threads
    int avail[];   // avail[i]: instances of resource i available
    int max[][];   // max[i][j]: max demand of thread i for resource j
    int alloc[][]; // alloc[i][j]: current allocation of resource i to thread j

public:
    // Invariant: the system is in a safe state.
    ResourceMgr::Request(int resoourceID, int threadID)
    {
        lock.Acquire();
        assert(isSafe());
        while (!wouldBeSafe(resourceId, threadID))
            cv.Wait(&lock);

        alloc[resourceID][threadID]++;
        avail[resourceID]--;
        assert(isSafe());
        lock.Release();
    }

    // A state is safe iff there exists a safe sequence of grants that are sufficient
    // to allow all threads to eventually receive their maximum resource needs.
    bool ResourceMgr::isSafe()
    {
        int j;
        int toBeAvail[] = copy avail[];
        int need[][] = max[][] - alloc[][]; // need[i][j] is initialized to max[i][j] - alloc[i][j]

        bool finish[] = [ false, false, false, ... ]; // finish [j] is true if thread j is guaranteed to finish

        while (true)
        {
            j = any threadID such that : (finish[j] == false) && forall i : need[i][j] <= toBeAvail[i];
            if (no such j exists)
            {
                if (forall j
                    : finish[j] == true)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            { // Thread j will eventually finish and return its current location to the pool.
                finish[j] = true;
                forall i : toBeAvail[i] = toBeAvail[i] + alloc[i][j];
            }
        }
    }

    // Hypothetically grant request and see if resulting state is safe.
    bool ResourceMgr::wouldBeSafe(int resourceID, int threadID){
        bool result = false;

        avail[resourceID][threadID]++;
        if (isSafe()) {
            result = true;
        }
        avail[resourceID]++;
        alloc[resourceID][threadID]--;
        return result;
    }
}
