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
#include <algorithm>

#include "CCDecimal.h"

using namespace std;

unsigned int CCDecimal::defaultPrecision = 2;

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
	if (!number.empty()) {
		unsigned int begin = number.find_first_of("-+.0987654321", 0);
		unsigned int end = number.find_first_not_of("-+.0987654321", begin);

		if (begin == std::string::npos || end == std::string::npos) {

			std::string numCandidate = number.substr(begin, end - 1);

			/*
			 * validate: max one sign; max one point; sign at the beginning,
			 * then digits, then point, if point, then more digits
			 * uses state-machine-ish behaviour, result valid if in valid_end.
			 */
			enum ValidatorStates {
				error = -1,
				start,
				sign_negative,
				sign_positive,
				point_after_sign,
				digit_after_sign,
				point_after_digit,
				end_after_point,
				digit_after_point,
				valid_end
			};

			ValidatorStates validator = start;
			auto it = numCandidate.begin();
			while (it != numCandidate.end()) {
				switch (validator) {
				case start:
					validator =
							(it == numCandidate.end()) ?
									(error) :
									((*it == '-') ?
											(sign_negative) :
											((*it == '+') ?
													(sign_positive) :
													((*it == '.') ?
															(point_after_sign) :
															((('0' <= *it
																	&& *it
																			<= '9') ?
																	(digit_after_sign) :
																	(error))))));
					break;
				case sign_negative:
					//set sign
					isNegative = true;
					/* no break */
				case sign_positive:
					//next state
					it++;
					validator =
							(it == numCandidate.end()) ?
									(error) :
									((*it == '.') ?
											(point_after_sign) :
											(('0' <= *it && *it <= '9') ?
													(digit_after_sign) : (error)));
					break;
				case point_after_sign:
					//next state
					it++;
					validator =
							(it == numCandidate.end()) ?
									(error) :
									(('0' <= *it && *it <= '9') ?
											(digit_after_point) : (error));
					break;
				case digit_after_sign:
					//tracking digit
					used++;
					//next state
					it++;
					validator =
							(it == numCandidate.end()) ?
									(valid_end) :
									((*it == '.') ?
											(point_after_digit) :
											(('0' <= *it && *it <= '9') ?
													(validator) : (error)));
					break;
				case point_after_digit:
					//next state
					it++;
					validator =
							(it == numCandidate.end()) ?
									(valid_end) :
									(('0' <= *it && *it <= '9') ?
											(digit_after_point) : (error));
					break;
				case digit_after_point:
					//tracking shift
					shift--;
					//tracking digit
					used++;
					//next state
					it++;
					validator =
							(it == numCandidate.end()) ?
									(valid_end) :
									(('0' <= *it && *it <= '9') ?
											(validator) : (error));
					break;
				case error:
					it++;
					break;
				default:
					std::cout << "FSM in default. This should not happen."
							<< std::endl;
				}
			}

			//if valid string representation of a double
			if (validator == valid_end) {
				//TODO trim number
				//remove leading zeroes
				auto it = numCandidate.begin();
				if (*it == '+' || *it == '-') {
					it++;
				}
				while (it != numCandidate.end() && *it == '0') {
					numCandidate.erase(it);
					used--;
					//skip over decimal point
					if (it != numCandidate.end() && *it == '.') {
						it++;
					}
				}
				//remove trailing zeroes
				auto rit = numCandidate.rbegin();
				while (rit != numCandidate.rend() && *rit == '0') {
					numCandidate.erase(rit.base() - 1);
					shift++;
					used--;
					rit++;
					//skip over decimal point
					if (rit != numCandidate.rend() && *rit == '.') {
						rit++;
					}
				}
				if (!numCandidate.empty()) {
					//TODO check for fit in type
					int cutOffset = 0;
					if (used > MAX) {
						int digitsToCut = used - MAX;
						int digitsToSpare = -*pPrecision + shift;
						std::cout << "digitsToCut:" << digitsToCut
							<< "; digitsToSpare:" << digitsToSpare;
						if (!(digitsToCut >= digitsToSpare)) {
							cutOffset = digitsToCut;
						}
						else {
							std::cout << "\n overflow. \n";
						}
					}
					//TODO copy digits into array
					int i = 0;
					rit = numCandidate.rbegin() + cutOffset;
					while (i <= MAX && rit != numCandidate.rend()) {
						if (*rit == '.' || *rit == '+' || *rit == '-') {
							rit++;
						}
						else {
							digit[i] = ((*rit) - '0');
							i++;
							rit++;
						}
					}
				}
				else {
					std::cout << "\n trimmed number empty. assuming value zero.\n";
					shift = 0;
					used = 0;
				}

			} else {
				//dunno, init to zero maybe?
				//reset everything changed within validation (shift, used, sign)
				std::cout << "\n invalid number. \n";
				shift = 0;
				used = 0;
				isNegative = false;
			}
		} else {
			std::cout << "\n string cannot contain a number. \n";
		}
	} else {
		std::cout << "\n empty string. \n";
	}
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
		//cout << "false, shift or used" << endl;
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
				shift_delta -= -digToCut;
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
	if (result->digit[size] > 0) {
		result->used++;
	}

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

