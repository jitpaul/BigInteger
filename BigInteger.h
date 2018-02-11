#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

class BigInteger {
	std::vector<int> num;
	bool negSign;

public:

	BigInteger();

	BigInteger(long long int b);

	BigInteger(std::string b);


	/****** Addition ******/

	BigInteger operator+(BigInteger& b);

	BigInteger operator+(long long int& b);



	/****** Subtraction ******/

	BigInteger operator-(BigInteger &b);

	BigInteger operator-(long long int b);

	BigInteger subtract2(BigInteger& b);


	/****** Multiplication ******/

	BigInteger multiply(BigInteger& a, BigInteger& b);

	BigInteger operator*(BigInteger& b);

	BigInteger operator*(long long int b);


	/****** Division ******/

	std::pair<BigInteger, BigInteger> divide2(BigInteger& b);

	BigInteger operator/(BigInteger& b);

	BigInteger operator/(long long int b);


	/****** Power ******/

	BigInteger power2(BigInteger& b);

	BigInteger pow(BigInteger& b);

	BigInteger pow(long long int b);


	/****** SquareRoot ******/

	BigInteger sqrt();

	BigInteger squareRoot2(BigInteger begin, BigInteger end);


	/****** Mod ******/

	BigInteger operator%(BigInteger& b);

	BigInteger operator%(long long int b);


	/****** Helper Methods ******/

	void removeLeadingZeros(BigInteger&b);

	friend std::ostream& operator << (std::ostream& out, BigInteger &a);

	int isGreater(BigInteger &b);


};