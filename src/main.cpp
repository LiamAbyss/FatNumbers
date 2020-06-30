#include "FatNumber.h"

using namespace std;

int main()
{
	FatNumber a(794360), b(43698749);
	a *= b;
	a.print();
	cout << endl;
	system("pause");
	return EXIT_SUCCESS;
}