/*
 * CCString.cpp
 *
 *  Created on: 21.01.2019
 *      Author: jan
 */

/**
 * @file    CCString.cpp
 *
 * @brief   Implementierung von CCString, CCString::SplitIterator und anderen zusammenhängenden Funktionen.
 *
 * @author  Jan Alexander Stiehl
 */

#include "CCString.h"

#include <iomanip>
#include <limits>
#include <locale>
#include <regex>
#include <sstream>
#include <stdexcept>

//constructors for textual types
CCString::CCString(const std::string& str) :
		internalStr(str) {
}
CCString::CCString(const char* cstr) :
		CCString(std::string(cstr)) {
}
CCString::CCString(char c) :
		CCString(std::string() = c) {
}

//constructor for boolean type
CCString::CCString(bool b) :
		CCString() {
	if (b) {
		internalStr = std::use_facet<std::numpunct<char>>(std::locale()).truename();
	}
	else {
		internalStr = std::use_facet<std::numpunct<char>>(std::locale()).falsename();
	}
}

//constructors for numeric types:
CCString::CCString(int64_t number) :
		CCString() {
	internalStr = std::to_string(number);
}
CCString::CCString(int32_t number) :
		CCString(static_cast<int64_t>(number)) {
}
CCString::CCString(int16_t number) :
		CCString(static_cast<int64_t>(number)) {
}

CCString::CCString(uint64_t number) :
		CCString() {
	internalStr = std::to_string(number);
}
CCString::CCString(uint32_t number) :
		CCString(static_cast<uint64_t>(number)) {
}
CCString::CCString(uint16_t number) :
		CCString(static_cast<uint64_t>(number)) {
}

CCString::CCString(long double number, bool hiPrec) :
		CCString() {
	if (hiPrec) {
		std::stringstream stringStream;
		stringStream << std::setprecision(std::numeric_limits<long double>::digits10) << number;
		internalStr = stringStream.str();
	}
	else {
		internalStr = std::to_string(number);
	}
}
CCString::CCString(double number, bool hiPrec) :
		CCString(static_cast<long double>(number), hiPrec) {
}
CCString::CCString(float number, bool hiPrec) :
		CCString(static_cast<long double>(number), hiPrec) {
}

CCString::CCString(const CCDecimal& number, int32_t sigDigits) {
	internalStr = number.toString(sigDigits, true);
}

CCString::CCString(const CCDecimal& number) :
		CCString() {
	internalStr = number.toString();
}

CCString::~CCString() = default;

/* ######################################### */

std::string CCString::toString() const {
	return internalStr;
}
CCString::operator const char*() {
	return internalStr.c_str();
}

CCString::operator std::string() {
	return internalStr;
}

std::ostream& operator<<(std::ostream& os, const CCString& ccstr) {
	os << ccstr.internalStr;
	return os;
}
std::istream& operator>>(std::istream& in, CCString& ccstr) {
	in >> ccstr.internalStr;
	return in;
}

size_t CCString::length() const {
	return internalStr.length();
}

char& CCString::at(size_t index) {
	try {
		return internalStr.at(index);
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Index pointing outside of the string.");
	}

}
const char& CCString::at(size_t index) const {
	try {
		return internalStr.at(index);
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Index pointing outside of the string.");
	}

}

char& CCString::operator[](size_t index) {
	return at(index);
}
const char& CCString::operator[](size_t index) const {
	return at(index);
}

bool CCString::operator==(const CCString& other) const {
	return internalStr == other.internalStr;
}
bool CCString::operator<(const CCString& other) const {
	return internalStr < other.internalStr;
}

