#include <string.h>
#include <iostream.h>
#include <stdlib.h>
#include <string>
using namespace std;

int main()
{
	string SizeE;
	int SizeA, SizeB, SizeD, SizeC;

	cout << "The Breast Measurer v2.0" << endl << "by endura29@hotmail.com" << endl;
	cout << "Enter the Measurement (in inches) just below the breast: ";
	cin >> SizeA;
	cout << "Enter the Measurement (in inches) at the Nipple: ";
	cin >> SizeB;
	//SizeD = (SizeA + 5);
	//SizeC = (SizeB - SizeD);
	SizeC = SizeB - SizeA;

	//if (SizeD % 2)
	//	SizeD++;
	if (SizeC < 0) {
		cout << "\n" << "Now, that's less than a mouthful!!" << endl;
		return 0;
	}

	switch (SizeC)
	{
		case 0:
			SizeE = "AA";
			break;
		case 1:
			SizeE = "A";
			break;
		case 2:
			SizeE = "B";
			break;
		case 3:
			SizeE = "C";
			break;
		case 4:
			SizeE = "D";
			break;
		case 5:
			SizeE = "DD";
			break;
		case 6:
			SizeE = "F";
			break;
		case 7:
			SizeE = "G";
			break;
		case 8:
			SizeE = "H";
			break;
		case 9:
			SizeE = "I";
			break;
		default:
			cout << "Damn, that's a lotta boobs!" << endl;
			return 0;
	}

	cout << "Bra Size: " << SizeA << SizeE.c_str() << endl;

	return 0;
}
