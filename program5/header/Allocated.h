// Allocated contains a list of requests that have been allocated in memory.
// block_id contains the memory block where the allocation takes place
// mem_size is the amount of memory used in the block.

// !this program ASSUMES that requests are recieved in order of positive integer values 1 2 3 4 (excluding 0).
// !requests may not skip any integers. 1 2 4 is not allowed.
// !if requests are recieved out of order, insertion and deltion will fail.

#ifndef Allocated_H
#define Allocated_H

#include <iostream>

using namespace std;

class Allocated
{
private:
    struct Node
    {
        int request_id;     // the request number. starts at 1
        int block_id;       // the block in memory that is allocated
        int mem_size;
        Node *next;
    };
    Node *head;
    int size;

public:
    Allocated();
    void insert(int request_id, int block_id, int mem_size);
    bool remove(int request_id);
    int get_mem_size(int request_id, int &block_id, int &mem_size);
    void print();
};

Allocated::Allocated()
{
    head = NULL;
    size = 0;
}

int Allocated::get_mem_size(int request_id, int &block_id, int &mem_size){
    Node* cur = head;
    while (cur != nullptr && cur->request_id != request_id)
    {
        cur = cur->next;
    }

    if (cur == nullptr)
    {
        cout << "block id does not exist" << endl;
        return -1; 
    }

    
    block_id = cur->block_id;
    mem_size = cur->mem_size;
}


void Allocated::insert(int request_id, int block_id, int mem_size)
{
    Node *new_node = new Node;
    new_node->request_id = request_id;
    new_node->block_id = block_id;
    new_node->mem_size = mem_size;

    // insert at head
    if (!head)
    {
        head = new_node;
        head->next = nullptr;
    }
    else
    {
        // find the last node
        Node *cur = head;
        while (cur->next)
        {
            cur = cur->next;
        }

        // add a node
        cur->next = new_node;
        new_node->next = nullptr;
    }
    size++;
}

bool Allocated::remove(int request_id)
{
    // no nodes in list
    if (!head)
    {
        cout << "Allocated List is empty" << endl;
        return false;
    }

    Node *cur = head;
    Node *prev = nullptr;

    // delete head node
    if (head->request_id == request_id)
    {
        head = head->next;
        size--;
        delete cur;
        return true;
    }

    // find the request
    prev = cur;
    cur = cur->next;
    while (cur != NULL && cur->request_id != request_id)
    {
        prev = cur;
        cur = cur->next;
    }

    // request was not found
    if (!cur)
    {
        cout << "Request ID does not exist" << endl;
        return false;
    }

    // delete the request
    prev->next = cur->next;
    delete cur;
    size--;
    return true;
}

void Allocated::print()
{
    if (!head)
    {
        cout << "Allocated List is empty" << endl;
    }

    cout << "allocation list" << endl;
    Node *temp = head;
    while (temp != nullptr)
    {
        cout << temp->request_id << " " << temp->block_id << " " << temp->mem_size << endl;
        temp = temp->next;
    }

}

#endif