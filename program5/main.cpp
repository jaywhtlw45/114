// CSCI 114 Program 5
// Jason Whitlow

// !this program ASSUMES that requests are recieved in order.
// !if requests are recieved out of order, insertion and deltion will fail.

#include "header/Allocated.h"
#include "header/Memory.h"

#include <iostream>
#include <fstream>

using namespace std;

//! NOTE TO SELF!!!!!!!!!!!!!!!! OUTPUT TO A FILE!!!!!!!!!!!!!!!!!!!!!!!!!
int main()
{
    // open file
    ifstream in_file;
    in_file.open("./requests-1.txt");

    char type; // A - allocate, R - release
    int request_id;
    int block_id;
    int mem_size;

    Allocated alloc;
    Memory memory;

    // Process all requests
    while (in_file >> type)
    {
        // allocation
        if (type == 'A')
        {
            in_file >> request_id >> mem_size;
            // cout << "request" << endl;
            // cout << type << " " << request_id << " " << mem_size << endl;

            // find an avialable memory block and allocate memory
            block_id = memory.allocate(mem_size);
            // memory.print();

            // add the memory allocation to alloc
            alloc.insert(request_id, block_id, mem_size);
            // alloc.print();

            cout << mem_size << " bytes allocated at block " << block_id << " for request " << request_id << endl;
            cout << endl;
        }
        // release
        else if (type == 'R')
        {
            in_file >> request_id;
            // cout << "request" << endl;
            //  cout << type << " " << request_id << endl
            //      << endl;

            // get mem_size from allocated list
            alloc.get_mem_size(request_id, block_id, mem_size);

            // remove request from allocated
            alloc.remove(request_id);
            // alloc.print();

            // release memory
            memory.release(block_id, mem_size);
            // memory.print();

            cout << mem_size << " bytes returned to block " << block_id << " for request " << request_id << endl;
            cout << endl;
        }
        // error
        else
        {
            cout << "cout type does not exist" << endl;
            return 1;
        }
    }

    in_file.close();

    return 0;
}
