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

public:
    Memory();
    void insert(int request_id, int block_id, int mem_size);
    bool remove(int request_id);
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

// void Memory::insert(int request_id, int block_id, int mem_size)
// {
//     Node *new_node = new Node;
//     new_node-> = request_id;
//     new_node->block_id = block_id;
//     new_node->mem_size = mem_size;

//     // insert at head
//     if (!head)
//     {
//         head = new_node;
//         head->next = nullptr;
//     }
//     else
//     {
//         // find the last node
//         Node *cur = head;
//         while (cur->next)
//         {
//             cur = cur->next;
//         }

//         // add a node
//         cur->next = new_node;
//         new_node->next = nullptr;
//     }
//     size++;
// }

// bool Memory::remove(int request_id)
// {
//     // no nodes in list
//     if (!head)
//     {
//         cout << "Allocated List is empty" << endl;
//         return false;
//     }

//     Node *cur = head;
//     Node *prev = nullptr;

//     // delete head node
//     if (head->request_id == request_id)
//     {
//         head = head->next;
//         size--;
//         delete cur;
//         return true;
//     }

//     // find the request
//     prev = cur;
//     cur = cur->next;
//     while (cur != NULL && cur->request_id != request_id)
//     {
//         prev = cur;
//         cur = cur->next;
//     }

//     // request was not found
//     if (!cur)
//     {
//         cout << "Request ID does not exist" << endl;
//         return false;
//     }

//     // delete the request
//     prev->next = cur->next;
//     delete cur;
//     size--;
//     return true;
// }

void Memory::print()
{
    if (!head)
    {
        cout << "Allocated List is empty" << endl;
    }

    Node *cur = head;
    while (cur != nullptr)
    {
        cout << cur->block_id << " " << cur->available_memory << endl;
        cur = cur->next;
    }
}

#endif