/*
 * Decimal.cpp


 *
 *  Created on: 24.10.2018
 *      Author: marlo
 */

/** \file CCDecimal.cpp
 * A brief file description.
 *
 * A more elaborated file description.
 */

#include "CCDecimal.h"

#include "gtest/gtest.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>
#include <cstdlib>

using namespace std;

//### static attribute initialization
int32_t CCDecimal::globalPrecision = 11;

//### constructors/destructors #####################
CCDecimal::CCDecimal() noexcept {

	//precision should be the global precision, unless explicitly changed for an instance
	pPrecision = &CCDecimal::globalPrecision;

}
CCDecimal::CCDecimal(const CCDecimal& original) :
		CCDecimal() {

	*this = original;

	//ensures that pPrecision points to the local precision if the original's was explicitly changed.
	if (pPrecision == &original.localPrecision) {
		pPrecision = &localPrecision;
	}
}
CCDecimal::CCDecimal(double number) :
		CCDecimal() {
	std::stringstream stringStream;

	//convert double to string using 'stringstream' and 'setprecision' to get the highest precision available
	stringStream << std::setprecision(std::numeric_limits<double>::max_digits10) << number;

	//construct CCDecimal from the double's string representation
	constructFromString(stringStream.str());
}
CCDecimal::CCDecimal(const string& numberStr) :
		CCDecimal() {

	//construct CCDecimal from the given string representation
	constructFromString(numberStr);
}
CCDecimal::CCDecimal(const char* numberCStr) :
		CCDecimal(string(numberCStr)) {
}

//### public setter/getter #########################
int32_t CCDecimal::getPrecision() {
	return *pPrecision - 1;
}
void CCDecimal::setLocalPrecision(int32_t precision) {
	if (precision < 0) throw std::out_of_range("precision has to be positive");
	localPrecision = precision + 1;
	pPrecision = &localPrecision;
}
int32_t CCDecimal::getGlobalPrecision() {
	return CCDecimal::globalPrecision - 1;
}
void CCDecimal::setGlobalPrecision(int32_t precision) {
	if (precision < 0) throw std::out_of_range("precision has to be positive");
	CCDecimal::globalPrecision = precision + 1;
}

//### arithmetic operators #########################
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

			//-5 +  10  = 5
			// 5 +(-10) = -5
		}
		else {
			sub(&result, op2); //-10 + 3 = -7
			result.isNegative = isNegative;

			//-10 +   5  = -5
			// 10 + (-5) =  5
		}
	}

	if (result.digit[MAX] > 0) {
		throw std::overflow_error("overflow_error");
	}

	return result;
}
CCDecimal CCDecimal::operator -(const CCDecimal& op2) const {
	CCDecimal result;

	if (isNegative != op2.isNegative) {
		add(&result, op2);
		result.isNegative = isNegative;
	}
	else {
		if (magnitudeLessThan(op2)) {
			op2.sub(&result, *this);
			result.isNegative = !isNegative;
		}
		else {
			sub(&result, op2);
			result.isNegative = isNegative;
		}
	}

	if (result.digit[MAX] > 0) {
		throw std::overflow_error("overflow_error");
	}

	return result;
}
CCDecimal CCDecimal::operator *(const CCDecimal& op2) const {
	CCDecimal result;
	result.isNegative = isNegative != op2.isNegative;
	mult(&result, op2);

	if (result.digit[MAX] > 0) {
		throw std::overflow_error("overflow_error");
	}

	return result;
}
CCDecimal CCDecimal::operator /(const CCDecimal& op2) const {
	CCDecimal result;
	result.isNegative = isNegative != op2.isNegative;
	div(&result, op2);

	if (result.digit[MAX] > 0) {
		throw std::overflow_error("overflow_error");
	}

	return result;
}
CCDecimal CCDecimal::operator %(const CCDecimal& op2) const {
	CCDecimal result;
	result.isNegative = isNegative != op2.isNegative;
	mod(&result, op2);

	return result;
}

