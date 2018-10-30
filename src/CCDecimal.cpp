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
			//cout << "Index: " << i << " op1: " << digit[i] << " op2: " << op2.digit[i] << endl;
			return false;
		}
	}
	return true;
}

void CCDecimal::add(CCDecimal* result, const CCDecimal& op2) const {

	//determine the most and the least precise decimal
	const CCDecimal* pMostPrec = this;
	const CCDecimal* pLeastPrec = &op2;
	if (op2.shift < shift) {
		pMostPrec = &op2;
		pLeastPrec = this;
	}

	int shift_min = pMostPrec->shift;
	int shift_delta = pLeastPrec->shift - pMostPrec->shift;
	int size = max(pMostPrec->used, shift_delta + pLeastPrec->used); //calculate coalesced size

	//calculate amount of expendable digits
	int digToSpend = -precision - shift_min;

	//calculate amount of digits needed to be cut to meet size requirements
	int digToCut = size - MAX; //size - 31 = digits to cut

	//requiered to cut
	int opOffsetMost = 0;
	int opOffsetLeast = 0;
	if (digToCut > 0) {
		size = MAX;

		if (digToSpend < digToCut) { //overflow
			throw std::overflow_error(
					"Result is too large to store in Decimal. Keep values in range or reduce precision!");

		} else { //cut
			shift_min += digToCut;
			opOffsetMost = digToCut;

			if (digToCut > shift_delta) {
				opOffsetLeast = digToCut - shift_delta;

			} else if (digToCut < shift_delta) {
				shift_delta -= - digToCut;
			}
		}
	}

	//add operand 1 to the result
	for (unsigned int i = 0; i + opOffsetMost < used; i++) {
		result->digit[i] = digit[i + opOffsetMost];
	}

	//add operand 2 to the result
	for (unsigned int i = 0; i + opOffsetLeast < op2.used; i++) {
		result->digit[i + shift_delta] += op2.digit[i + opOffsetLeast];
	}

	//forward carries
	for (int i = 0; i < size; i++) {
		if (result->digit[i] >= 10) {
			result->digit[i] -= 10;
			result->digit[i + 1]++;
		}
	}

	result->used = size;
	result->shift = shift_min;

	//remove trailing zeros
	if (result->digit[0] == 0) {

		int nonZero = 1; //TODO non zero
		while (result->digit[nonZero] == 0 && nonZero < MAX) {
			nonZero++;
		}
		for (int i = nonZero; i <= MAX; i++) {
			result->digit[i - nonZero] = result->digit[i];
		}
		result->used -= nonZero;
		result->shift += nonZero;
		result->digit[MAX] = 0;
	}

	if (result->digit[MAX] > 0) {

		//overflow
		throw std::overflow_error(
				"Result is too large to store in Decimal. Keep values in range or reduce precision!");
	}

}

void CCDecimal::setDefaultPrecision(unsigned int precision) {
	CCDecimal::defaultPrecision = precision;
}

unsigned int CCDecimal::getDefaultPrecision() {
	return CCDecimal::defaultPrecision;
}

