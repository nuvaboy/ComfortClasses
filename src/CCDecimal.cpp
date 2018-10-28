/*
 * Decimal.cpp
 *
 *  Created on: 24.10.2018
 *      Author: marlo
 */

#include "CCDecimal.h"

using namespace std;

unsigned int CCDecimal::defaultPrecision = 6;

CCDecimal::CCDecimal() {
	pPrecision = &CCDecimal::defaultPrecision;

	//initialize all digits to zero
	for (int i = 0; i <= MAX; i++) {
		digit[i] = 0;
	}
	used = 0;
	shift = 0;
}

CCDecimal::~CCDecimal() {

}

CCDecimal CCDecimal::operator +(const CCDecimal& op2) const {
	CCDecimal result;
	add(&result, op2);

	return result;
}

bool CCDecimal::operator ==(const CCDecimal& op2) const {
	if (used != op2.used || shift != op2.shift) {
		cout << "false, shift or used" << endl;
		return false;
	}

	for (unsigned int i = 0; i < used; i++) {
		if (digit[i] != op2.digit[i]) {
			cout << "Index: " << i << " op1: " << digit[i]  << " op2: " <<  op2.digit[i] << endl;
			return false;
		}
	}
	return true;
}

void CCDecimal::add(CCDecimal* result, const CCDecimal& op2) const {
	//temporary values
	int shift_delta = shift - op2.shift; //distance between decimal points of both operands
	int resultOffset1 = 0; //offset for adding operand 1 to result, zero if shift of operand 1 is less than that of operand 2
	int resultOffset2 = 0; //offset for adding operand 2 to result, zero if shift of operand 2 is less than that of operand 1
	int opOffset1 = 0; //offset for addressing operand 1
	int opOffset2 = 0; //offset for addressing operand 2
	int shift_min = op2.shift; //max precision (operand 1, operand 2)
	int size = 0; //coalesced size to hold result of the addition (without overflow and removal of trailing zeros)

	/*
	 * 1.) calculate highest precision (lowest exponent) : "shift_min"
	 * 2.) calculate distance between the decimal points of both operators : "shift_delta"
	 * 3.) apply distance as offset for the operator with the highest exponent : "sIndex1" or "sIndex2"
	 * 4.) calculate coalesced size to hold the result of the addition
	 *     (without regard to a possible overflow or the removal of trailing zeros)
	 */
	if (shift_delta < 0) { //operand 1 has highest precision (lowest exponent)

		shift_min = shift; //save highest precision
		shift_delta = -shift_delta; //save distance between decimal points (operand 1, operand 2)
		resultOffset2 = shift_delta; //apply distance as offset for operand 2
		size = max(used, shift_delta + op2.used); //calculate coalesced size

	} else { //operand 2 has highest (or equal) precision (lowest exponent)
		resultOffset1 = shift_delta; //apply distance as offset for operand 1
		size = max(op2.used, shift_delta + used); //calculate coalesced size

	}
	cout << "calculated size: " << size;

	//calculate amount of expendable digits
	int digToSpend = -precision - shift_min;
	cout << "   to spend: " << digToSpend;

	//calculate amount of digits needed to be cut to meet size requirements
	int digToCut = size - MAX; //size - 31 = digits to cut
	cout << "   to cut: " << digToCut;

	//requiered to cut
	if (digToCut > 0) {
		size = MAX;

		if (digToSpend < digToCut) { //overflow
			cout << "   overflow ";

		} else { //cut
			shift_min += digToCut;
			if (resultOffset1 == 0) { //operand 1 has highest precision
				opOffset1 = digToCut;
				opOffset2 = (digToCut > resultOffset2) ? (digToCut - resultOffset2) : 0;
				resultOffset2 = (resultOffset2 > digToCut) ? (resultOffset2 - digToCut) : 0;

			} else { //operand 2 has highest precision
				opOffset2 = digToCut;
				opOffset1 = (digToCut > resultOffset1) ? (digToCut - resultOffset1) : 0;
				resultOffset1 = (resultOffset1 > digToCut) ? (resultOffset1 - digToCut) : 0;
			}
		}
	}

	cout << "   shdelta: " << shift_delta;
	cout << "   index1: " << resultOffset1;
	cout << "   index2: " << resultOffset2;

	//add operand 1 to the result
	for (unsigned int i = resultOffset1; i < MAX && (i - resultOffset1 + opOffset1 < used); i++) {
		result->digit[i] = digit[i - resultOffset1 + opOffset1];
	}

	//add operand 2 to the result
	for (unsigned int i = resultOffset2; i < MAX && (i - resultOffset2 + opOffset2 < op2.used);
			i++) {
		result->digit[i] += op2.digit[i - resultOffset2 + opOffset2];
	}

	//forward carries
	for (int i = 0; i < size; i++) {
		if (result->digit[i] >= 10) {
			result->digit[i] -= 10;
			result->digit[i + 1]++;
		}
	}

	//end/////////////////////
	result->used = size;
	result->shift = shift_min;
	//////////////////////////

	//remove trailing zeros
	if (result->digit[0] == 0) {

		int lastZero = 1; //TODO non zero
		while (result->digit[lastZero] == 0 && lastZero < MAX) {
			lastZero++;
		}
		for (int i = lastZero; i <= MAX; i++) {
			result->digit[i - lastZero] = result->digit[i];
		}
		result->used -= (lastZero ); //- 1//overflow digit now visible, hence used = used - lastZero_+ 1_
		result->shift += lastZero;
		result->digit[MAX] = 0;
	}

	//TODO remove leading zeros in case of subtraction

	if (result->digit[MAX] > 0) {

		//overflow
		throw std::overflow_error(
				"Result is too large to store in Decimal. Keep values in range or reduce precision!");
	}

	cout << "   r: ";
	//result->print();

	cout << endl;
}

void CCDecimal::setDefaultPrecision(unsigned int precision) {
	CCDecimal::defaultPrecision = precision;
}

unsigned int CCDecimal::getDefaultPrecision() {
	return CCDecimal::defaultPrecision;
}