CCDecimal& CCDecimal::operator +=(const CCDecimal& op2) {
	*this = *this + op2;
	return *this;
}
CCDecimal& CCDecimal::operator -=(const CCDecimal& op2) {
	*this = *this - op2;
	return *this;
}
CCDecimal& CCDecimal::operator *=(const CCDecimal& op2) {

	*this = *this * op2;
	return *this;
}
CCDecimal& CCDecimal::operator /=(const CCDecimal& op2) {
	*this = *this / op2;
	return *this;
}
CCDecimal& CCDecimal::operator %=(const CCDecimal& op2) {
	*this = *this % op2;
	return *this;
}

CCDecimal& CCDecimal::operator++() {
	*this += CCDecimal(1);
	return *this;
}
CCDecimal& CCDecimal::operator--() {
	*this -= CCDecimal(1);
	return *this;
}
CCDecimal CCDecimal::operator++(int) {
	CCDecimal copy(*this);
	*this += CCDecimal(1);
	return copy;
}
CCDecimal CCDecimal::operator--(int) {
	CCDecimal copy(*this);
	*this -= CCDecimal(1);
	return copy;
}

//### comparison operators #########################
bool CCDecimal::operator ==(const CCDecimal& op2) const {

	if (used == 0 && op2.used == 0) return true;

	//round
	CCDecimal dec1(*this);
	CCDecimal dec2(op2);
	if (dec1.used >= MAX) CCDecimal::round(&dec1, max<int>(-shift - 1, *pPrecision - 1));
	if (dec2.used >= MAX) CCDecimal::round(&dec2, max<int>(-op2.shift - 1, *op2.pPrecision - 1));

	//return false, if either used, shift or sign is not equal
	if (dec1.used != dec2.used || dec1.shift != dec2.shift || dec1.isNegative != dec2.isNegative) {
		return false;
	}

	//compares each individual digit, return false as soon as a mismatch is found
	for (uint32_t i = 0; i < dec1.used; i++) {
		if (dec1.digit[i] != dec2.digit[i]) return false;
	}

	//return true if neither of the above checks fails
	return true;
}
bool CCDecimal::operator !=(const CCDecimal& op2) const {
	return !(*this == op2);
}
bool CCDecimal::operator <(const CCDecimal& op2) const {

	//round
	CCDecimal dec1(*this);
	CCDecimal dec2(op2);
	if (dec1.used >= MAX) CCDecimal::round(&dec1, max<int>(-shift - 1, *pPrecision - 1));
	if (dec2.used >= MAX) CCDecimal::round(&dec2, max<int>(-op2.shift - 1, *op2.pPrecision - 1));

	if (dec1.used == 0 && dec2.used == 0) return false;
	if (dec1.isNegative) {
		if (!dec2.isNegative || dec2.magnitudeLessThan(*this)) return true;
	}
	else if (!dec2.isNegative && dec1.magnitudeLessThan(dec2)) {
		return true;
	}
	return false;
}
bool CCDecimal::operator >(const CCDecimal& op2) const {

	//round
	CCDecimal dec1(*this);
	CCDecimal dec2(op2);
	if (dec1.used >= MAX) CCDecimal::round(&dec1, max<int>(-shift - 1, *pPrecision - 1));
	if (dec2.used >= MAX) CCDecimal::round(&dec2, max<int>(-op2.shift - 1, *op2.pPrecision - 1));

	if (dec1.used == 0 && dec2.used == 0) return false;
	if (dec2.isNegative) {
		if (!dec1.isNegative || dec1.magnitudeLessThan(dec2)) return true;
	}
	else if (!dec1.isNegative && dec2.magnitudeLessThan(dec1)) {
		return true;
	}
	return false;
}

