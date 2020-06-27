#include "FatNumber.h"

using namespace std;

int main()
{
	FatNumber a(1400600003), b(20005000009);
	a += b;
	a.print();
	cout << endl;
	system("pause");
	return EXIT_SUCCESS;
}