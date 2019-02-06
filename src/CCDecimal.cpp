/*
 * Decimal.cpp
 *
 *  Created on: 24.10.2018
 *      Author: marlo
 */
#include "gtest/gtest.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <exception>

#include "CCDecimal.h"
#include <cctype>
#include <cstdlib>

using namespace std;

//initialize static properties
uint32_t CCDecimal::defaultPrecision = 3;

//constructors
CCDecimal::CCDecimal() { /* construct with default value (0) */

	//precision should be the default precision, unless explicitly changed for an instance
	pPrecision = &CCDecimal::defaultPrecision;

	//initialize all digits to zero
	for (int32_t i = 0; i <= MAX; i++) {
		digit[i] = 0;
	}
	used = 0;
	shift = 0;
}

CCDecimal::CCDecimal(const CCDecimal& d2) : /* construct as a copy */
		CCDecimal() {
	*this = d2;
	if (pPrecision == &d2.precision) { //local precision of d2 was set
		pPrecision = &precision; //correct pointer due to shallow copy
	}
}

CCDecimal::CCDecimal(const char* str) /* construct from C-string */{
	string s(str);
	*this = s;
}

CCDecimal::CCDecimal(const string& numberStr) /* construct from string */:
		CCDecimal() {
	constructFromString(numberStr);
	//cfs(numberStr);
}

CCDecimal::CCDecimal(double number) /* construct from double */:
		CCDecimal() {
	std::stringstream stringStream;
	std::string numberStr;

	stringStream << std::setprecision(std::numeric_limits<double>::digits10) << number;
	numberStr = stringStream.str();

	constructFromString(numberStr);
}

CCDecimal::~CCDecimal() {
}

//public setter/getter
uint32_t CCDecimal::getLocalPrecision() {
	return *pPrecision - 1;
}

void CCDecimal::setLocalPrecision(uint32_t p) {
	precision = p + 1;
	pPrecision = &precision;
}

uint32_t CCDecimal::getGlobalPrecision() {
	return CCDecimal::defaultPrecision - 1;
}

void CCDecimal::setGlobalPrecision(uint32_t precision) {
	CCDecimal::defaultPrecision = precision + 1;
}

//91
//core functionality
void CCDecimal::add(CCDecimal* result, const CCDecimal& op2) const {

	ASSERT_TRUE((*result == CCDecimal()));

	//determine the most and the least precise decimal
	const CCDecimal* pMostPrec = this;
	const CCDecimal* pLeastPrec = &op2;
	if (op2.shift < shift) {
		pMostPrec = &op2;
		pLeastPrec = this;
	}

	int32_t shift_result = pMostPrec->shift; //dpPos_min
	int32_t shift_delta = pLeastPrec->shift - pMostPrec->shift; //dpPos_gap
	int32_t used_result = max(pMostPrec->used, pLeastPrec->used + shift_delta); //calculate coalesced size

	//calculate amount of expendable digits (digits to spend)
	int32_t digToSpend = -*pPrecision - shift_result;

	//calculate amount of digits needed to be cut to meet size requirements (digits to cut)
	int32_t digToCut = max(used_result - MAX, 0);

	//check, if cutting of decimal places is necessary
	if (digToCut > 0) {

		if (digToSpend < digToCut) { //overflow
			throw std::overflow_error(
					"Result is too large to store in Decimal. Keep values in range or reduce precision!");
		}

		//cut decimal places
		used_result = MAX;
		shift_result += digToCut;

		ASSERT_TRUE(digToCut <= shift_delta);
		shift_delta -= digToCut;
	}

	//add most precise operand to the result
	for (uint32_t i = digToCut; i < pMostPrec->used; i++) {
		result->digit[i - digToCut] = pMostPrec->digit[i];
	}

	//add least precise operand to the result
	for (uint32_t i = 0; i < pLeastPrec->used; i++) {
		result->digit[i + shift_delta] += pLeastPrec->digit[i];
	}

	//forward carries
	for (int32_t i = 0; i < used_result; i++) {
		if (result->digit[i] >= 10) {
			result->digit[i] -= 10;
			result->digit[i + 1]++;
		}
	}
	if (result->digit[used_result] > 0) {
		used_result++;
	}

	//remove trailing zeros
	if (used_result > 0 && result->digit[0] == 0) {

		int32_t trailingZeroes = 1;
		while (result->digit[trailingZeroes] == 0 && trailingZeroes < used_result) {
			trailingZeroes++;
		}
		for (int32_t i = trailingZeroes; i < used_result; i++) { //<resultUsed tatt <=MAX
			result->digit[i - trailingZeroes] = result->digit[i];
		}

		//decrease used and increase shift according to the number of leading zeroes
		used_result -= trailingZeroes;
		shift_result += trailingZeroes;

		//clear overflow flag
		result->digit[MAX] = 0;
	}
	else if (result->digit[MAX] > 0) {
		//overflow
		throw std::overflow_error(
				"Result is too large to store in Decimal. Keep values in range or reduce precision!");
	}

	result->used = used_result;
	result->shift = shift_result;

}

