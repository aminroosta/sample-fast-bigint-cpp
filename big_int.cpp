#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;

template <typename T> struct big_int; /* forward declaration */

template<typename T>
struct binary_wrapper { T const * value; };

template<typename T>
binary_wrapper<T> binary(T const & v) {
	binary_wrapper<T> bv;
	bv.value = &v;
	return bv;
}

/* print T in binary format */
template<typename T>
ostream& operator<<(ostream& os, binary_wrapper<T> bv) {
	vector<char> chars;
	int shift = sizeof(T) * 8 / 2;
	T value = *(bv.value);
	for (int i = 0; i < shift; ++i) {
		chars.push_back((value % 2) + '0');
		value /= 2;
	}
	for (int i = chars.size() - 1; i >= 0; --i)
		os << chars[i];
	return os;
}

template<typename T>
ostream& operator<<(ostream& os, binary_wrapper<big_int<T> > bi_warpper) {
	const big_int<T>* bi = bi_warpper.value;
	for (int i = bi->arr.size() - 1; i >= 0; --i)
		os << binary(bi->arr[i]);
	return os;
}

template <typename T>
struct big_int {
	vector<T> arr;
	static const int shift = sizeof(T)*8/2;
	big_int(T initial = 0) {
		arr.push_back(0);
		*this += initial;
	}
	big_int(const big_int<T>& initial) {
		arr = initial.arr;
	}

	static T upper(T v) {
		return v >> shift;
	}
	static T& clean_upper(T& v) {
		return v &= (1 << shift) - 1;
	}

	/* add a number to big_int */
	big_int<T>& operator +=(T rhs) {
		T carry = rhs;
		int i;
		for (i = 0; i < arr.size() && carry > 0; ++i) {
			arr[i] += carry;
			carry = upper(arr[i]);
			clean_upper(arr[i]);
		}
		if (i == arr.size() && carry > 0)
			arr.push_back(carry);
		return *this;
	}

	big_int<T>& operator=(const big_int<T>& rhs) {
		if (this == *rhs) return *this;
		arr = rhs.arr(); // copy the other array
		return *this;
	}
	big_int<T>& operator+=(const big_int<T>& rhs) {
		T carry = 0;
		int till = arr.size() > rhs.arr.size() ? arr.size() : rhs.arr.size();
		for (int i = 0; i < till; i++) {
			T elem = carry + (i < arr.size() ? arr[i] : 0) + (i < rhs.arr.size() ? rhs.arr[i] : 0);
			carry = upper(elem);
			clean_upper(elem);
			if ( i < arr.size()) arr[i] = elem;
			else arr.push_back(elem);
		}
		if (carry > 0) arr.push_back(carry);
		return *this;
	}
};

template<typename T>
big_int<T> operator+(const big_int<T>& A, const big_int<T>& B) {
	big_int<T> ret = A;
	ret += B;
	return ret;
}

/* adds a string of numbers to another string of numbers*/
string add(const string& A, const string& B) {
	vector<char> digits;
	int Asize = A.size(), Bsize = B.size();
	int i = 0, till = Asize > Bsize ? Asize : Bsize;
	int carry = 0;
	for (i = 1; i <= till; ++i) {
		carry += (i <= Asize ? (A[Asize - i] - '0') : 0) + (i <= Bsize ? (B[Bsize - i] - '0') : 0);
		digits.push_back(carry%10 + '0');
		carry /= 10;
	}
	if (carry) digits.push_back(carry + '0');
	return string(digits.rbegin(), digits.rend());
}

/* returns the string representation of 2^pw */
const string& power_two(int pw) {
	static vector<string> powers; // = {"1", "2", "4", "8", "16", "32", "64", "128", "256", ... };
	static bool initialized = false;
	if (!initialized) {
		powers.push_back("1");
		for (int i = 1; i < 100; ++i)
			powers.push_back(add(powers.back(), powers.back()));
		initialized = true;
	}
	return powers[pw];
}

template<typename T>
ostream& operator <<(ostream& os, const big_int<T> & bi) {
	string res = "0";
	int digit_pos = 0, till = bi.arr.size() * bi.shift;
	for (digit_pos = 0; digit_pos < till; ++digit_pos) {
		if (bi.arr[digit_pos / bi.shift] & (1ULL << (digit_pos%bi.shift))) {
			res = add(res, power_two(digit_pos));
			//cout << power_two[digit_pos] << ',';
		}
	}
	os << res;
	return os;
}

void test_one() {
	big_int<uint8> bi = 0;
	uint32 sum = 0;
	for (int i = 1; i <= 10; ++i) {
		bi += (uint8)i;
		sum += i;
		cout << i << " => sum=" << sum << "   big_int=" << binary(bi) << "   sum=" << binary(sum) << endl;
		cout << bi << endl;

	}
}

void test_two() {
	big_int<uint8> bi = 150;
	for (int i = 0; i < 250; ++i)
		cout << (bi + big_int<uint8>(i)) << endl;
}

int main() {
	
	
	return 0;
}