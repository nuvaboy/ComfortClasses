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
	int precision = 2;


	unsigned int* pPrecision;
	static unsigned int defaultPrecision;

	void add(CCDecimal* result, const CCDecimal&) const;

public:

	CCDecimal();
	CCDecimal(std::string number);
	CCDecimal(double number);
	virtual ~CCDecimal();

	CCDecimal operator +(const CCDecimal&) const;
	bool operator ==(const CCDecimal&) const;

	static void setDefaultPrecision(unsigned int);
	static unsigned int getDefaultPrecision();

	void setDigit(unsigned int pos, int8_t value) {
		digit[pos] = value;
		if (pos >= used && value != 0) {
			used = pos + 1;
		}
	}
	void setNegative(bool isNegative){
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

	CCDecimal& operator = (string& str) {

		shift = 0;
		int lz = 0;
		bool hasDp = false;

		unsigned int pos = 0;

		int i = str.length() -1;
		while (str[i] == '0') {
			i--;
		}
		lz = str.length() - i -1;

		for (; i >= 0; i--) {

			if (pos >= MAX) {
				throw std::exception();
			}

			char c = str[i];

			if (c >= '0' && c <= '9') {
				digit[pos] = c - 48;
				pos++;
				//cout << digit[pos-1] << endl;
				//print();
			} else if (!hasDp && c == '.') {
				hasDp = true;
				shift = -pos;
			} else {
				//cout << "some failure: " << i << " " << endl;
				throw std::exception();
			}
		}

		if (hasDp == false) {
			shift = lz;
		}
		//used = pos;

		//remove leading zeroes
		pos--;
		while (pos > 0 && (digit[pos] == 0)){
			pos--;
		}
		used = pos + 1;


		//cout << "used: " << used  << endl;
		//cout << "shift: " <<  shift<< endl;

		return *this;
	}

	/*CCDecimal(const string& str) : CCDecimal(){
		*this = str;
	}*/
	CCDecimal(const char* str) : CCDecimal(){
		string s(str);
		*this = s;
	}



//	CCDecimal(const string& str){
//		*this = str;
//	}
//
//	CCDecimal(const char* str){
//		string s = str;
//		*this = s;
//	}

	string toString() const{

		if (used == 0){
			return "0";
		}

		string result = "";

		if (isNegative){
			result ="-";
		}

		//shift = -3 =>          3 2 1 0 -1 -2 -3
		//5.3

		int b = shift+(int)used;
		if (b <= 0){
			result += "0.";
			for (int i = b; i <= -1; i++){
				result += '0';
			}
		}

		for (int i = used-1; i>=0;i--){
			result += (char)(digit[i]+48);
			if (i==-shift && i != 0){
				result += '.';
			}
		}

		for (int i = 0; i < shift; i++){
			result += '0';
		}


		return result;
	}



};

#endif /* CCDECIMAL_H_ */
