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

#include "CCString.hpp"

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
CCString::CCString(const char cstr[]) :
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
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Index pointing outside of the string.");
	}
}
const char& CCString::at(size_t index) const {
	try {
		return internalStr.at(index);
	} catch (std::out_of_range& e) {
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
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		throw;
	}
}

CCString& CCString::operator<<(const CCString& ccStr) {
	return append(ccStr);
}

CCString& CCString::operator+=(const CCString& ccStr) {
	return append(ccStr);
}

CCString operator+(const CCString& lhs, const CCString& rhs) {
	return CCString(lhs) += rhs;
}

CCString& CCString::replaceAt(size_t pos, const CCString& ccStr) {
	try {
		internalStr.replace(pos, ccStr.length(), ccStr.internalStr);
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		throw;
	}
}

CCString& CCString::insert(size_t pos, const CCString& ccStr) {
	try {
		internalStr.insert(pos, ccStr.internalStr);
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		throw;
	}
}

CCString& CCString::erase(size_t pos, size_t length) {
	try {
		internalStr.erase(pos, length);
		return *this;
	} catch (std::out_of_range& e) {
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
		internalStr.erase(rit.base()-1);
	}
	return *this;
}

CCString CCString::subString(size_t pos, size_t length) const {
	try {
		return CCString(internalStr.substr(pos, length));
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::bad_alloc& e) {
		throw;
	}

}

size_t CCString::find(const CCString& ccstr, size_t pos) const {
	return internalStr.find(ccstr.internalStr, pos);
}

size_t CCString::findLast(const CCString& ccstr, size_t pos) const {
	return internalStr.rfind(ccstr.internalStr, pos);
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
	std::regex_search(internalStr, matches, re);

	if (matches.empty()) {
		return CCString(*this);
	}
	if (!matches[0].matched) {
		return CCString(*this);
	}

	auto startOfString = matches.prefix().first;
	auto endOfFirstMatch = matches[0].second;
	auto endOfAll = matches.suffix().second;

	std::string resultFirstHalf;
	std::regex_replace(std::back_inserter(resultFirstHalf), startOfString, endOfFirstMatch, re,
			replacement.internalStr);
	resultFirstHalf.append(endOfFirstMatch, endOfAll);
	return CCString(resultFirstHalf);
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

CCString::SplitIterator& CCString::SplitIterator::operator=(const CCString::SplitIterator& orig) {
	if (this != &orig) {
		originString = orig.originString;
		separatorRegex = orig.separatorRegex;
		currentSplit = std::unique_ptr<CCString>(new CCString(*orig.currentSplit));
		currentRemainder = orig.currentRemainder;
		hadMatch = orig.hadMatch;
		isFinished = orig.isFinished;
	}
	return *this;
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
