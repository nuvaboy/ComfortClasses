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
unsigned int CCDecimal::defaultPrecision = 3;

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
	if (pPrecision == &d2.precision) {
		pPrecision = &precision;
	}
}

CCDecimal::CCDecimal(const char* str) {
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

	stringStream << std::setprecision(std::numeric_limits<double>::digits10) << number;

	numberStr = stringStream.str();

	constructFromString(numberStr);
}

CCDecimal::~CCDecimal() {
}

//public setter/getter
unsigned int CCDecimal::getGlobalPrecision() {
	return CCDecimal::defaultPrecision - 1;
}

void CCDecimal::setGlobalPrecision(unsigned int precision) {
	CCDecimal::defaultPrecision = precision + 1;
}

//utility functions
void CCDecimal::constructFromString(const string& numberStr) {
	if (!numberStr.empty()) {
		unsigned int begin = numberStr.find_first_of("-+.0987654321e", 0);
		unsigned int end = numberStr.find_first_not_of("-+.0987654321e", begin);

		if (begin == std::string::npos) std::cout << "\n didn't find beginning \n";
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
															((('0' <= *it && *it <= '9') ?
																	(digit_after_sign) : (error))))));
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
									(('0' <= *it && *it <= '9') ? (digit_after_point) : (error));
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
											(('0' <= *it && *it <= '9') ? (validator) : (error)));
					break;
				case point_after_digit:
					//next state
					it++;
					validator =
							(it == numCandidate.end()) ?
									(valid_end) :
									(('0' <= *it && *it <= '9') ? (digit_after_point) : (error));
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
											((*it == 'e') ? (exponent_after_digit) : (error)));
					break;
				case exponent_after_digit:
					//next state
					numCandidate.erase(it); //remove so not present in following processing
					validator =
							(it == numCandidate.end()) ?
									(error) :
									((*it == '+') ?
											(exponent_positive) :
											((*it == '-') ? (exponent_negative) : (error)));
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
									(('0' <= *it && *it <= '9') ? (digit_after_exponent) : (error));
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
									(('0' <= *it && *it <= '9') ? (validator) : (error));
					break;
				case error:
					it++;
					break;
				default:
					std::cout << "FSM in default. This should not happen." << std::endl;
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
						std::cout << "digitsToCut:" << digitsToCut << "; digitsToSpare:"
								<< digitsToSpare;
						if (!(digitsToCut >= digitsToSpare)) {
							cutOffset = digitsToCut;
						}
						else {
							throw std::overflow_error("Digits to store surpassing precision limit");
						}
					}
					//TODO copy digits into array
					int i = 0;
					rit = numCandidate.rbegin() + cutOffset;
					while (rit != numCandidate.rend() && i <= MAX) {
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

			}
			else {
				//dunno, init to zero maybe?
				//reset everything changed within validation (shift, used, sign)
				std::cout << "\n invalid number. \n";
				shift = 0;
				used = 0;
				isNegative = false;
			}
		}
		else {
			std::cout << "\n string cannot contain a number. \n";
		}
	}
	else {
		std::cout << "\n empty string. \n";
	}
}

