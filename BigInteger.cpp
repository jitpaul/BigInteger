#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

class BigInteger {
	std::vector<int> num;
	bool negSign;

public:


	BigInteger():negSign(false) {}


	BigInteger(long long int b) {
		negSign = false;
		if (b < 0) { negSign = true; b = b * -1; }
		while (b>0) {
			num.push_back(b % 10);
			b = b / 10;
		}
	}


	BigInteger(std::string b) {
		negSign = false;
		if (b[0]=='-') { negSign = true; b = b.substr(1); }
		int i = b.size() - 1;
		while (i>=0) num.push_back(int(b[i--]-'0'));
	}
	


	/****** Addition ******/

	BigInteger add(BigInteger& b) {
		BigInteger ret;
		if (negSign == b.negSign) {
			ret.negSign = negSign;
			int carry = 0, l = 0, r = 0;
			while (l < num.size() && r < b.num.size()) {
				ret.num.push_back((num[l] + b.num[r] + carry) % 10);
				carry = (num[l++] + b.num[r++] + carry) / 10;
			}
			while (l < num.size()) {
				ret.num.push_back((num[l] + carry) % 10);
				carry = (num[l++] + carry) / 10;
			}
			while (r < b.num.size()) {
				ret.num.push_back((b.num[r] + carry) % 10);
				carry = (b.num[r++] + carry) / 10;
			}
			if (carry > 0) ret.num.push_back(carry);
			return ret;
		}
		else if (negSign) {
			negSign = false;
			BigInteger ret = b.subtract(*this);
			negSign = true;
			return ret;
		}
		else {
			b.negSign = false;
			BigInteger ret = subtract(b);
			b.negSign = true;
			return ret;
		}
	}


	BigInteger add(long long int& b) {
		return add(BigInteger(b));
	}



	/****** Subtraction ******/

	BigInteger subtract(BigInteger &b) {
		if (negSign != b.negSign) {
			(b.negSign) ? b.negSign = false : b.negSign = true;
			BigInteger ret = add(b);
			ret.negSign = negSign;
			(b.negSign) ? b.negSign = false : b.negSign = true;
			return ret;
		}
		int check = isGreater(b);
		if (check > 0) return subtract2(b);
		else if (check < 0) {
			BigInteger ret = b.subtract2(*this);
			if (negSign && b.negSign) ret.negSign = false;
			else ret.negSign = true;
			return ret;
		}
		else return BigInteger(0);
	}


	BigInteger subtract(long long int b) {
		return subtract(BigInteger(b));
	}


	BigInteger subtract2(BigInteger& b) {
		BigInteger ret;
		int carry = 0, l = 0, r = 0, diff;
		while (l< num.size() && r< b.num.size()) {
			diff = num[l] - b.num[r] - carry;
			(diff < 0) ? (carry = 1, diff += 10) : carry = 0;
			ret.num.push_back(diff);
			l++; r++;
		}
		while (l < num.size()) {
			diff = num[l] - carry;
			diff < 0 ? carry = 1, diff += 10 : carry = 0;
			ret.num.push_back(diff);
			l++;
		}
		int tp = ret.num.size() - 1;
		while (tp >= 0 && ret.num[tp--] == 0)
			ret.num.pop_back();
		ret.negSign = negSign;
		return ret;
	}


	/****** Multiplication ******/
	
	BigInteger multiply(BigInteger& a, BigInteger& b) {
		int n = a.num.size();
		if (n == 1) return BigInteger(a.num[0] * b.num[0]);
		if (n % 2 == 1) { a.num.push_back(0); b.num.push_back(0); n += 1; }

		BigInteger a1, a2, b1, b2, t1,t2,t3, ret;
		a2.num.insert(a2.num.begin(), a.num.begin(), a.num.begin()+n/2);
		a1.num.insert(a1.num.begin(), a.num.begin()+n/2, a.num.end());
		b2.num.insert(b2.num.begin(), b.num.begin(), b.num.begin() + n / 2);
		b1.num.insert(b1.num.begin(), b.num.begin()+n/2, b.num.end());
		t1 = a1.multiply(b1);
		t2 = a2.multiply(b2);
		t3 = (a1.add(a2)).multiply(b1.add(b2));
		t3 = (t3.subtract(t1)).subtract(t2);
		for (int i = 0; i < n / 2; i++) {
			reverse(t3.num.begin(), t3.num.end());
			t3.num.push_back(0);
			reverse(t3.num.begin(), t3.num.end());
		}
		for (int i = 0; i < n; i++) {
			reverse(t1.num.begin(), t1.num.end());
			t1.num.push_back(0);
			reverse(t1.num.begin(), t1.num.end());
		}
		ret = (t1.add(t2)).add(t3);
		return ret;
	}


