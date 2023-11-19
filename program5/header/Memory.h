// Memory contains a linked list of 1024 memory block Nodes.
// Each Node represents 1024 bytes of memory.
// As new memory is allocated, we check if a node has enough memory to support the allocation.
// If the current node does not have enough memory, check the next node.
// Memory is not split accross nodes. An allocation of 1025 bytes will fail.

// Memory always stores the smallest available memory block at the front of the list.

// !this program ASSUMES that requests are recieved in order.
// !if requests are recieved out of order, insertion and deltion will fail.

#ifndef Memory_H
#define Memory_H

#include <iostream>
#include <cstdlib>

using namespace std;

class Memory
{
private:
    struct Node
    {

        int block_id;         // block_id refers to the specific memory block.
        int available_memory; // the amount of memory not used in the block
        Node *next;
    };

    Node *head;             // when initailzed first block starts at 1
    int memory_blocks = 10; // number of memory blocks
    int bytes = 1024;       // bytes per memory block

    void insert(Node *node); // inserts a node into the list

public:
    Memory();

    int allocate(int mem_size);
    bool deallocate(int block_id, int mem_size);

    void print();
};

Memory::Memory()
{
    // create first Node
    Node *new_node = new Node;
    new_node->block_id = 1;
    new_node->available_memory = bytes;
    new_node->next = NULL;

    head = new_node;

    // create all other nodes
    Node *cur = head;
    for (int i = 2; i < memory_blocks + 1; i++)
    {
        // create new node
        new_node = new Node;
        new_node->block_id = i;
        new_node->available_memory = bytes;

        // insert node at back of list
        cur->next = new_node;
        cur = cur->next;
    }
    cur->next = nullptr;
}

// allocates memory to a memory block. returns the memory block id.
// if memory cannot fit the allocation request, return -1.
int Memory::allocate(int mem_size)
{
    if (mem_size > bytes)
    {
        cout << "Cannot assign more than " << bytes << " bytes" << endl;
        return -1;
    }

    Node *cur = head;
    Node *prev = nullptr;

    // find a memory block for allocation
    while (cur != nullptr && cur->available_memory < mem_size)
    {
        prev = cur;
        cur = cur->next;
    }

    // all memory blocks are too small
    if (cur == nullptr)
    {
        cout << "Request does not fit" << endl;
        return -1;
    }

    // allocate memory
    cur->available_memory -= mem_size;

    // sort list if not in ascending order
    if (prev != nullptr && cur->available_memory < prev->available_memory)
    {
        // remove cur from list
        prev->next = cur->next;

        // insert cur back into list
        insert(cur);
    }

    return cur->block_id;
}

void Memory::insert(Node* node)
{
    Node* cur = head;
    Node* prev = nullptr;

    while (cur != nullptr && cur->available_memory < node->available_memory)
    {
        prev = cur;
        cur = cur->next;
    }

    // head node
    if (prev == nullptr)
    {
        head = node;
        node->next = cur;
    } else 
    {
        prev->next = node;
        node->next = cur;
    }
}

bool Memory::deallocate(int block_id, int mem_size)
{
    Node* prev = nullptr;
    Node* cur = head;

    // find the memory block
    while(cur->block_id != block_id)
    {
        prev = cur;
        cur = cur->next;
    }

    if (cur == nullptr)
    {
        cerr << "Error in Memory.h" << endl;
        exit(1);
    }

    // check if deallocation is valid
    if (cur->available_memory + mem_size > bytes)
    {
        cout << "block " << cur->block_id << " has " << cur->available_memory << " bytes unallocated" << endl;
        cout << "cannot deallocate any more bytes" << endl;
        return -1;
    }

    // deallocate memory
    cur->available_memory += mem_size;

    // head
    if ((cur == head) && (cur->next != nullptr) && (cur->available_memory > cur->available_memory))
    {
        head = head->next;
        cur->next = head->next;
        head->next = cur;

        prev = head;
    }

    // sort list
    while(cur->next != nullptr && cur->available_memory > cur->next->available_memory)
    {
        prev->next = cur->next;
        prev = prev->next;

        cur = prev->next; 
        prev->next = cur;
    }
    return 1;
}

void Memory::print()
{
    if (!head)
    {
        cout << "Memory List is empty" << endl;
    }

    Node *cur = head;
    while (cur != nullptr)
    {
        cout << cur->block_id << " " << cur->available_memory << endl;
        cur = cur->next;
    }
}

#endif