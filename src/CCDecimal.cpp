/*
 * Decimal.cpp
 *
 *  Created on: 24.10.2018
 *      Author: marlo
 */

#include <string>
#include <sstream>
#include <iomanip>
#include <limits>

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

CCDecimal::CCDecimal(std::string number) :
		CCDecimal() {
//	pPrecision = &CCDecimal::defaultPrecision;
//	shift = 0;
//	used = 0;

	int begin = 0;
	int end = 0;
	bool foundValid = false;

	while (!foundValid) {
		begin = number.find_first_of("-+.0987654321", end);
		end = number.find_first_not_of("-+.0987654321", begin);

		std::string numCandidate = number.substr(begin, end - 1);

		/*
		 * validate: max one sign; max one point; sign at the beginning,
		 * then digits, then point, if point, then more digits
		 * uses state-machine-ish behaviour, result valid if in digits_* state.
		 */
		enum ValidatorStates {
			error = -1,
			start,
			found_negative,
			found_positive,
			digits_after_sign,
			found_point,
			digits_after_point
		} validatorState;
		auto it = numCandidate.begin();
		while (it != numCandidate.end()) {
			switch (validatorState) {
			case start:
				validatorState =
						(*it == '-') ?
								(found_negative) :
								((*it == '+') ?
										(found_positive) :
										((*it == '.') ?
												(found_point) :
												((('0' <= *it && *it <= '9') ?
														(digits_after_sign) :
														(error)))));
				/*;*/
				break;
			case found_negative:
				//TODO set sign to negative
				negative = true;
				/* no break */
			case found_positive:
				//next state
				it++;
				validatorState = (it == numCandidate.end()) ? // in case number has only a sign
						error : digits_after_sign;
				break;
			case digits_after_sign:
				//TODO tracking shift goes here
				used++;

				//next state
				it++;
				validatorState =
						(*it == '.') ?
								(found_point) :
								(('0' <= *it && *it <= '9') ?
										(validatorState) : (error));
				break;
			case found_point:
				//TODO change tracking of shift
				//TODO "X." has to be allowed

				//next state
				it++;
				validatorState =
						(it == numCandidate.end()
								&& (it - 1) == numCandidate.begin()) ? // in case number has only a point
						error : digits_after_point;
				break;
			case digits_after_point:
				//TODO tracking shift goes here
				shift--;
				used++;
				//next state
				it++;
				validatorState =
						('0' <= *it && *it <= '9') ? (validatorState) : (error);
				break;
			case error:
				it++;
				break;
			default:
				std::cout << "FSM in default. This should not happen."
						<< std::endl;
			}
			if (validatorState == digits_after_point
					|| validatorState == digits_after_sign) {
				//TODO copy digits into array
				for (unsigned int i = 0; i < used; i++) {
					//digits[i];
					//*(numCanditate.rbegin()+i)

					//digits
					;
				}

				foundValid = true;
			} else {
				//dunno, init to zero maybe?
				//reset everything changed within validation (shift, sign)
				shift = 0;
				used = 0;
				negative = false;
			}
		}

//		for (int i = (end-1); i >= begin; i--) { //index of digits should be -i + end-1
//			if('0' <= number[i] && number[i] <= '9'){
//				//if digit, store in array
//				digit[-i+end-1] = number[i] - (int)'0';
//
//				used++;
//				shift = (foundPoint) ? (shift+1) : (shift-1);
//			} else if ()
//		}
		//copy digits
		for (auto it = numCandidate.rbegin(); it != numCandidate.rend(); it++) {
			//insert digits, set sign
		}
	}
//TODO read digits
}

CCDecimal::CCDecimal(double number) {
	pPrecision = &CCDecimal::defaultPrecision;
	std::stringstream stringStream;
	std::string numberAsString;

	stringStream << std::setprecision(std::numeric_limits<double>::digits10)
			<< number;

	numberAsString = stringStream.str();

	//basically copy-paste the rest from CCDecimal(string)
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

