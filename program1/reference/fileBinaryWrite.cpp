#include <iostream>
#include <fstream>
using namespace std;

int main () {
	ofstream OutFile;
	
	int my_double = 3.1415926121;
	
				OutFile.open("mybinary.txt", ios::out | ios::binary);
	
	OutFile.write( (char*)&my_double, sizeof(double));
	
	OutFile.close();
	
	return 0;
}