//### stream operator ##############################
ostream& operator <<(ostream& os, const CCDecimal& dec) {

	std::ios_base::fmtflags f = os.flags();
	if ((f & os.scientific) == os.scientific) {
		return os << dec.toString(os.precision(), true);
	}
	return os << dec.toString(os.precision(), false);
}

//### conversion functions #########################
void CCDecimal::round(CCDecimal* pDec, int32_t precOut) {

	if (precOut < 0) {
		throw std::out_of_range("The output precision has to be positive");
	}

//skip rounding, if precision less than precOut (implicit check: shift < 0)
	if (precOut >= -pDec->shift) return;

//calculate index that indicates rounding up or down
	int32_t roundIndex = -pDec->shift - precOut - 1;
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

		pDec->digit[MAX] = 0;
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
string CCDecimal::toString(int32_t precOut, bool scientific) const {

	//zero case

	//create a copy to round without changing the original
	CCDecimal copy(*this);

	int32_t exp_sci = 0;
	string result = "";

	if (used == 0) {
		result = "0";
	}
	else {
		if (scientific) {
			exp_sci = copy.shift + (int32_t) copy.used - 1;
			copy.shift -= exp_sci;
		}

		CCDecimal::round(&copy, precOut);

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
	}

	//scientific exponent
	if (scientific) {
		if (copy.shift == 0 && precOut > 0) result += ".";
		for (int32_t i = -copy.shift; i < precOut; i++) {
			result += "0";
		}

		result += "e";
		result += exp_sci < 0 ? "-" : "+";
		if (abs(exp_sci) < 100) result += "0";
		if (abs(exp_sci) < 10) result += "0";
		result += std::to_string(abs(exp_sci));
	}

	return result;

}
string CCDecimal::toString(bool scientific) const {
	return toString(*pPrecision - 1, scientific);
}
double CCDecimal::toDouble() const {

	double result;
	try {
		//construct double from CCDecimals string representation
		result = std::stod(toString(used - 1, true));
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("CCDecimal value exceeding range of type double.");
	}
	catch (std::invalid_argument& e) {
		throw;
	}
	return result;
}

//### utility funcions ############################
void CCDecimal::add(CCDecimal* result, const CCDecimal& op2) const {

	//ASSERT_TRUE((*result == CCDecimal()));

	//determine the most and the least precise decimal
	const CCDecimal* pMostPrec = this;
	const CCDecimal* pLeastPrec = &op2;
	if (op2.shift < shift) {
		pMostPrec = &op2;
		pLeastPrec = this;
	}

	int32_t shift_result = pMostPrec->shift; //dpPos_min
	int32_t shift_delta = pLeastPrec->shift - pMostPrec->shift; //dpPos_gap

	//dertermine merged size
	int32_t used_result = max(pMostPrec->used, pLeastPrec->used + shift_delta);

	//calculate amount of expendable digits (digits to spend)
	int32_t digToSpend = -*pPrecision - shift_result;

	//calculate amount of digits needed to be cut to meet size requirements (digits to cut)
	int32_t digToCut = max(used_result - MAX, 0);

	//check, if cutting of decimal places is necessary
	if (digToCut > 0) {

		if (digToSpend < digToCut) { //overflow

			result->digit[MAX] = 1;
			return; //throw std::overflow_error("Result is too large to store in Decimal. Keep values in range or reduce precision!");
		}

		//cut decimal places
		used_result = MAX;
		shift_result += digToCut;

		//ASSERT_TRUE((digToCut <= shift_delta));
		shift_delta -= digToCut;
	}

	//add most precise operand to the result
	for (uint32_t i = digToCut; i < pMostPrec->used; i++) {
		result->digit[i - digToCut] = pMostPrec->digit[i];
	}

	//round fix
	if (digToCut > 0 && pMostPrec->digit[digToCut - 1] >= 5) {
		result->digit[0]++;
	}
	//fix round

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
	if (result->digit[used_result] > 0) used_result++;

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
		return;//throw std::overflow_error("Result is too large to store in Decimal. Keep values in range or reduce precision!");
	}

	result->used = used_result;
	result->shift = shift_result;
}

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

		if (toCut > toSpend + 1) { //overflows by more than one digit
			result->digit[MAX] = 1;
			return; // throw std::overflow_error("Can not cut result to minimal precision!");
		}

		// bet that one will be reduced
		toCut--; //change: cut one less, if you can spend one
	}

	//Cut one less?toCut > 0
	bool msdIsZero = false;

	//can I spend digits?
	if (toSpend >= toCut + 1) { //&& tz_body == 0 && tz_head == 0

	//Compute in advance if the MSD will become zero.
		if (digit[used - 1] == 1) { //HINT: Where has to be a head, if cutting is necessary.
			bool carryFromBody = false;

			//filter leading zeroes
			int32_t i = (int32_t) used - 2;
			if (i >= 0) {

				//filter leading zeroes
				int32_t end = (int32_t) opSmall.used + shift_delta;
				for (; i >= 0 && i >= end && digit[i] == 0; i--) {
				}

				//filter equal digits which in consequence will be zero
				if (i < (int32_t) used - 2) {
					for (; i >= 0 && digit[i] == opSmall.digit[i - shift_delta]; i--) {
					}
					if (digit[i] < opSmall.digit[i - shift_delta]) { //carry from subtraction
						carryFromBody = true;
					}
				}
			}
			if (i < 0 || carryFromBody) { //tail follows directly after head
				msdIsZero = true;

				printf("MSD will become zero!\n");
			}
		}
		if (msdIsZero == false) toCut++;

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
		for (;
				search_pos >= (int32_t) opSmall.used && search_pos < (int32_t) used
						&& digit[search_pos - tail_length] == 0; search_pos++) {
			tz_head++;
		}
	}

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
//	if (result->digit[MAX] > 0) {
//		throw std::overflow_error("overflow - capacity");
//	}