CCString& CCString::append(const CCString& ccStr) {

	try {
		internalStr.append(ccStr.internalStr);
		return *this;
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::append(const std::string& str) {
	try {
		internalStr.append(str);
		return *this;
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::append(const char* cstr) {
	try {
		internalStr.append(cstr);
		return *this;
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::append(char c) {
	try {
		internalStr.push_back(c);
		return *this;
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::append(bool b) {
	return append(CCString(b));
}
CCString& CCString::append(int16_t i) {
	return append(CCString(i));
}
CCString& CCString::append(int32_t i) {
	return append(CCString(i));
}
CCString& CCString::append(int64_t i) {
	return append(CCString(i));
}
CCString& CCString::append(uint16_t i) {
	return append(CCString(i));
}
CCString& CCString::append(uint32_t i) {
	return append(CCString(i));
}
CCString& CCString::append(uint64_t i) {
	return append(CCString(i));
}
CCString& CCString::append(float f, bool hiPrec) {
	return append(CCString(f, hiPrec));
}
CCString& CCString::append(double d, bool hiPrec) {
	return append(CCString(d, hiPrec));
}
CCString& CCString::append(long double d, bool hiPrec) {
	return append(CCString(d, hiPrec));
}

CCString& CCString::operator<<(const CCString& ccStr) {
	return append(ccStr);
}
CCString& CCString::operator<<(const std::string& str) {
	return append(str);
}
CCString& CCString::operator<<(const char* cstr) {
	return append(cstr);
}
CCString& CCString::operator<<(char c) {
	return append(c);
}
CCString& CCString::operator<<(bool b) {
	return append(CCString(b));
}
CCString& CCString::operator<<(int16_t i) {
	return append(CCString(i));
}
CCString& CCString::operator<<(int32_t i) {
	return append(CCString(i));
}
CCString& CCString::operator<<(int64_t i) {
	return append(CCString(i));
}
CCString& CCString::operator<<(uint16_t i) {
	return append(CCString(i));
}
CCString& CCString::operator<<(uint32_t i) {
	return append(CCString(i));
}
CCString& CCString::operator<<(uint64_t i) {
	return append(CCString(i));
}
CCString& CCString::operator<<(float f) {
	return append(CCString(f));
}
CCString& CCString::operator<<(double d) {
	return append(CCString(d));
}
CCString& CCString::operator<<(long double d) {
	return append(CCString(d));
}

CCString& CCString::operator+=(const CCString& ccStr) {
	return append(ccStr);
}
CCString& CCString::operator+=(const std::string& str) {
	return append(str);
}
CCString& CCString::operator+=(const char* cstr) {
	return append(cstr);
}
CCString& CCString::operator+=(char c) {
	return append(c);
}
CCString& CCString::operator+=(bool b) {
	return append(CCString(b));
}
CCString& CCString::operator+=(int16_t i) {
	return append(CCString(i));
}
CCString& CCString::operator+=(int32_t i) {
	return append(CCString(i));
}
CCString& CCString::operator+=(int64_t i) {
	return append(CCString(i));
}
CCString& CCString::operator+=(uint16_t i) {
	return append(CCString(i));
}
CCString& CCString::operator+=(uint32_t i) {
	return append(CCString(i));
}
CCString& CCString::operator+=(uint64_t i) {
	return append(CCString(i));
}
CCString& CCString::operator+=(float f) {
	return append(CCString(f));
}
CCString& CCString::operator+=(double d) {
	return append(CCString(d));
}
CCString& CCString::operator+=(long double d) {
	return append(CCString(d));
}

CCString operator+(const CCString& lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const CCString& lhs, const char* rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const char* lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const CCString& lhs, const std::string& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const std::string& lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const CCString& lhs, char rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(char lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}

CCString operator+(const CCString& lhs, bool rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(bool lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const CCString& lhs, int16_t rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(int16_t lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const CCString& lhs, int32_t rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(int32_t lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const CCString& lhs, int64_t rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(int64_t lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(float lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}

CCString operator+(const CCString& lhs, double rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(double lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(const CCString& lhs, long double rhs) {
	return CCString(lhs) += rhs;
}
CCString operator+(long double lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}

CCString& CCString::replaceAt(size_t pos, const CCString& ccStr) {
	try {
		internalStr.replace(pos, ccStr.length(), ccStr.internalStr);
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::replaceAt(size_t pos, const std::string& str) {
	try {
		internalStr.replace(pos, str.length(), str);
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

	throw std::exception();
}

CCString& CCString::replaceAt(size_t pos, const char* cstr) {
	try {
		internalStr.replace(pos, std::char_traits<char>::length(cstr), cstr);
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::replaceAt(size_t pos, char c) {
	try {
		internalStr.at(pos) = c;
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}

}

CCString& CCString::insert(size_t pos, const CCString& ccStr) {
	try {
		internalStr.insert(pos, ccStr.internalStr);
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::insert(size_t pos, const std::string& str) {
	try {
		internalStr.insert(pos, str);
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::insert(size_t pos, const char* cstr) {
	try {
		internalStr.insert(pos, cstr);
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}
CCString& CCString::insert(size_t pos, char c) {
	try {
		internalStr.insert(pos, 1, c);
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}
	catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}

CCString& CCString::insert(size_t pos, bool b) {
	return insert(pos, CCString(b));
}

CCString& CCString::insert(size_t pos, int16_t i) {
	return insert(pos, CCString(i));
}
CCString& CCString::insert(size_t pos, int32_t i) {
	return insert(pos, CCString(i));
}
CCString& CCString::insert(size_t pos, int64_t i) {
	return insert(pos, CCString(i));
}
CCString& CCString::insert(size_t pos, uint16_t i) {
	return insert(pos, CCString(i));
}
CCString& CCString::insert(size_t pos, uint32_t i) {
	return insert(pos, CCString(i));
}
CCString& CCString::insert(size_t pos, uint64_t i) {
	return insert(pos, CCString(i));
}
CCString& CCString::insert(size_t pos, float f, bool hiPrec) {
	return insert(pos, CCString(f, hiPrec));
}
CCString& CCString::insert(size_t pos, double d, bool hiPrec) {
	return insert(pos, CCString(d, hiPrec));
}
CCString& CCString::insert(size_t pos, long double d, bool hiPrec) {
	return insert(pos, CCString(d, hiPrec));
}

CCString& CCString::erase(size_t pos, size_t length) {
	try {
		internalStr.erase(pos, length);
		return *this;
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}

}

CCString& CCString::trim() {
	for (auto it = internalStr.begin();
			it != internalStr.end() && !std::isgraph(*it, std::locale()); internalStr.erase(it)) {
	}
	for (auto rit = internalStr.rbegin();
			rit != internalStr.rend() && !std::isgraph(*rit, std::locale()); rit++) {
		internalStr.erase(rit.base());
	}
	return *this;
}
//CCString& CCString::toUpper() {
//	for (char& c : internalStr) {
//		c = std::use_facet<std::ctype<char>>(std::locale()).toupper(
//				static_cast<int>(c));
//	}
//	return *this;
//}
//CCString& CCString::toLower() {
//	for (char& c : internalStr) {
//		c = std::use_facet<std::ctype<char>>(std::locale()).tolower(
//				static_cast<int>(c));
//	}
//	return *this;
//}

CCString CCString::subString(size_t pos, size_t length) const {
	try {
		return CCString(internalStr.substr(pos, length));
	}
	catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	}
	catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	}

}

size_t CCString::find(const CCString& ccstr, size_t pos) const {
	return internalStr.find(ccstr.internalStr, pos);
}
size_t CCString::find(const std::string& str, size_t pos) const {
	return internalStr.find(str, pos);
}
size_t CCString::find(const char* cstr, size_t pos) const {
	return internalStr.find(cstr, pos);
}
size_t CCString::find(char c, size_t pos) const {
	return internalStr.find(c, pos);
}

size_t CCString::findLast(const CCString& ccstr, size_t pos) const {
	return internalStr.rfind(ccstr.internalStr, pos);
}
size_t CCString::findLast(const std::string& str, size_t pos) const {
	return internalStr.rfind(str, pos);
}
size_t CCString::findLast(const char* cstr, size_t pos) const {
	return internalStr.rfind(cstr, pos);
}
size_t CCString::findLast(char c, size_t pos) const {
	return internalStr.rfind(c, pos);
}

bool CCString::isMatch(const CCString& regex) const {
	std::regex re(regex.internalStr);
	return std::regex_match(internalStr, re);
}
bool CCString::containsMatch(const CCString& regex) const {
	std::regex re(regex.internalStr);
	return std::regex_search(internalStr, re);
}
CCString CCString::getMatch(const CCString& regex) const {
	std::regex re(regex.internalStr);
	std::smatch matches;
	std::regex_search(internalStr, matches, re);
	while (!matches.ready())
		;
	if (matches.empty()) {
		return CCString();
	}
	if (!matches[0].matched) {
		return CCString();
	}
	return CCString(std::string(matches[0]));
}
CCString CCString::replaceAll(const CCString& regex, const CCString& replacement) const {
	std::regex re(regex.internalStr);
	return std::regex_replace(internalStr, re, replacement.internalStr);
}
CCString CCString::replaceFirst(const CCString& regex, const CCString& replacement) const {
	std::regex re(regex.internalStr);
	std::smatch matches;
	std::regex_search(internalStr, re);

	auto startOfString = matches.prefix().first;
	auto endOfFirstMatch = matches[0].second;
	auto endOfAll = matches.suffix().second;

	std::string resultFirstHalf;
	std::regex_replace(std::back_inserter(resultFirstHalf), startOfString, endOfFirstMatch, re,
			replacement.internalStr);
	resultFirstHalf += std::string(endOfFirstMatch, endOfAll);
	return regex;
}

CCString::SplitIterator CCString::splitBegin(const CCString& regex) const {
	return SplitIterator(*this, regex);
}

CCString::SplitIterator CCString::splitEnd() const {
	return SplitIterator(*this);
}

CCString::SplitIterator::SplitIterator(const SplitIterator& orig) :
		originString(orig.originString), //
		separatorRegex(orig.separatorRegex), //
		currentSplit(new CCString(*orig.currentSplit)), //
		currentRemainder(orig.currentRemainder), //
		hadMatch(orig.hadMatch), //
		isFinished(orig.isFinished) //
{
}

CCString::SplitIterator::SplitIterator(const CCString& origin, const CCString& regex) :
		originString(&origin), //
		separatorRegex(regex.internalStr), //
		currentSplit(new CCString()), //
		currentRemainder(origin.internalStr) //
{
	doSplit();
}

CCString::SplitIterator::SplitIterator(const CCString& origin) :
		originString(&origin), //
		currentSplit(new CCString()), //
		isFinished(true) {
}

void CCString::SplitIterator::doSplit() {
	std::regex re(separatorRegex);
	std::smatch matches;
	std::regex_search(currentRemainder, matches, re);
	while (!matches.ready())
		;

	if (matches.empty() || !matches[0].matched) {
		//catch closing on a separator:
		//not finished if had a previous match
		isFinished = !hadMatch;
		hadMatch = false;

		//set split to remainder
		std::unique_ptr<CCString> newSplit(new CCString(currentRemainder));
		currentSplit.swap(newSplit);
		//set remainder to empty
		currentRemainder.erase();
	}
	else {
		//mark match
		hadMatch = true;
		std::string splitStr(matches.prefix().first, matches.prefix().second);
		std::string remainderStr(matches.suffix().first, matches.suffix().second);

		std::unique_ptr<CCString> newSplit(new CCString(splitStr));
		currentSplit.swap(newSplit);

		currentRemainder = remainderStr;
	}
}

CCString::SplitIterator& CCString::SplitIterator::operator++() {
	doSplit();
	return *this;
}

CCString::SplitIterator CCString::SplitIterator::operator++(int) {
	SplitIterator copy(*this);
	doSplit();
	return copy;
}

const CCString& CCString::SplitIterator::operator*() const {
	return *currentSplit;
}

const CCString* CCString::SplitIterator::operator->() const {
	return currentSplit.get();
}

bool CCString::SplitIterator::operator==(const SplitIterator& other) const {

//Check for domain (operating on the same CCString?)
	if (originString != other.originString) return false;

//Check for past-the-end
	if (isFinished || other.isFinished) return isFinished && other.isFinished;

//check for equality on: regex and position
	if (separatorRegex == other.separatorRegex)
		if (*currentSplit == *other.currentSplit)
			if (currentRemainder == other.currentRemainder) return true;

	return false;
}
