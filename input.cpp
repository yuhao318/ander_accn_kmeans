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
	srand((unsigned)time(NULL));  
	for (i = 0; i <2^10 ; i++) {
			input =rand();
			if(input>=50)input =input %50;
			InputFile << input;
			InputFile << endl;
		}
		

    return 0;
}