//adjust used and shift
	result->used = result_pos + 1;
	result->shift = min<int32_t>(shift, opSmall.shift) + toCut + tz_body + tz_head;

	if (result_pos < 0) {
		result->shift = 0;
	}
}

void CCDecimal::mult(CCDecimal* result, const CCDecimal& op2) const {

	if (used == 0 || op2.used == 0) return;

	//determine the decimal numbers with most and least digits
	const CCDecimal* pSmall = this;
	const CCDecimal* pBig = &op2;
	if (op2.used < used) {
		pSmall = &op2;
		pBig = this;
	}

	bool tzFlag = true; //flag is set while a result of 0 at an index means a trailing zero
	uint32_t resultUsedMax = pSmall->used + pBig->used - 1; //best case

	int32_t shift_sum = pSmall->shift + pBig->shift;
	int32_t toSpend = -shift_sum - *pPrecision;
	int32_t toCut = 0;

	int32_t carryFromCut = 0;
	if (resultUsedMax > MAX) {
		toCut = (int32_t) resultUsedMax - MAX;

		if (toSpend < toCut) {
			result->digit[MAX] = 1;
			return; //throw std::overflow_error("can not cut");
		}

		toSpend -= toCut; //spend the digits

		//calculate carry from the cut
		for (uint32_t index = 0; (int32_t) index < toCut; index++) {

			uint32_t i_begin = max<int32_t>(index - pSmall->used + 1, 0);
			int32_t j = index - i_begin;

			for (uint32_t i = i_begin; j >= 0 && i < pBig->used; i++) {
				carryFromCut += pBig->digit[i] * pSmall->digit[j];
				j--;
			}

			carryFromCut /= 10;
		}
	}

	uint32_t carry = carryFromCut; //propagate carry from cut
	int32_t resultIndex = 0; //current position in the result

	//index is the currently calculated coefficient of the result
	for (uint32_t index = toCut; index < resultUsedMax; index++) {

		//calculate all multiplications for the current index
		uint32_t i_begin = max<int32_t>(index - pSmall->used + 1, 0);
		int32_t j = index - i_begin;

		for (uint32_t i = i_begin; j >= 0 && i < pBig->used; i++) {
			carry += pBig->digit[i] * pSmall->digit[j];
			j--;
		}

		//apply result at current index and propagate carry
		if (resultIndex <= MAX) {
			int temp = carry / 10;
			result->digit[resultIndex] = carry - temp * 10;
			carry = temp;
		}

		//remove trailing Zeroes
		if (tzFlag && (tzFlag = result->digit[resultIndex] == 0)) resultIndex--;

		if (carry > 0 && resultIndex + 1 == MAX && toSpend > 0) { //prevent carry into MSD if possible

			bool carryFromCut = result->digit[0] >= 5;

			//Shift all digits one to the right
			for (int i = 1; i <= resultIndex; i++) {
				result->digit[i - 1] = result->digit[i];
			}
			if (carryFromCut) result->digit[0]++;

			result->digit[resultIndex] = 0;
		}
		else {
			resultIndex++; //increment according to index unless trailing Zeroes were removed
		}

	}

	//adjust the used and shift of the result
	int32_t tzCount = resultUsedMax - resultIndex; //amount of invisible trailing Zeroes
	result->shift = shift + op2.shift + tzCount;
	result->used = resultIndex;

	//apply last carry
	if (carry > 0) {
		EXPECT_TRUE(resultIndex <= MAX);
		result->digit[resultIndex] = carry;
		if (carry > 0) result->used++;
	}

	//check overflow flag
//	if (result->digit[MAX] > 0)
//		throw std::overflow_error(
//				"Result is too large to store in Decimal. Keep values in range or reduce precision!");

}

