/*
 * CCString.cpp
 *
 *  Created on: 21.01.2019
 *      Author: jan
 */

#include "CCString.h"

#include <iomanip>
#include <limits>
#include <locale>
#include <sstream>
#include <stdexcept>

//constructors for textual types
CCString::CCString(const std::string& str) :
		internalStr(str) {
}
CCString::CCString(const char* cstr) :
		CCString(std::string(cstr)) {
}
CCString::CCString(char* cstr) :
		CCString() {
	for (int i = 0; cstr[i] != '\0'; i++) {
		append(cstr[i]);
	}
}
CCString::CCString(char* cstr, size_t n) {
	for (unsigned int i = 0; i < n; i++) {
		append(cstr[i]);
	}
}
CCString::CCString(char c) :
		CCString(std::string() += c) {
}

//constructor for boolean type
CCString::CCString(bool b) :
		CCString() {
	if (b) {
		internalStr =
				std::use_facet<std::numpunct<char>>(std::locale()).truename();
	} else {
		internalStr =
				std::use_facet<std::numpunct<char>>(std::locale()).falsename();
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
		stringStream
				<< std::setprecision(std::numeric_limits<long double>::digits10)
				<< number;
		internalStr = stringStream.str();
	} else {
		internalStr = std::to_string(number);
	}
}
CCString::CCString(double number, bool hiPrec) :
		CCString(static_cast<long double>(number), hiPrec) {
}
CCString::CCString(float number, bool hiPrec) :
		CCString(static_cast<long double>(number), hiPrec) {
}

CCString::~CCString() = default;

/* ######################################### */

std::string CCString::toString() const {
	return internalStr;
}
CCString::operator const char*() {
	return internalStr.c_str();
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
	} catch (...) {
		throw std::exception();
	}
}
const char& CCString::at(size_t index) const {
	try {
		return internalStr.at(index);
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Index pointing outside of the string.");
	} catch (...) {
		throw std::exception();
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
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}
CCString& CCString::append(const std::string& str) {
	try {
		internalStr.append(str);
		return *this;
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}
CCString& CCString::append(const char* cstr) {
	try {
		internalStr.append(cstr);
		return *this;
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}
CCString& CCString::append(char c) {
	try {
		internalStr.push_back(c);
		return *this;
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
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
CCString& CCString::append(float f) {
	return append(CCString(f));
}
CCString& CCString::append(double d) {
	return append(CCString(d));
}
CCString& CCString::append(long double d) {
	return append(CCString(d));
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

//CCString operator+(const CCString& lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const CCString& lhs, const char* rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const char* lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const CCString& lhs, const std::string& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const std::string& lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const CCString& lhs, char rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(char lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//
//CCString operator+(const CCString& lhs, bool rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(bool lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const CCString& lhs, int16_t rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(int16_t lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const CCString& lhs, int32_t rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(int32_t lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const CCString& lhs, int64_t rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(int64_t lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(float lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//
//CCString operator+(const CCString& lhs, double rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(double lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(const CCString& lhs, long double rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}
//CCString operator+(long double lhs, const CCString& rhs) {
//	CCString copy(lhs);
//	copy += rhs;
//	return copy;
//}

CCString& CCString::replace(size_t pos, const CCString& ccStr) {
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
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}

}
CCString& CCString::replace(size_t pos, const std::string& str) {
	try {
		internalStr.replace(pos, str.length(), str);
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}
CCString& CCString::replace(size_t pos, const char* cstr) {
	try {
		internalStr.replace(pos, std::char_traits<char>::length(cstr), cstr);
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}
CCString& CCString::replace(size_t pos, char c) {
	try {
		internalStr.at(pos) = c;
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (...) {
		throw std::exception();
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
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}
CCString& CCString::insert(size_t pos, const std::string& str) {
	try {
		internalStr.insert(pos, str);
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}
CCString& CCString::insert(size_t pos, const char* cstr) {
	try {
		internalStr.insert(pos, cstr);
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}
CCString& CCString::insert(size_t pos, char c) {
	try {
		internalStr.insert(pos, 1, c);
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		e.what();
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
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
CCString& CCString::insert(size_t pos, float f) {
	return insert(pos, CCString(f));
}
CCString& CCString::insert(size_t pos, double d) {
	return insert(pos, CCString(d));
}
CCString& CCString::insert(size_t pos, long double d) {
	return insert(pos, CCString(d));
}

CCString& CCString::erase(size_t pos, size_t length) {
	try {
		internalStr.erase(pos, length);
		return *this;
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (...) {
		throw std::exception();
	}
}

CCString CCString::subString(size_t pos, size_t length) const {
	try {
		return CCString(internalStr.substr(pos, length));
	} catch (std::out_of_range& e) {
		e.what();
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::bad_alloc& e) {
		e.what();
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
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
