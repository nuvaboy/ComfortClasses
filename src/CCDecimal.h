/*
 * Decimal.h
 *
 *  Created on: 24.10.2018
 *      Author: marlo
 */

#ifndef CCDECIMAL_H_
#define CCDECIMAL_H_

#define MAX 31 //31

#include <stdint.h>
#include <iostream>
#include <cstdarg>
#include <string>

using namespace std;

class CCDecimal {

private:
	int8_t digit[MAX + 1];
	uint32_t used = 0;
	//int shift = 0;
	int32_t shift = 0;
	bool isNegative = false;

	int32_t precision = 0;
	int32_t* pPrecision;
	static int32_t defaultPrecision;


	void constructFromString(string numberStr);
	bool magnitudeLessThan(const CCDecimal& op2) const;
	void add(CCDecimal* result, const CCDecimal& op2) const;
	void sub(CCDecimal* result, const CCDecimal& opSmall) const;
	void mult(CCDecimal* result, const CCDecimal& op2) const;
	void div(CCDecimal* result, const CCDecimal& op2) const;
	void mod(CCDecimal* result, const CCDecimal& op2) const;

public:


	void cfs(const string& numberStr);

	//constructors
	CCDecimal();
	CCDecimal(const CCDecimal& d2);
	CCDecimal(const char* str);
	CCDecimal(const string& numberStr);
	CCDecimal(double number);
	virtual ~CCDecimal();

	//getter/setter
	int32_t getLocalPrecision();
	void setLocalPrecision(int32_t prec);
	static void setGlobalPrecision(int32_t);
	static int32_t getGlobalPrecision();

	//utility functionality
	static void round(CCDecimal* pDec, int32_t precOut);
	string toString() const;
	string toString(int32_t precOut) const;

	//operators
	CCDecimal operator +(const CCDecimal&) const;
	CCDecimal& operator +=(const CCDecimal&);
	CCDecimal operator -(const CCDecimal&) const;
	CCDecimal& operator -=(const CCDecimal&);
	CCDecimal operator *(const CCDecimal&) const;
	CCDecimal& operator *=(const CCDecimal&);
	CCDecimal operator /(const CCDecimal&) const;
	CCDecimal& operator /=(const CCDecimal&);

	CCDecimal operator %(const CCDecimal&) const;

	bool operator ==(const CCDecimal&) const;
	bool operator !=(const CCDecimal&) const;
	bool operator < (const CCDecimal&) const;
	bool operator > (const CCDecimal&) const;

	CCDecimal& operator++();
	CCDecimal operator++(int);

	CCDecimal& operator--();
	CCDecimal operator--(int);



	//testing only
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

};

std::ostream& operator<< (std::ostream &os, const CCDecimal& dec);


#endif /* CCDECIMAL_H_ */
