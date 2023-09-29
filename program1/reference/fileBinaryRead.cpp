#include <iostream>
#include <fstream>
using namespace std;

int main () {
	ifstream inFile;
	
	double my_double;
	
	inFile.open("mybinary.txt", ios::in | ios::binary);
	
	inFile.read( (char*)&my_double, sizeof(double));
	
	cout << my_double << endl;
	
	inFile.close();
	
	return 0;
}