//124 ====> (284 - 182 + 1 = 103) (329 - 182 + 1 = 148) (329 - 180 + 1 = 150)
void CCDecimal::sub(CCDecimal* result, const CCDecimal& opSmall) const {

	//Subtract zero
	if (opSmall.used == 0) {
		*result = *this;
		return;
	}

	int32_t shift_delta = opSmall.shift - shift;

	int32_t toCut = 0;
	int32_t toSpend = 0;

	int32_t merged_used = max<int32_t>(-shift_delta, 0) + (int32_t) used;
	int32_t tail_length = max<int32_t>(-shift_delta, 0);

	//cutting
	if (merged_used > MAX) {
		toCut = merged_used - MAX;
		toSpend = max<int32_t>(-min<int32_t>(shift, opSmall.shift) - *pPrecision, 0);

		if (toCut > toSpend + 1)
			throw std::overflow_error("Can not cut result to minimal precision!");
	}

	bool hasTail = shift_delta < 0;

	//trailing zeroes
	bool carry = hasTail;
	int32_t tz_body = 0;
	int32_t tz_head = 0;
	if (toCut == -shift_delta) {
		int32_t search_pos = tail_length;

		if (search_pos == (int32_t) opSmall.used) { //no body, trailing zero into head due to carry
			if (hasTail && digit[0] == 1) {
				search_pos++;
				tz_head++;
				carry = false;
			}
		}
		else if (search_pos < (int32_t) opSmall.used) {

			if (hasTail) { //trailing zero due to carry into body
				if (digit[search_pos - tail_length] == opSmall.digit[search_pos] + 1) {
					tz_body++;
					search_pos++;
					carry = false;
				}
				else {
					search_pos = MAX + 1; //end search for trailing zeroes

				}
			}

			while (search_pos < (int32_t) opSmall.used
					&& digit[search_pos - tail_length] == opSmall.digit[search_pos]) {
				search_pos++;
				tz_body++;
			}

		}

		//trailing zeroes in head
		//HINT:if (search_pos >= (int32_t) opSmall.used) is implicitly true
		for (; search_pos < (int32_t) used && digit[search_pos - tail_length] == 0; search_pos++) {
			tz_head++;
		}
	}

	//Cut one less?
	if (toCut > 0 && tz_body == 0 && tz_head == 0) {

		//Compute in advance if the MSD will become zero.
		if (digit[used - 1] == 1) { //HINT: Where has to be a head, if cutting is necessary.

			//filter leading zeroes
			int32_t i = (int32_t) used - 2;
			if (i < 0) {
				toCut--;
				printf("MSD will become zero!\n");
			}
			else {
				int32_t end = (int32_t) opSmall.used + shift_delta;
				for (; i >= end && i >= 0 && digit[i] == 0; i--) {
				}

				//filter equal digits which in consequence will be zero
				if (i == end - 1) {

					while (i-- >= 0 && digit[i + 1] == opSmall.digit[i + 1 - shift_delta]) { //shift_delta has to be negative, if cutting
					}

					if (i < end - 1 && (digit[i + 1] < opSmall.digit[i + 1 - shift_delta] || /* one digit is greater and produces carry */
					(i == -1 && shift_delta < 0))) { /* all equal and tail produces carry */
						toCut--;
						printf("MSD will become zero!\n");
					}

				}
			}

		}
	}

	if (toCut > toSpend) throw std::overflow_error("Can not cut result to minimal precision!");

	int32_t result_pos = 0;

	//read tail (negative shift_delta)
	int32_t i = toCut;
	if (hasTail) {
		if (i == 0) result->digit[result_pos++] = 10 - opSmall.digit[i++]; //LSD has no carry-in

		for (; i < min<int32_t>(-shift_delta, opSmall.used); i++) {
			result->digit[result_pos++] = 9 - opSmall.digit[i];
		}
		for (; i < -shift_delta; i++) {
			result->digit[result_pos++] = 9;
		}
	}
	//read (positive shift_delta)
	else {
		for (; i < shift_delta; i++) {
			result->digit[result_pos++] = digit[i];
		}
	}

	//read body
	int32_t x = tail_length + tz_body;
	for (; x < (int32_t) opSmall.used; x++) {
		int32_t temp = digit[x - tail_length + max(0, shift_delta)] - opSmall.digit[x] - carry;
		if ((carry = temp < 0)) temp += 10;
		result->digit[result_pos++] = temp;
	}

	//read head
	for (int32_t i = max<int32_t>((int32_t) opSmall.used + shift_delta, 0) + tz_head;
			i < (int32_t) used; i++) {
		int32_t temp = digit[i] - carry;

		if ((carry = temp < 0)) temp += 10;
		result->digit[result_pos++] = temp;
	}

	//last carry
	if (result_pos > MAX) {
		result_pos--;
	}
	else {
		result->digit[result_pos] = carry;
	}

	//leading zeroes
	while (result_pos >= 0 && result->digit[result_pos] == 0) {
		result_pos--;
	}

	//overflow detection
	if (result->digit[MAX] == 1) throw std::overflow_error("test");

	//adjust used and shift
	result->used = result_pos + 1;
	result->shift = min<int32_t>(shift, opSmall.shift) + toCut + tz_body + tz_head;

	if (result_pos < 0) {
		result->shift = 0;
	}
}

