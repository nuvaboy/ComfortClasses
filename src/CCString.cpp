/*
 * CCString.cpp
 *
 *  Created on: 21.01.2019
 *      Author: jan
 */

#include "CCString.h"

#include <cstddef>
#include <string>
#include <exception>
#include <stdexcept>

CCString::CCString(std::string str) :
		internalStr(str) {
}
CCString::CCString(const char* cstr) :
		CCString(std::string(cstr)) {
}
CCString::CCString(char c) :
		CCString(std::string() += c) {
}
CCString::~CCString() = default;

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
		throw std::out_of_range("Index pointing outside of the string.");
	}
}
const char& CCString::at(size_t index) const {
	try {
		return internalStr.at(index);
	} catch (std::out_of_range& e) {
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
		return append(ccStr.internalStr);
	} catch (std::length_error& e) {
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
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
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
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
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
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
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
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

CCString operator+(CCString lhs, const CCString& rhs) {
	lhs += rhs;
	return lhs;
}
CCString operator+(CCString lhs, const std::string& rhs) {
	lhs += rhs;
	return lhs;
}
CCString operator+(CCString lhs, const char* rhs) {
	lhs += rhs;
	return lhs;
}
CCString operator+(CCString lhs, char rhs) {
	lhs += rhs;
	return lhs;
}

CCString& CCString::replace(size_t pos, const CCString& ccStr) {
	return replace(pos, ccStr.internalStr);
}
CCString& CCString::replace(size_t pos, const std::string& str) {
	try {
		internalStr.replace(pos, str.length(), str);
		return *this;
	} catch (std::out_of_range& e) {
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
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
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
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
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (...) {
		throw std::exception();
	}
}

CCString& CCString::insert(size_t pos, const CCString& ccStr) {
	return insert(pos, ccStr.internalStr);
}
CCString& CCString::insert(size_t pos, const std::string& str) {
	try {
		internalStr.insert(pos, str);
		return *this;
	} catch (std::out_of_range& e) {
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
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
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
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
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::length_error& e) {
		throw std::length_error("Result exceeding max length for a string.");
	} catch (std::bad_alloc& e) {
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}

CCString& CCString::erase(size_t pos, size_t length) {
	try {
		internalStr.erase(pos, length);
		return *this;
	} catch (std::out_of_range& e) {
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (...) {
		throw std::exception();
	}
}

CCString CCString::subString(size_t pos, size_t length) const {
	try {
		return CCString(internalStr.substr(pos, length));
	} catch (std::out_of_range& e) {
		throw std::out_of_range("Position pointing outside of the string.");
	} catch (std::bad_alloc& e) {
		throw std::bad_alloc();
	} catch (...) {
		throw std::exception();
	}
}

size_t CCString::find(const CCString& ccstr, size_t pos) const {
	return find(ccstr.internalStr, pos);
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
	return findLast(ccstr.internalStr, pos);
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