void CCDecimal::div(CCDecimal* result, const CCDecimal& divisor) const {

	//some special values
	const CCDecimal VALUE_2(2);
	const CCDecimal VALUE_0;

	if (divisor.used == 0) {
		throw std::domain_error("divide by zero");
	}

	if (this->used == 0) return;

	int result_pos = MAX - 1; //position in the result array

	CCDecimal div_arr[4]; //holds multiples of the divider (* 2^i)
	const int32_t DIV_CONST[4] = { 1, 2, 4, 8 }; //2^i
	int32_t div_max = 1;  //number of divider values computed in advance

	CCDecimal nominator(*this); //copy of the nominator

	int32_t msd_delta = divisor.shift + (int32_t) divisor.used - shift - (int32_t) used;
	div_arr[0] = divisor;
	nominator.shift += msd_delta;
	if (nominator.magnitudeLessThan(div_arr[0])) {
		nominator.shift++;
		msd_delta++;
	}

	for (int32_t i = 1; i <= 3 && (div_arr[i - 1].used < MAX || div_arr[i - 1].digit[MAX - 1] <= 4);
			i++) {
		div_arr[i] = div_arr[i - 1] * VALUE_2;
		div_max++;
	}

	cout << "msd_delta: " << msd_delta << endl;

	bool runDivision = true;

	while (runDivision && nominator != VALUE_0) {

		int temp = 0;
		while (runDivision && !nominator.magnitudeLessThan(div_arr[0])) {

			//find divisor that fits best
			int32_t div_index = 0;
			while (div_index < div_max - 1 && !nominator.magnitudeLessThan(div_arr[div_index + 1])) {
				div_index++;
			}

			//subtract divisor
			CCDecimal subResult;
			nominator.sub(&subResult, div_arr[div_index]);
			nominator = subResult;
			temp += DIV_CONST[div_index];

			if (nominator == VALUE_0) runDivision = false;
		}

		//apply temp
		if (result_pos >= 0) result->digit[result_pos] = temp;

		if (result_pos == -1 && temp >= 5) result->digit[0]++;

		if (runDivision) {
			//shift nominator
			do {
				nominator.shift++;
				result_pos--;
				if (result_pos >= 0) msd_delta++;

			} while (nominator.magnitudeLessThan(div_arr[0]) && result_pos >= -1);

			if (result_pos <= -2) {
				if (msd_delta < (int32_t) *pPrecision) {

					result->digit[MAX] = 1;
					return; //throw std::overflow_error("hfdhfdjfh");

				}

				result_pos = 0;
				runDivision = false;
			}

		}
	}

	result->shift = -msd_delta;
	result->used = MAX - result_pos;
	for (int i = result_pos; i < MAX; i++) {
		result->digit[i - result_pos] = result->digit[i];
	}
}