//63
void CCDecimal::mult(CCDecimal* result, const CCDecimal& op2) const {

	if (used == 0 || op2.used == 0) return;

	//determine the decimal numbers with most and least digits
	const CCDecimal* pSmall = this;
	const CCDecimal* pBig = &op2;
	if (op2.used < used) {
		pSmall = &op2;
		pBig = this;
	}

	int32_t carryFromCut = 0;
	//53,28 * 89,8965
	bool tzFlag = true; //flag is set while a result of 0 at an index means a trailing zero
	uint32_t resultUsedMax = pSmall->used + pBig->used - 1; //index of the

	int32_t toCut = 0;
	if (resultUsedMax > MAX) {
		toCut = (int32_t) resultUsedMax - MAX;
		int32_t shift_sum = pSmall->shift + pBig->shift;

		int32_t toSpend = -shift_sum - *pPrecision;
		if (toSpend < toCut) throw std::overflow_error("");

		////fix fix
		int32_t ri = 0;

		for (uint32_t index = 0; (int32_t) index < toCut; index++) {

			uint32_t i_begin = max<int32_t>(index - pSmall->used + 1, 0);
			int32_t j = index - i_begin;

			int32_t x = carryFromCut;
			carryFromCut = 0;
			for (uint32_t i = i_begin; j >= 0 && i < pBig->used; i++) {
				x += pBig->digit[i] * pSmall->digit[j];
				int32_t temp = x / 10;
				x -= 10 * temp;
				carryFromCut += temp;
				j--;
			}
			ri++; //increment according to index unless trailing Zeroes were removed
		}
		///end fix fix
	}

	int32_t resultIndex = 0; //current position in the result

	//index is the currently calculated coefficient of the result

	result->digit[0] = carryFromCut;
	for (uint32_t index = toCut; index < resultUsedMax; index++) {

		//calculate all multiplications for the current index
		//cout << index << ": ";
		uint32_t i_begin = max<int32_t>(index - pSmall->used + 1, 0);
		int32_t j = index - i_begin;

		uint32_t carry = 0;

		for (uint32_t i = i_begin; j >= 0 && i < pBig->used; i++) {
			//cout << "(" << i << ", " << j << ")";

			//printf("%i + %i * %i\n",result->digit[resultIndex], pBig->digit[i],pSmall->digit[j]);
			result->digit[resultIndex] += pBig->digit[i] * pSmall->digit[j];

			//fix
			int32_t temp = result->digit[resultIndex] / 10;
			result->digit[resultIndex] -= temp * 10;
			carry += temp;

			//
			j--;
		}

		//cout << endl;

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

	int32_t tzCount = (resultUsedMax - toCut) - resultIndex; //amount of invisible trailing Zeroes

//adjust the used and shift of the result
	result->used = resultIndex + 1;

//quick fix
	if (result->digit[resultIndex] == 0) result->used--;

	result->shift = shift + op2.shift + tzCount + toCut;

}

//utility functions
void CCDecimal::constructFromString(std::string numberStr) {
	if (numberStr.find_first_not_of("-+.0987654321eE") != std::string::npos) {
		throw std::invalid_argument("String contains invalid characters.");
	}

	/*
	 * validate: max one sign; max one point; sign at the beginning,
	 * then digits, then point, if point, then more digits
	 * optionally, an exponent
	 * uses state-machine-ish behaviour, result valid if in valid_end.
	 */
	enum ValidatorStates {
		error = -1,
		start,
		sign_negative,
		sign_positive,
		point_after_sign,
		zero_after_sign,
		digit_after_sign,
		point_after_zero,
		point_after_digit,
		zero_after_point,
		digit_after_point,
		exponent_after_zero,
		exponent_after_digit,
		exponent_positive,
		exponent_negative,
		digit_after_exponent,
		valid_end
	};

	ValidatorStates validator = start;

	int64_t exponent = 0;
	int32_t exponentSign = 1;

	auto fwdChar = numberStr.begin();
	while (validator != valid_end && validator != error) {
		switch (validator) {
		case start:
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							((*fwdChar == '-') ?
									(sign_negative) :
									((*fwdChar == '+') ?
											(sign_positive) :
											((*fwdChar == '.') ?
													(point_after_sign) :
													(((*fwdChar == '0') ?
															(zero_after_sign) :
															(('1' <= *fwdChar
																	&& *fwdChar
																			<= '9') ?
																	(digit_after_sign) :
																	(error)))))));
			break;
		case sign_negative:
			//set sign
			isNegative = true;
			/* no break */
		case sign_positive:
			//next state
			//remove sign
			numberStr.erase(fwdChar);
			validator =
					(fwdChar == numberStr.end()) ?
							(error) :
							((*fwdChar == '.') ?
									(point_after_sign) :
									((*fwdChar == '0') ?
											(zero_after_sign) :
											(('1' <= *fwdChar && *fwdChar <= '9') ?
													(digit_after_sign) : (error))));
			break;
		case point_after_sign:
			//next state
			//remove point
			numberStr.erase(fwdChar);
			validator =
					(fwdChar == numberStr.end()) ?
							(error) :
							((*fwdChar == '0') ?
									(zero_after_point) :
									(('1' <= *fwdChar && *fwdChar <= '9') ?
											(digit_after_point) : (error)));
			break;
		case zero_after_sign:
			//equal to a leading zero
			//next state
			//remove leading zero
			numberStr.erase(fwdChar);
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							((*fwdChar == '.') ?
									(point_after_zero) :
									((*fwdChar == '0') ?
											(validator) :
											(('1' <= *fwdChar && *fwdChar <= '9') ?
													(digit_after_sign) :
													((*fwdChar == 'e'
															|| *fwdChar == 'E') ?
															(exponent_after_zero) :
															(error)))));
			break;
		case digit_after_sign:
			//tracking digit
			used++;
			//next state
			fwdChar++;
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							((*fwdChar == '.') ?
									(point_after_digit) :
									(('0' <= *fwdChar && *fwdChar <= '9') ?
											(validator) :
											((*fwdChar == 'e' || *fwdChar == 'E') ?
													(exponent_after_digit) :
													(error))));
			break;
		case point_after_zero:
			//next state
			//remove point
			numberStr.erase(fwdChar);
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							((*fwdChar == '0') ?
									(zero_after_point) :
									(('1' <= *fwdChar && *fwdChar <= '9') ?
											(digit_after_point) :
											((*fwdChar == 'e' || *fwdChar == 'E') ?
													(exponent_after_zero) :
													(error))));
			break;
		case point_after_digit:
			//next state
			//remove point
			numberStr.erase(fwdChar);
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							(('0' <= *fwdChar && *fwdChar <= '9') ?
									(digit_after_point) :
									((*fwdChar == 'e' || *fwdChar == 'E') ?
											(exponent_after_digit) : (error)));
			break;
		case zero_after_point:
			//tracking shift

			if (shift < -std::numeric_limits<int32_t>::max()) {
				throw std::overflow_error("Exponent too large");
			}
			shift--;
			//next state
			//remove leading zero
			numberStr.erase(fwdChar);
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							((*fwdChar == '0') ?
									(validator) :
									(('1' <= *fwdChar && *fwdChar <= '9') ?
											(digit_after_point) :
											((*fwdChar == 'e' || *fwdChar == 'E') ?
													(exponent_after_zero) :
													(error))));
			break;
		case digit_after_point:
			//tracking shift

			if (shift < -std::numeric_limits<int32_t>::max()) {
				throw std::overflow_error("Exponent too large");
			}
			shift--;
			//tracking digit
			used++;
			//next state
			fwdChar++;
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							(('0' <= *fwdChar && *fwdChar <= '9') ?
									(validator) :
									((*fwdChar == 'e' || *fwdChar == 'E') ?
											(exponent_after_digit) : (error)));
			break;
		case exponent_after_zero:
			//next state
			numberStr.erase(fwdChar, numberStr.end());
			validator = valid_end;
			break;
		case exponent_after_digit:
			//next state
			numberStr.erase(fwdChar); //remove so not present in following processing
			validator =
					(fwdChar == numberStr.end()) ?
							(error) :
							((*fwdChar == '+') ?
									(exponent_positive) :
									((*fwdChar == '-') ?
											(exponent_negative) :
											(('0' <= *fwdChar && *fwdChar <= '9') ?
													(digit_after_exponent) :
													(error))));
			break;
		case exponent_negative:
			exponentSign = -1;
			/* no break */
		case exponent_positive:
			//next state
			numberStr.erase(fwdChar); //remove so not present in following processing
			validator =
					(fwdChar == numberStr.end()) ?
							(error) :
							(('0' <= *fwdChar && *fwdChar <= '9') ?
									(digit_after_exponent) : (error));
			break;
		case digit_after_exponent:
			//check if multiply would fail
			if (exponent > std::numeric_limits<int64_t>::max() / 10) {
				throw std::overflow_error("Exponent out of range.");
			}
			//basically shift all current digits
			exponent *= 10;

			//check if add would fail
			if (exponent
					> std::numeric_limits<int64_t>::max() - (*fwdChar - '0')) {
				throw std::overflow_error("Exponent out of range.");
			}

			//add current digit in
			exponent += *fwdChar - '0';

			//next state
			numberStr.erase(fwdChar); //remove so not present in following processing
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							(('0' <= *fwdChar && *fwdChar <= '9') ?
									(validator) : (error));
			break;
		case error:
			fwdChar++;
			break;
		default:
			std::cerr << "FSM in default. This should not happen." << std::endl;
		}
	}

	if (validator != valid_end) {
		throw std::invalid_argument("Invalid number.");
	}

	/*
	 * trim number
	 */
	//remove trailing zeroes before cut
	auto revChar = numberStr.rbegin();
	while (revChar != numberStr.rend() && *revChar == '0') {
		shift++;
		used--;
		revChar++;
	}
	if (numberStr.empty()) {
		shift = 0;
		used = 0;
		return;
	}

	/*
	 * add present exponent in (if not applicable, is zero)
	 */
	//add sign to exponent
	exponent *= exponentSign;

	//check for overflows
	int64_t int32max = std::numeric_limits<int32_t>::max();
	if (shift <= 0) {
		if (exponent < 0
				&& exponent < -int32max - static_cast<int64_t>(shift)) {
			throw std::overflow_error("Exponent out of range.");
		}
		if (exponent > int32max
				&& exponent > int32max - static_cast<int64_t>(shift)) {
			throw std::overflow_error("Exponent out of range.");
		}
	}
	if (shift > 0) {
		if (exponent > 0 && exponent > int32max - static_cast<int64_t>(shift)) {
			throw std::overflow_error("Exponent out of range.");
		}
		if (exponent < -int32max
				&& exponent < -int32max - static_cast<int64_t>(shift)) {
			throw std::overflow_error("Exponent out of range.");
		}
	}
	//add exponent in
	shift += static_cast<int32_t>(exponent);

	/*
	 * check for fit in type
	 */
	if (used > MAX) {
		int32_t cutOffset = 0;

		int32_t digitsToCut = used - MAX;
		int32_t digitsToSpare = -*pPrecision - shift;

		if (digitsToCut > digitsToSpare) {
			throw std::overflow_error(
					"Digits to store surpassing precision limit");
		}
		cutOffset = digitsToCut;
		used -= cutOffset;
		shift += cutOffset;
		revChar += cutOffset;

		//check for new traling zeroes
		while (revChar != numberStr.rend() && *revChar == '0') {
			shift++;
			used--;
			revChar++;
		}
	}

	/*
	 * copy digits into array
	 */
	int32_t i = 0;
	while (revChar != numberStr.rend() && i <= MAX) {
		digit[i] = ((*revChar) - '0');
		i++;
		revChar++;
	}
}

void CCDecimal::round(CCDecimal* pDec, uint32_t precOut) {

	//skip rounding, if precision less than precOut (implicit check: shift < 0)
	if ((int32_t) precOut >= -pDec->shift) return;

	//calculate index that indicates rounding up or down
	int32_t roundIndex = -pDec->shift - (int32_t) precOut - 1;
	int32_t validIndex = roundIndex + 1;

	//fix

	if (validIndex >= (int32_t) pDec->used) {
		if (roundIndex >= (int32_t) pDec->used || pDec->digit[roundIndex] < 5) {
			pDec->used = 0;
			pDec->shift = 0;
			return;
		}

		pDec->digit[0] = 1;
		pDec->used = 1;
		pDec->shift = -precOut;
		return;
	}
	//fix end

	if (pDec->digit[roundIndex] >= 5) { //round up

		while (pDec->digit[validIndex] == 9) { //propagate carry
			validIndex++;
		}

		//apply carry
		pDec->digit[validIndex]++;

		if (pDec->digit[pDec->used] > 0) { //carry into unoccupied space
			pDec->used++;
		}
	}

	//remove trailing zeroes
	while (pDec->digit[validIndex] == 0) {
		validIndex++;
	}

	//shift digits [used-1 : validIndex] to the right
	for (int32_t i = validIndex; i < (int32_t) pDec->used; i++) {
		pDec->digit[i - validIndex] = pDec->digit[i];
		pDec->digit[i] = 0;
	}
	for (int32_t i = validIndex - 1; i >= (int32_t) pDec->used - validIndex; i--) {
		pDec->digit[i] = 0;
	}

	//adjust used and shift
	pDec->used -= validIndex;
	pDec->shift += validIndex;

}

string CCDecimal::toString() const {

	//zero case
	if (used == 0) return "0";

	//create a copy to round without changing the original
	CCDecimal copy(*this);
	CCDecimal::round(&copy, *pPrecision - 1);

	string result = "";

	//sign
	if (isNegative) result = "-";

	//at least one zero before dp
	if ((int32_t) copy.used <= -copy.shift) {
		result += "0";
	}

	//digits before dp
	for (int32_t i = copy.used - 1; i >= max<int32_t>(-copy.shift, 0); i--) {
		result += (char) (copy.digit[i] + 48);
	}

	//dp
	if (copy.shift < 0) result += ".";

	//trailing zeroes
	for (int32_t i = copy.used; i < -copy.shift; i++) {
		result += "0";
	}

	//digits after dp
	for (int32_t i = min<int32_t>(-copy.shift, copy.used) - 1; i >= 0; i--) {
		result += (char) (copy.digit[i] + 48);
	}

	//trailing zeroes
	for (int32_t i = copy.shift; i > 0; i--) {
		result += "0";
	}

	return result;

}

bool CCDecimal::magnitudeLessThan(const CCDecimal& op2) const {

	if ((int32_t) used + shift > (int32_t) op2.used + op2.shift) return false;

	if ((int32_t) used + shift < (int32_t) op2.used + op2.shift) return true;

	int32_t i = (int32_t) used - 1;
	int32_t j = (int32_t) op2.used - 1;

	while (i >= 0 && j >= 0) {
		if (digit[i] > op2.digit[j]) return false;
		if (digit[i] < op2.digit[j]) return true;
		i--;
		j--;
	}
	return false;
}

//arithmetic operators
CCDecimal CCDecimal::operator +(const CCDecimal& op2) const {
	CCDecimal result;

	if (isNegative == op2.isNegative) {
		add(&result, op2);
		result.isNegative = isNegative;
	}
	else {
		if (magnitudeLessThan(op2)) {
			op2.sub(&result, *this); //-3 + 10 = 7
			result.isNegative = op2.isNegative;
		}
		else {
			sub(&result, op2); //-10 + 3 = -7
			result.isNegative = isNegative;
		}
	}

	return result;
}
CCDecimal& CCDecimal::operator +=(const CCDecimal& op2) {
	*this = *this + op2;
	return *this;
}
CCDecimal CCDecimal::operator -(const CCDecimal& op2) const {
	CCDecimal result;

	if (isNegative != op2.isNegative) {
		add(&result, op2);
		result.isNegative = isNegative;
	}
	else {
		//+5 - 10 = -5
		//-5 - (-10) = 5
		if (magnitudeLessThan(op2)) {
			op2.sub(&result, *this);
			result.isNegative = !isNegative;
		}
		//+10 - 5 = 5
		//-10 - (-5) = -5
		else {
			sub(&result, op2);
			result.isNegative = isNegative;
		}
	}

	return result;
}
CCDecimal& CCDecimal::operator -=(const CCDecimal& op2) {
	*this = *this + op2;
	return *this;
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

	if (used == 0 && op2.used == 0) return true;

	//return false, if either used, shift or sign is not equal
	if (used != op2.used || shift != op2.shift || isNegative != op2.isNegative) {
		return false;
	}

	//compares each individual digit, return false as soon as a mismatch is found
	for (uint32_t i = 0; i < used; i++) {
		if (digit[i] != op2.digit[i]) return false;
	}

	//return true if neither of the above checks fails
	return true;
}