	BigInteger multiply(BigInteger& b) {
		int diff = 0;
		if (num.size() < b.num.size()) {
			diff = b.num.size() - num.size();
			while (diff-- > 0) num.push_back(0);
		}
		else if (num.size() > b.num.size()) {
			diff = num.size() - b.num.size();
			while (diff-- > 0) b.num.push_back(0);
		}
		BigInteger ret = multiply(*this,b);
		removeLeadingZeros(ret);
		if (negSign == b.negSign) ret.negSign = false;
		else ret.negSign = true;
		return ret;
	}


	BigInteger multiply(long long int b) {
		return multiply(BigInteger(b));
	}


	/****** Division ******/


	std::pair<BigInteger, BigInteger> divide2(BigInteger& b) {		
		bool save = b.negSign;
		b.negSign = false;
		BigInteger q(0), r(0), temp = b;
		while (isGreater(temp) >= 0) {
			q = q.add(BigInteger(1));
			temp = temp.add(b);
		}
		temp = temp.subtract(b);
		b.negSign = save;
		save = negSign; 
		negSign = false;
		r = subtract(temp);
		negSign = save;
		if (negSign == b.negSign) q.negSign = false;
		else q.negSign = true;
		removeLeadingZeros(q);
		removeLeadingZeros(r);
		return std::pair<BigInteger, BigInteger>(q,r);
	}


	BigInteger divide(BigInteger& b) {
		std::pair<BigInteger, BigInteger> ret = divide2(b);
		return ret.first;
	}


	BigInteger& divide(long long int b) {
		return divide(BigInteger(b));
	}


	/****** Power ******/


	BigInteger power2(BigInteger& b) {
		if (b.isGreater(BigInteger(0)) == 0) return BigInteger(1);
		BigInteger temp = multiply(*this);
		if((b.mod(BigInteger(2))).isGreater(BigInteger(1))==0) return multiply(temp.power2(b.divide(BigInteger(2))));
		else return temp.power2(b.divide(BigInteger(2)));
	}

	BigInteger power(BigInteger& b) {
		BigInteger ret = power2(b);
		removeLeadingZeros(ret);
		removeLeadingZeros(b);
		return ret;
	}


	BigInteger power(long long int b) {
		return power(BigInteger(b));
	}


	/****** SquareRoot ******/


	BigInteger squareRoot() {
		return squareRoot2(BigInteger(1), *this);
	}


	BigInteger squareRoot2(BigInteger begin, BigInteger end) {
		int c = begin.isGreater(end);
		if ( c == 0 && isGreater(begin.power(BigInteger(2))) < 0) return begin.subtract(BigInteger(1));
		else if (c == 0) return begin;
		else if (c > 0) return end;

		BigInteger mid = (begin.add(end)).divide(BigInteger(2));
		int compare = isGreater(mid.power(BigInteger(2)));
		if (compare == 0) return mid;
		else if (compare > 0) return squareRoot2(mid.add(BigInteger(1)), end);
		else return squareRoot2(begin, mid.subtract(BigInteger(1)));

	}

	/****** Mod ******/

	BigInteger mod(BigInteger& b) {
		std::pair<BigInteger, BigInteger> ret = divide2(b);
		return ret.second;
	}


	BigInteger& mod(long long int b) {
		return mod(BigInteger(b));
	}


	/****** Helper Methods ******/

	void removeLeadingZeros(BigInteger&b) {
		int tp = b.num.size() - 1;
		while (tp >= 0 && b.num[tp--] == 0)
		    b.num.pop_back();
	}

	friend std::ostream& operator << (std::ostream& out, BigInteger &a);


	int isGreater(BigInteger &b) {
		if (num.size() == b.num.size()) {
			int l = num.size() - 1;
			while (l >= 0 && num[l] == b.num[l]) l--;
			if (l < 0) return 0;
			else if (num[l] > b.num[l]) return 1;
			else return -1;
		}
		else if (num.size() > b.num.size()) return 1;
		else return -1;
	}


};

std::ostream& operator << (std::ostream& out, BigInteger &a) {
	std::string ret = "";
	for (int tp : a.num) {
		std::string temp = std::to_string(tp);
		ret = temp.append(ret);
	}
	if (ret.size() == 0) ret = "0";
	else if (a.negSign) ret = "-" + ret;
	out << ret;
	return out;
}



int main() {
	for (int i = 1; i <= 100; i++) {
		BigInteger a(i);
		BigInteger c = a.squareRoot();
		std::cout <<i<<"-> "<< c << std::endl;
	}
	std::cin.get();
	return 0;
}