/*
 * Decimal.h
 *
 *  Created on: 24.10.2018
 *      Author: marlo
 */

#ifndef CCDECIMAL_H_
#define CCDECIMAL_H_

#define MAX 31

#include <stdint.h>
#include <iostream>
#include <cstdarg>
#include <string>

using namespace std;

class CCDecimal {

private:
	int8_t digit[MAX + 1];
	unsigned int used = 0;
	int shift = 0;
	bool isNegative = false;

	unsigned int precision = 0;
	unsigned int* pPrecision;
	static unsigned int defaultPrecision;

	bool lessThan(const CCDecimal& op2) const;
	void add(CCDecimal* result, const CCDecimal& op2) const;

	void constructFromString(const string& numberStr);

public:
	void sub(CCDecimal* result, const CCDecimal& op2) const;

	void mult(CCDecimal* result, const CCDecimal& op2) const;
	unsigned int getLocalPrecision() {
		return *pPrecision;
	}
	void setLocalPrecision(unsigned int p) {
		precision = p;
		pPrecision = &precision;
	}

	//constructors

	CCDecimal();
	CCDecimal(const CCDecimal& d2);
	CCDecimal(const char* str);

	CCDecimal(const string& numberStr);
	//CCDecimal(const char* str);
	CCDecimal(double number);
	virtual ~CCDecimal();

	//getter/setter
	static void setGlobalPrecision(unsigned int);
	static unsigned int getGlobalPrecision();

	//others
	CCDecimal operator +(const CCDecimal&) const;
	CCDecimal operator *(const CCDecimal&) const;
	CCDecimal& operator *=(const CCDecimal&);


	bool operator ==(const CCDecimal&) const;

	//testing only
	CCDecimal(int count, ...) {
		va_list arguments;             // A place to store the list of arguments
		va_start(arguments, count); // Initializing arguments to store all values after count
		for (int i = count - 1; i >= 0; i--) {
			int val = (int) (va_arg(arguments, int));
			setDigit(i, val);
			//cout << va_arg(arguments, int) << endl;
		}
		// Adds the next value in argument list to sum.
		va_end(arguments);
	}
	void setDigit(unsigned int pos, int8_t value) {
		digit[pos] = value;
		if (pos >= used && value != 0) {
			used = pos + 1;
		}
	}
	void setNegative(bool isNegative) {
		this->isNegative = isNegative;
	}
	void setDigits(int count, ...) {
		//cout << count << endl;

		va_list arguments;             // A place to store the list of arguments
		va_start(arguments, count); // Initializing arguments to store all values after count
		for (int i = count - 1; i >= 0; i--) {
			int val = (int) (va_arg(arguments, int));
			setDigit(i, val);
			//cout << va_arg(arguments, int) << endl;
		}
		// Adds the next value in argument list to sum.
		va_end(arguments);

	}
	void setShift(int shift) {
		this->shift = shift;
	}
	void print() const {
		for (int i = used - 1; i >= 0; i--) {

			cout << (int) digit[i];
			if (i > 0 && i == -shift) {
				cout << '.';
			}
		}
		cout << 'E';
		cout << shift;
	}
//	CCDecimal& operator =(string& str) {
//
//		shift = 0;
//		int lz = 0;
//		bool hasDp = false;
//
//		unsigned int pos = 0;
//
//		int i = str.length() - 1;
//		while (str[i] == '0') {
//			i--;
//		}
//		lz = str.length() - i - 1;
//
//		for (; i >= 0; i--) {
//
//			if (pos >= MAX) {
//				throw std::exception();
//			}
//
//			char c = str[i];
//
//			if (c >= '0' && c <= '9') {
//				digit[pos] = c - 48;
//				pos++;
//				//cout << digit[pos-1] << endl;
//				//print();
//			} else if (!hasDp && c == '.') {
//				hasDp = true;
//				shift = -pos;
//			} else {
//				//cout << "some failure: " << i << " " << endl;
//				throw std::exception();
//			}
//		}
//
//		if (hasDp == false) {
//			shift = lz;
//		}
//		//used = pos;
//
//		//remove leading zeroes
//		pos--;
//		while (pos > 0 && (digit[pos] == 0)) {
//			pos--;
//		}
//		used = pos + 1;
//
//		//cout << "used: " << used  << endl;
//		//cout << "shift: " <<  shift<< endl;
//
//		return *this;
//	}

//	CCDecimal(const char* str){
//		string s(str);
//		*this = s;
//	}

	void round(CCDecimal* pDec, unsigned int precOut) const {

		//not enough places after the decimal point
		if ((int) precOut > -pDec->shift) {
			return;
		}

		int roundIndex = -shift - (int) precOut - 1;

		//roundIndex is out of bounds (invisible zero)
		if (roundIndex < 0 || roundIndex >= (int) pDec->used) {
			return;
		}
		unsigned int validIndex = roundIndex + 1;

		if (pDec->digit[roundIndex] >= 5) {

			while (validIndex < pDec->used && pDec->digit[validIndex] >= 9) { //end condition needed?
				validIndex++;
			}
			pDec->digit[validIndex]++;
		}

		//remove trailing zeroes
		for (unsigned int i = validIndex; i <= used; i++) {
			pDec->digit[i - validIndex] = pDec->digit[i];
		}
		pDec->digit[used] = 0;
		pDec->used -= validIndex;
		pDec->shift += validIndex;
	}

	string toString() const {

		//create a copy to round without changing the original
		CCDecimal copy(*this);
		round(&copy, *pPrecision - 1);

		//catch zero case
		if (copy.used == 0) {
			return "0";
		}

		string result = "";

		//append sign
		if (isNegative) {
			result = "-";
		}

		//append leading zeroes
		int lz_end = copy.shift + (int) copy.used;
		for (int i = lz_end; i <= 0; i++) {
			result += '0';
		}

		//append digits before the decimal point
		int dp = max(-copy.shift, 0);
		for (int i = copy.used - 1; i >= dp; i--) {
			result += (char) (copy.digit[i] + 48);
		}

		//append decimal point
		if (-copy.shift > 0) {
			result += '.';
		}

		//append digits after decimal point
		for (int i = dp - 1; i >= 0; i--) {
			result += (char) (copy.digit[i] + 48);
		}

		//append trailing zeroes
		for (int i = 0; i < copy.shift; i++) {
			result += '0';
		}

		return result;
	}

};

#endif /* CCDECIMAL_H_ */