void CCDecimal::mod(CCDecimal* nominator, const CCDecimal& divisor) const {
	//some special values
	const CCDecimal VALUE_2(2);
	const CCDecimal VALUE_0;

	if (*this == VALUE_0) return;
	if (divisor == VALUE_0) throw std::domain_error("divide by zero");
	if (this->magnitudeLessThan(divisor)) return;

	CCDecimal div_arr[4]; //holds multiples of the divider (* 2^i)
	int32_t div_max = 1;  //number of divider values computed in advance

	*nominator = *this; //copy of the nominator

	int32_t msd_delta = divisor.shift + (int32_t) divisor.used - shift - (int32_t) used;
	div_arr[0] = divisor;
	nominator->shift += msd_delta;
	if (nominator->magnitudeLessThan(div_arr[0])) nominator->shift++;

	for (int32_t i = 1; i <= 3 && (div_arr[i - 1].used < MAX || div_arr[i - 1].digit[MAX - 1] <= 4);
			i++) {
		div_arr[i] = div_arr[i - 1] * VALUE_2;
		div_max++;
	}

	while (!nominator->magnitudeLessThan(div_arr[0])) {

		//find divisor that fits best
		int32_t div_index = 0;
		while (div_index < div_max - 1 && !nominator->magnitudeLessThan(div_arr[div_index + 1])) {
			div_index++;
		}

		//subtract divisor
		CCDecimal subResult;
		nominator->sub(&subResult, div_arr[div_index]);
		*nominator = subResult;
	}

}

