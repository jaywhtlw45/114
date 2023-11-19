// CSCI 114 Program 5
// Jason Whitlow

// !this program ASSUMES that requests are recieved in order.
// !if requests are recieved out of order, insertion and deltion will fail.

#include "header/Allocated.h"
#include "header/Memory.h"

#include <iostream>

using namespace std;

int main()
{
    int request_id = 1;
    int block_id = 1;
    int mem_size = 500;

    Allocated allocated;
    allocated.insert(request_id, block_id, mem_size);
    allocated.insert(request_id + 1, block_id, mem_size);
    allocated.insert(request_id + 2, block_id, mem_size);

    Memory memory;
    memory.print();

    return 0;
}