//core functionality
bool CCDecimal::lessThan(const CCDecimal& op2) const {

	if (used + shift > op2.used + op2.shift) return false;

	if (used + shift < op2.used + op2.shift) return true;

	int i = used - 1;
	int j = op2.used - 1;

	while (i >= 0 && j >= 0) {
		if (digit[i] > op2.digit[j]) return false;
		if (digit[i] < op2.digit[j]) return true;
		i--;
		j--;
	}
	return false;
}

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
//	for (unsigned int i = 0; i + digToCut < pMostPrec->used; i++) {
//		result->digit[i] = pMostPrec->digit[i + digToCut];
//	}
	for (unsigned int i = digToCut; i < pMostPrec->used; i++) {
		result->digit[i - digToCut] = pMostPrec->digit[i];
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

CCDecimal CCDecimal::sub2(const CCDecimal& opSmall) const {

	//create empty decimal
	CCDecimal result;
	if (opSmall.used == 0) return result;

	int pos_curr = 0;
	bool carry = true;

	//calculate shift_delta
	int shift_delta = opSmall.shift - shift;

	//step 1: append tail
	int tail_length = 0;

	if ((carry = shift_delta < 0)) { //tail exists (a tail always produces a carry for the body)
		tail_length = -shift_delta;

		//subtract tail and append to the result (propagate carry)
		result.digit[0] = 10 - opSmall.digit[0];
		for (int i = 1; i < min<int>(tail_length, opSmall.used); i++) {
			result.digit[i] = 9 - opSmall.digit[i];
		}
		pos_curr = min<int>(tail_length, opSmall.used);
	}

	//step 2: append body
	int tz_count = 0;
	if (opSmall.shift + (int) opSmall.used > shift) { //body exists


		if (shift == opSmall.shift) {
			//remove trailing zeroes (where can not be trailing zeroes if tail caused a carry)
			while (tz_count < (int) opSmall.used && digit[tz_count] == opSmall.digit[tz_count]) {
				tz_count++;
			}
		}
		else {
			//append non intersecting part of the body
			for (int i = 0; i < shift_delta; i++) {
				result.digit[pos_curr] = digit[i];
				pos_curr++;
			}
		}

		//subtract body and append to result (propagate carry)
		for (int i = tail_length + tz_count; i < (int) opSmall.used; i++) {
			int temp = digit[i - tail_length + max(0, shift_delta)] - opSmall.digit[i] - carry;
			if ((carry = temp < 0)) temp += 10;
			result.digit[pos_curr] = temp;
			pos_curr++;
		}
	}

	//step 3.1: leading zeroes in head
	int lz_count = 0;
	int head_length = 0; // = used;
	bool hasHead = (shift + (int) used > opSmall.shift + (int) opSmall.used);

	if (hasHead) { //head exists
		head_length = min(used, shift + used - opSmall.shift - (int) opSmall.used);

		//will head be eliminated by leading zeroes
		if (carry && digit[used - 1] == 1) {
			int i = head_length - 1;
			if ((hasHead = head_length > 1)) {
				while (digit[i] == 0)
					i--;
			}
			if (i == 0) lz_count = 1;
		}
	}

	//step 3.2: leading zeroes in body and tail
	if (!hasHead && pos_curr > 0) {
		while (result.digit[pos_curr - 1] == 0) {
			pos_curr--;
			lz_count++;
		}
	}

	//estimate used digits
	int result_used = used + max(-shift_delta, 0) - tz_count - lz_count;
	int result_shift = min(shift, opSmall.shift) + tz_count;
	int digToCut = max(0, result_used - MAX);
	if (digToCut > 0) {
		int digToSpend = -min(shift, opSmall.shift) - *pPrecision;
		if (digToSpend < digToCut) {
			//overflow
			cout << "overflow" << endl;
			return result;
		}

		//cut digits
		for (int i = digToCut; i < pos_curr; i++) {
			result.digit[i - digToCut] = result.digit[i];
		}
		pos_curr -= digToCut;
		result_used = MAX;
		result_shift += digToCut;
	}

	//propagate carry through gap
	if (carry) {
		while (pos_curr < tail_length - tz_count) {
			result.digit[pos_curr++] = 9;
		}
	}

	//append head  (propagate carry)
	for (int i = used - head_length; i < (int) used - lz_count; i++) {
		int temp = digit[i] - carry;
		if ((carry = temp < 0)) temp += 10;
		result.digit[pos_curr] = temp;
		pos_curr++;
	}

	result.used = result_used;
	result.shift = result_shift;

	return result;

}
void CCDecimal::sub(CCDecimal* result, const CCDecimal& opSmall) const {

//copy itself to the result
	*result = *this;

//exit by 0 - 0 = 0
	if (used == 0) return;

	int shift_delta = opSmall.shift - shift;

//calculate and apply carry in advance for the least significant digit of the bigger decimal (this)
	if (shift_delta < 0) result->digit[0]--;

	bool carry = false; //shift_delta < 0;

	int unhandled = (int) opSmall.used;

//the decimals have an intersection
	if (opSmall.shift + (int) opSmall.used >= shift) {

		//set small_pos to the position of the first intersecting digit in opSmall (save amount of unhandled alias non-intersecting digits)
		int small_pos = unhandled = (shift_delta < 0) ? -shift_delta : 0;

		//subtract intersecting digits
		while (small_pos < (int) opSmall.used) {
			int temp = result->digit[small_pos + shift_delta] - opSmall.digit[small_pos] - carry;
			carry = (temp < 0);
			if (carry) temp += 10;
			result->digit[small_pos + shift_delta] = temp;
			small_pos++;
		}

		//propagate carry
		while (carry) {
			int temp = result->digit[small_pos + shift_delta] - 1;
			carry = (temp < 0);
			if (carry) temp += 10;
			result->digit[small_pos + shift_delta] = temp;
			small_pos++;
		}
	}

//count trailing zeroes
	int tz_count = 0;
	if (shift == opSmall.shift) {
		while (tz_count < (int) used && result->digit[tz_count] == 0) {
			tz_count++;
		}
	}

//count leading zeroes
	int lz_count = 0;
	if (tz_count != (int) used) {
		for (int i = used - 1; i >= 0 && result->digit[i] == 0; i--) {
			lz_count++;
		}
	}

//calculate the amount of used digits for the result in advance
	int result_used = used - min(shift_delta, 0) - lz_count - tz_count;
	cout << "calculate result.used in advance: " << result_used << endl;

	int digToCut = max(result_used - MAX, 0);
	cout << "digits to cut: " << digToCut << endl;

	int digToSpend = -min(shift, opSmall.shift) - *pPrecision;
	cout << "digits to spend: " << digToSpend << endl;

//overflow detection
	if (digToCut > digToSpend) {
		cout << "overflow, precision requirements do not hold" << endl;
	}

	if (tz_count > 0) {

		//right shift to remove trailing zeroes
		for (int i = 0; i <= (int) used; i++) {
			result->digit[i] = result->digit[i + tz_count];
		}
	}
	else if (-shift_delta - digToCut > 0) {

		int movLeft = -shift_delta - digToCut;

		//left shift to make space for unhandled digits
		for (int i = used; i >= 0; i--) {
			result->digit[i + movLeft] = result->digit[i];
			result->digit[i] = 0;
		}

		//insert unhandled digits
		carry = false;
		for (int i = 0; i < unhandled - digToCut; i++) {
			result->digit[i] = 10 - opSmall.digit[digToCut + i] - carry;
			carry = true;
		}

		//propagate carry
		for (int i = unhandled - digToCut; i < -shift_delta; i++) {
			result->digit[i] = 9;
		}

	}

	result->used = result_used;
	result->shift = min(opSmall.shift, shift) + digToCut + tz_count;
///   123
///  123 123
//step 2: subtract second part

//	int shift_delta = opSmall.shift - shift;
//	int b0 = max(0, shift_delta);
//	int s0 = max(0,-shift_delta);
//	int b1 = 0;
//
//
//	int carry = s0 > 0; //smallest operand has highest precision (implicit carry)
//
//	if  (opSmall.shift + (int)opSmall.used > shift){ //has intersection
//
//		cout << "b0: " << b0  <<  ", s0: " << s0 << endl;
//
//		//subtract and copy intersection
//		for (int i = s0; i < (int)opSmall.used; i++){
//			int temp =digit[i-s0+b0] - opSmall.digit[i] - carry;
//			carry = temp < 0;
//			if (carry)temp += 10;
//			result->digit[i-s0] = temp;
//		}
//		b1 = -s0+b0 + opSmall.used;
//	}
//
//	//copy head
//	for (int i = b1; i < (int)used; i++){
//		int temp = digit[i]-carry;
//		carry = temp < 0;
//		if(carry)temp+=10;
//		result->digit[i-b0] = temp;
//	}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	int offset = -1;
//	if (opSmall.shift + (int)opSmall.used <= shift){ //separated
//		offset = -1;
//	}else if (opSmall.shift < shift){ //joined
//		offset = shift - opSmall.shift; //-shift_delta
//		//-offset
//		cout << "joined:" << endl;
//	}else{ //included
//		offset = 0;
//		//+abs(shift_delta)
//	}
//	cout << "offset: " << offset << endl;
//
//	bool hasBody = opSmall.shift+opSmall.used > shift;
//	int body_size = max<int>(opSmall.shift+opSmall.used-shift, 0);
//	//-> shift_delta
//					//0
//
//	int head_size = used - body_size;
//	//body_size, body_offset,
//	//1. copy opBig to result
//	//if (hasFoot)
//	//
//	//else
//	//
//
//	//22 - 11.25 = 10.75
//
//	int foot_size = shift_delta < 0 ? 0 : shift_delta;
//	//int head_size;// = opBig.shift + opBig.used - shift - used;
//	//int body_size;// = opBig.used - head_size - foot_size;
}

void CCDecimal::mult(CCDecimal* result, const CCDecimal& op2) const {

	//determine the decimal numbers with most and least digits
	const CCDecimal* pSmall = this;
	const CCDecimal* pBig = &op2;
	if (op2.used < used) {
		pSmall = &op2;
		pBig = this;
	}

	bool tzFlag = true; //flag is set while a result of 0 at an index means a trailing zero
	unsigned int resultUsedMax = pSmall->used + pBig->used - 1; //index of the

	int resultIndex = 0; //current position in the result

	//index is the currently calculated coefficient of the result
	for (unsigned int index = 0; index < resultUsedMax; index++) {

		//calculate all multiplications for the current index
		cout << index << ": ";
		unsigned int i_begin = max<int>(index - pSmall->used + 1, 0);
		int j = index - i_begin;

		for (unsigned int i = i_begin; j >= 0 && i < pBig->used; i++) {
			cout << "(" << i << ", " << j << ")";
			result->digit[resultIndex] += pBig->digit[i] * pSmall->digit[j];
			j--;
		}
		cout << endl;

		//correct result and calculate carry
		unsigned int carry = result->digit[resultIndex] / 10;
		result->digit[resultIndex] -= carry * 10;

		//remove trailing Zeroes
		if (tzFlag) {
			if (result->digit[resultIndex] == 0) {
				resultIndex--;
			}
			else {
				tzFlag = false; //result was not 0, so where can not be more trailing zeroes
			}
		}

//apply carry
		if (resultIndex < MAX) {
			result->digit[resultIndex + 1] += carry;
		}

//overflow
		if (result->digit[MAX] > 0) {
			throw std::overflow_error(
					"Result is too large to store in Decimal. Keep values in range or reduce precision!");
		}

		resultIndex++; //increment according to index unless trailing Zeroes were removed
	}

//adjust the used and shift of the result
	result->used = resultIndex;
	int tzCount = resultUsedMax - resultIndex; //amount of invisible trailing Zeroes
	result->shift = shift + op2.shift + tzCount;

}

//others
CCDecimal CCDecimal::operator +(const CCDecimal& op2) const {
	CCDecimal result;
	add(&result, op2);

	return result;
}
CCDecimal CCDecimal::operator *(const CCDecimal& op2) const {
	CCDecimal result;
	result.isNegative = isNegative != op2.isNegative;
	mult(&result, op2);

	return result;
}
CCDecimal& CCDecimal::operator *=(const CCDecimal& op2) {

	*this = *this * op2;
	return *this;
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

