// 114 Program 1
// Jason Whitlow
// 9/22/2023

// The following program reads a binary file and writes it to another file.
// The commands below are used to run the program.

// g++ -o a.out CSci114_p1.cpp
// ./a.out

#include <iostream>
#include <fstream>
using namespace std;

const int SIZE = 1024;

int main()
{
    ifstream inFile;
    ofstream outFile;

    double my_double;
    char buffer[SIZE];

    // Open data.in
    inFile.open("data.in", ios::in | ios::binary);
    if (!inFile.is_open())
    {
        cout << "Error opening input file!" << endl;
        return 1;
    }

    // Open data.out
    outFile.open("data.out", ios::out | ios::binary);
    if (!outFile.is_open())
    {
        cout << "Error opening output file!" << endl;
        inFile.close();
        return 1;
    }

    // Read from data.in and write to data.out,
    // .gcount() is used to get the number of bytes read
    cout << "writing" << endl;
    while (!inFile.eof())
    {
        inFile.read(buffer, sizeof(buffer));
        streamsize bytesRead = inFile.gcount();
        if (bytesRead > 0)
        {
            outFile.write(buffer, bytesRead);
        }
    }
    cout << "finished writing" << endl;

    inFile.close();
    outFile.close();

    return 0;
}

// https://stackoverflow.com/questions/6093224/reading-a-binary-file-1-byte-at-a-time