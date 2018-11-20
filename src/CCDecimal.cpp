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

//initialize static properties
unsigned int CCDecimal::defaultPrecision = 2;

//constructors
CCDecimal::CCDecimal() { /* construct as default */

	//precision should be the default precision, unless explicitly changed for an instance
	pPrecision = &CCDecimal::defaultPrecision;

	//initialize all digits to zero
	for (int i = 0; i <= MAX; i++) {
		digit[i] = 0;
	}
	used = 0;
	shift = 0;
}

CCDecimal::CCDecimal(const CCDecimal& d2) :
		CCDecimal() {
	*this = d2;
	if (pPrecision == &d2.precision){
		pPrecision = &precision;
	}
}

CCDecimal::CCDecimal(const char* str){
	string s(str);
	*this = s;
}

CCDecimal::CCDecimal(const string& numberStr) /* construct from string */:
		CCDecimal() {
	constructFromString(numberStr);
}

CCDecimal::CCDecimal(double number) { /* construct from double */
	pPrecision = &CCDecimal::defaultPrecision;
	std::stringstream stringStream;
	std::string numberStr;

	stringStream << std::setprecision(std::numeric_limits<double>::digits10)
			<< number;

	numberStr = stringStream.str();

	constructFromString(numberStr);
}

CCDecimal::~CCDecimal() {
}

//public setter/getter
unsigned int CCDecimal::getDefaultPrecision() {
	return CCDecimal::defaultPrecision - 1;
}

void CCDecimal::setDefaultPrecision(unsigned int precision) {
	CCDecimal::defaultPrecision = precision + 1;
}

//utility functions
void CCDecimal::constructFromString(const string& numberStr) {
	if (!numberStr.empty()) {
		unsigned int begin = numberStr.find_first_of("-+.0987654321e", 0);
		unsigned int end = numberStr.find_first_not_of("-+.0987654321e", begin);

		if (begin == std::string::npos)
			std::cout << "\n didn't find beginning \n";
		if (begin != std::string::npos) {

			std::string numCandidate = numberStr.substr(begin, end - 1);

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
				exponent_after_digit,
				exponent_positive,
				exponent_negative,
				digit_after_exponent,
				valid_end
			};

			ValidatorStates validator = start;
			int exponent = 0;
			int exponentSign = 1;

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
											(validator) :
											((*it == 'e') ?
													(exponent_after_digit) :
													(error)));
					break;
				case exponent_after_digit:
					//next state
					numCandidate.erase(it); //remove so not present in following processing
					validator =
							(it == numCandidate.end()) ?
									(error) :
									((*it == '+') ?
											(exponent_positive) :
											((*it == '-') ?
													(exponent_negative) :
													(error)));
					break;
				case exponent_negative:
					exponentSign = -1;
					/* no break */
				case exponent_positive:
					//next state
					numCandidate.erase(it); //remove so not present in following processing
					validator =
							(it == numCandidate.end()) ?
									(error) :
									(('0' <= *it && *it <= '9') ?
											(digit_after_exponent) : (error));
					break;
				case digit_after_exponent:
					//change exponent
					exponent *= 10;	//basically shift all current digits
					exponent += exponentSign * (*it - '0'); //add current digit in

					//next state
					numCandidate.erase(it); //remove so not present in following processing
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
//				numCandidate = numCandidate.substr(0,
//						numCandidate.find_first_not_of("-+.0987654321", 0) - 1);

				/* add present exponent in (if not applicable, is zero)*/
				shift += exponent;
				/* trim number */
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
						} else {
							throw std::overflow_error(
									"Digits to store surpassing precision limit");
						}
					}
					//TODO copy digits into array
					int i = 0;
					rit = numCandidate.rbegin() + cutOffset;
					while (rit != numCandidate.rend() && i <= MAX) {
						if (*rit == '.' || *rit == '+' || *rit == '-') {
							rit++;
						} else {
							digit[i] = ((*rit) - '0');
							i++;
							rit++;
						}
					}

				} else {
					std::cout
							<< "\n trimmed number empty. assuming value zero.\n";
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

//core functionality
void CCDecimal::add(CCDecimal* result, const CCDecimal& op2) const {

	//determine the most and the least precise decimal
	const CCDecimal* pMostPrec = this;
	const CCDecimal* pLeastPrec = &op2;
	if (op2.shift < shift) {
		pMostPrec = &op2;
		pLeastPrec = this;
	}

	int shift_result = pMostPrec->shift; //dpPos_min
	int shift_delta = pLeastPrec->shift - pMostPrec->shift; //dpPos_gap
	int used_result = max(pMostPrec->used, pLeastPrec->used + shift_delta); //calculate coalesced size

	//calculate amount of expendable digits (digits to spend)
	int digToSpend = -*pPrecision - shift_result;

	//calculate amount of digits needed to be cut to meet size requirements (digits to cut)
	int digToCut = max(used_result - MAX, 0);

	//check, if cutting of decimal places is necessary
	if (digToCut > 0) {

		if (digToSpend < digToCut) { //overflow
			throw std::overflow_error(
					"Result is too large to store in Decimal. Keep values in range or reduce precision!");
		}

		//cut decimal places
		used_result = MAX;
		shift_result += digToCut;

		if (digToCut <= shift_delta) {
			shift_delta -= -digToCut;
		}
	}

	//add most precise operand to the result
	for (unsigned int i = 0; i + digToCut < pMostPrec->used; i++) {
		result->digit[i] = pMostPrec->digit[i + digToCut];
	}

	//add least precise operand to the result
	for (unsigned int i = 0; i < pLeastPrec->used; i++) {
		result->digit[i + shift_delta] += pLeastPrec->digit[i];
	}

	//forward carries
	for (int i = 0; i < used_result; i++) {
		if (result->digit[i] >= 10) {
			result->digit[i] -= 10;
			result->digit[i + 1]++;
		}
	}
	if (result->digit[used_result] > 0) {
		used_result++;
	}

	//remove trailing zeros
	if (result->digit[0] == 0) {

		int trailingZeroes = 1;
		while (result->digit[trailingZeroes] == 0 && trailingZeroes < MAX) {
			trailingZeroes++;
		}
		for (int i = trailingZeroes; i <= MAX; i++) {
			result->digit[i - trailingZeroes] = result->digit[i];
		}

		//decrease used and increase shift according to the number of leading zeroes
		used_result -= trailingZeroes;
		shift_result += trailingZeroes;

		//clear overflow flag
		result->digit[MAX] = 0;
	}

	if (result->digit[MAX] > 0) {
		//overflow
		throw std::overflow_error(
				"Result is too large to store in Decimal. Keep values in range or reduce precision!");
	}

	result->used = used_result;
	result->shift = shift_result;

}

//others
CCDecimal CCDecimal::operator +(const CCDecimal& op2) const {
	CCDecimal result;
	add(&result, op2);

	return result;
}

bool CCDecimal::operator ==(const CCDecimal& op2) const {

	//return false, if either used or shift is not equal
	if (used != op2.used || shift != op2.shift) {
		return false;
	}

	//compares each individual digit, return false as soon as a mismatch is found
	for (unsigned int i = 0; i < used; i++) {
		if (digit[i] != op2.digit[i]) {
			return false;
		}
	}

	//return true if neither of the above checks fails
	return true;
}

