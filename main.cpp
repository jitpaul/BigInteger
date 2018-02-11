#include "BigInteger.h"


int main() {
	BigInteger a("12331321");
	BigInteger b = ("32343");
	BigInteger c = a+a*b;
	std::cout << c;
	std::cin.get();
	return 0;
}