void CCDecimal::constructFromString(std::string numberStr) {
	if (numberStr.find_first_not_of("-+.0987654321eE") != std::string::npos) {
		throw std::invalid_argument("String contains invalid characters.");
	}

	/*
	 * validate: max one sign; max one point; sign at the beginning,
	 * then digits, then point (if point), then more digits
	 * optionally, an exponent (sign, if any, followed by digits)
	 * uses state-machine-ish behaviour, result valid if in valid_end.
	 * Removes signs, leading zeroes (hence the separate *zero*-states),
	 * decimal points and exponents in-place, leaving only digits behind.
	 */
	enum ValidatorStates {
		error = -1,
		start,
		sign,
		point_after_sign,
		zero_after_sign,
		digit_after_sign,
		point_after_zero,
		point_after_digit,
		zero_after_point,
		digit_after_point,
		exponent_after_digit,
		exponent_sign,
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
							((*fwdChar == '-' || *fwdChar == '+') ?
									(sign) :
									((*fwdChar == '.') ?
											(point_after_sign) :
											(((*fwdChar == '0') ?
													(zero_after_sign) :
													(('1' <= *fwdChar && *fwdChar <= '9') ?
															(digit_after_sign) : (error))))));
			break;
		case sign:
			//set sign
			isNegative = *fwdChar == '-';
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
													((*fwdChar == 'e' || *fwdChar == 'E') ?
															(exponent_after_digit) : (error)))));
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
													(exponent_after_digit) : (error))));
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
													(exponent_after_digit) : (error))));
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
													(exponent_after_digit) : (error))));
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
		case exponent_after_digit:
			//next state
			numberStr.erase(fwdChar); //remove so not present in following processing
			validator =
					(fwdChar == numberStr.end()) ?
							(error) :
							((*fwdChar == '+' || *fwdChar == '-') ?
									(exponent_sign) :
									(('0' <= *fwdChar && *fwdChar <= '9') ?
											(digit_after_exponent) : (error)));
			break;
		case exponent_sign:
			exponentSign = (*fwdChar == '-') ? (-1) : (1);
			//next state
			numberStr.erase(fwdChar); //remove so not present in following processing
			validator =
					(fwdChar == numberStr.end()) ?
							(error) :
							(('0' <= *fwdChar && *fwdChar <= '9') ? (digit_after_exponent) : (error));
			break;
		case digit_after_exponent:
			//check if multiply would fail
			if (exponent > std::numeric_limits<int64_t>::max() / 10) {
				throw std::overflow_error("Exponent out of range.");
			}
			//basically shift all current digits
			exponent *= 10;

			//check if add would fail
			if (exponent > std::numeric_limits<int64_t>::max() - (*fwdChar - '0')) {
				throw std::overflow_error("Exponent out of range.");
			}

			//add current digit in
			exponent += *fwdChar - '0';

			//next state
			numberStr.erase(fwdChar); //remove so not present in following processing
			validator =
					(fwdChar == numberStr.end()) ?
							(valid_end) :
							(('0' <= *fwdChar && *fwdChar <= '9') ? (validator) : (error));
			break;
		default:
			std::cerr << "FSM in default. This should not happen." << std::endl;
		}
	}

	if (validator != valid_end) {
		throw std::invalid_argument("Invalid number.");
	}

	auto revChar = numberStr.rbegin();
	/*
	 * trim number
	 */
	//remove trailing zeroes before cut
	while (revChar != numberStr.rend() && *revChar == '0') {
		shift++;
		used--;
		revChar++;
	}

	/*
	 * add present exponent in (if not applicable, is zero)
	 */
	//add sign to exponent
	exponent *= exponentSign;

	//check for overflows
	int64_t int32max = std::numeric_limits<int32_t>::max();
	if (shift <= 0) {
		if (exponent < 0 && exponent < -int32max - static_cast<int64_t>(shift)) {
			throw std::overflow_error("Exponent out of range.");
		}
		if (exponent > int32max && exponent > int32max - static_cast<int64_t>(shift)) {
			throw std::overflow_error("Exponent out of range.");
		}
	}
	if (shift > 0) {
		if (exponent > 0 && exponent > int32max - static_cast<int64_t>(shift)) {
			throw std::overflow_error("Exponent out of range.");
		}
		if (exponent < -int32max && exponent < -int32max - static_cast<int64_t>(shift)) {
			throw std::overflow_error("Exponent out of range.");
		}
	}
	//add exponent in
	shift += static_cast<int32_t>(exponent);

	if (numberStr.empty()) {
		shift = 0;
		used = 0;
		return;
	}

	/*
	 * check for fit in type
	 */
	if (used > MAX) {
		int32_t cutOffset = 0;

		int32_t digitsToCut = used - MAX;
		int32_t digitsToSpare = -*pPrecision - shift;

		if (digitsToCut > digitsToSpare) {
			throw std::overflow_error("Digits to store surpassing precision limit");
		}
		cutOffset = digitsToCut;
		used -= cutOffset;
		shift += cutOffset;
		revChar += cutOffset;

		//round after cut
		if ('5' <= revChar[-1] && revChar[-1] <= '9') {
			//would carry?
			if (*revChar < '9') {
				++(*revChar);
			}
			else {
				*revChar = '0';
				bool carry = true;
				for (int i = 1; carry; i++) {
					if (revChar[i] < '9') {
						++(revChar[i]);
						carry = false;
					}
					else {
						revChar[i] = '0';
					}
				}
			}

		}
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
	return j >= 0;
}

