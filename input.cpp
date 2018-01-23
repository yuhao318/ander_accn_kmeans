#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<sys/time.h>
#include<cmath>
#include<stdlib.h>
using namespace std;

int main()
{
	ofstream InputFile;
	InputFile.open("test", ios::trunc);
	int start = 0;
	int input;
	int i;
	int j;
	int random1, random2;
	double x;
	srand((unsigned)time(NULL));  
	for (i = 0; i<1000000 ; i++) {
			input =rand();
			x =(input %10000)/10000.0;
			x = x +50;
			InputFile << x<<' ';
			
			input =rand();
			x =(input %10000)/10000.0;
			x = x +50;
			InputFile << x;
			
			InputFile << endl;
		}
    return 0;
}

