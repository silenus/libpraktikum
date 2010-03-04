#include "../utils.h"
#include <iostream>

int main() {
	double a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1000};
	utils::transform(a, sizeof(a)/sizeof(double), "log(x)");
	for (unsigned int i = 0; i < sizeof(a)/sizeof(double); i++)
		cout << a[i] << endl;
	return 0;
}
