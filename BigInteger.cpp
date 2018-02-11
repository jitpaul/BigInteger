#include "BigInteger.h"



	BigInteger::BigInteger():negSign(false) {}


	BigInteger::BigInteger(long long int b) {
		negSign = false;
		if (b < 0) { negSign = true; b = b * -1; }
		while (b>0) {
			num.push_back(b % 10);
			b = b / 10;
		}
	}


	BigInteger::BigInteger(std::string b) {
		negSign = false;
		if (b[0]=='-') { negSign = true; b = b.substr(1); }
		int i = b.size() - 1;
		while (i>=0) num.push_back(int(b[i--]-'0'));
	}
	


	/****** Addition ******/

	BigInteger BigInteger::operator +(BigInteger& b) {
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
			BigInteger ret = b - *this;
			negSign = true;
			return ret;
		}
		else {
			b.negSign = false;
			BigInteger ret = *this-b;
			b.negSign = true;
			return ret;
		}
	}


	BigInteger BigInteger::operator +(long long int& b) {
		return *this +(BigInteger(b));
	}



	/****** Subtraction ******/

	BigInteger BigInteger::operator -(BigInteger &b) {
		if (negSign != b.negSign) {
			(b.negSign) ? b.negSign = false : b.negSign = true;
			BigInteger ret = *this + (b);
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


	BigInteger BigInteger::operator -(long long int b) {
		return *this - BigInteger(b);
	}


	BigInteger BigInteger::subtract2(BigInteger& b) {
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
	
	BigInteger BigInteger::multiply(BigInteger& a, BigInteger& b) {
		int n = a.num.size();
		if (n == 1) return BigInteger(a.num[0] * b.num[0]);
		if (n % 2 == 1) { a.num.push_back(0); b.num.push_back(0); n += 1; }

		BigInteger a1, a2, b1, b2, t1,t2,t3, ret;
		a2.num.insert(a2.num.begin(), a.num.begin(), a.num.begin()+n/2);
		a1.num.insert(a1.num.begin(), a.num.begin()+n/2, a.num.end());
		b2.num.insert(b2.num.begin(), b.num.begin(), b.num.begin() + n / 2);
		b1.num.insert(b1.num.begin(), b.num.begin()+n/2, b.num.end());
		t1 = a1*b1;
		t2 = a2*b2;
		t3 = (a1+ a2)*(b1 + b2);
		t3 = t3-t1-t2;
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
		ret = (t1 + t2 + t3);
		return ret;
	}


	BigInteger BigInteger::operator*(BigInteger& b) {
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


	BigInteger BigInteger::operator*(long long int b) {
		return (*this) * (BigInteger(b));
	}


	/****** Division ******/

	std::pair<BigInteger, BigInteger> BigInteger::divide2(BigInteger& b) {
		bool save = b.negSign;
		b.negSign = false;
		BigInteger q(0), r(0), temp = b;
		while (isGreater(temp) >= 0) {
			q = q + BigInteger(1);
			temp = temp + b;
		}
		temp = temp-b;
		b.negSign = save;
		save = negSign; 
		negSign = false;
		r = *this- temp;
		negSign = save;
		if (negSign == b.negSign) q.negSign = false;
		else q.negSign = true;
		removeLeadingZeros(q);
		removeLeadingZeros(r);
		return std::pair<BigInteger, BigInteger>(q,r);
	}


	BigInteger BigInteger::operator/(BigInteger& b) {
		std::pair<BigInteger, BigInteger> ret = divide2(b);
		return ret.first;
	}


	BigInteger BigInteger::operator/(long long int b) {
		return *this / (BigInteger(b));
	}


	/****** Power ******/

	BigInteger BigInteger::power2(BigInteger& b) {
		if (b.isGreater(BigInteger(0)) == 0) return BigInteger(1);
		BigInteger temp = (*this) * (*this);
		if((b%(BigInteger(2))).isGreater(BigInteger(1))==0) return (*this) * (temp.power2(b/(BigInteger(2))));
		else return temp.power2(b/(BigInteger(2)));
	}

	BigInteger BigInteger::pow(BigInteger& b) {
		BigInteger ret = power2(b);
		removeLeadingZeros(ret);
		removeLeadingZeros(b);
		return ret;
	}


	BigInteger BigInteger::pow(long long int b) {
		return pow(BigInteger(b));
	}


	/****** SquareRoot ******/


	BigInteger BigInteger::sqrt() {
		return squareRoot2(BigInteger(1), *this);
	}


	BigInteger BigInteger::squareRoot2(BigInteger begin, BigInteger end) {
		int c = begin.isGreater(end);
		if ( c == 0 && isGreater(begin.pow(BigInteger(2))) < 0) return begin - BigInteger(1);
		else if (c == 0) return begin;
		else if (c > 0) return end;

		BigInteger mid = (begin +end)/(BigInteger(2));
		int compare = isGreater(mid.pow(BigInteger(2)));
		if (compare == 0) return mid;
		else if (compare > 0) return squareRoot2(mid + BigInteger(1), end);
		else return squareRoot2(begin, mid - BigInteger(1));

	}

	/****** Mod ******/

	BigInteger BigInteger::operator%(BigInteger& b) {
		std::pair<BigInteger, BigInteger> ret = divide2(b);
		return ret.second;
	}


	BigInteger BigInteger::operator%(long long int b) {
		return operator%(BigInteger(b));
	}


	/****** Helper Methods ******/

	void BigInteger::removeLeadingZeros(BigInteger&b) {
		int tp = b.num.size() - 1;
		while (tp >= 0 && b.num[tp--] == 0)
		    b.num.pop_back();
	}

	int BigInteger::isGreater(BigInteger &b) {
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
