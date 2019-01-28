/*
 * CCString.cpp
 *
 *  Created on: 21.01.2019
 *      Author: jan
 */

#include "CCString.h"

#include <cstddef>
#include <string>

CCString::CCString(std::string str) :
		internalStr(str) {
}

CCString::CCString(const char* cstr) :
		CCString(std::string(cstr)) {
}

CCString::~CCString() {
	// TODO Auto-generated destructor stub
}

std::string CCString::toString() {
	return internalStr;
}

size_t CCString::length() const {
	return internalStr.length();
}

char& CCString::at(size_t index) {
	return internalStr.at(index);
}

bool CCString::operator==(const CCString& other) const {
	return internalStr == other.internalStr;
}

bool CCString::operator<(const CCString& other) const {
	return internalStr < other.internalStr;
}

CCString& CCString::append(const CCString& ccStr) {
	return append(ccStr.internalStr);
}

CCString& CCString::append(const std::string& str) {
	internalStr.append(str);
	return *this;
}

CCString& CCString::append(const char* cstr) {
	internalStr.append(cstr);
	return *this;
}

CCString& CCString::append(char c) {
	internalStr.push_back(c);
	return *this;
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

CCString& CCString::replace(size_t pos, const CCString& ccStr) {
	return replace(pos, ccStr.internalStr);
}

CCString& CCString::replace(size_t pos, const std::string& str) {
	internalStr.replace(pos, str.length(), str);
	return *this;
}

CCString& CCString::replace(size_t pos, const char* cstr) {
	internalStr.replace(pos, std::char_traits<char>::length(cstr), cstr);
	return *this;
}

CCString& CCString::replace(size_t pos, char c) {
	internalStr.at(pos) = c;
	return *this;
}

CCString& CCString::insert(size_t pos, const CCString& ccStr) {
	return insert(pos, ccStr.internalStr);
}

CCString& CCString::insert(size_t pos, const std::string& str) {
	internalStr.insert(pos, str);
	return *this;
}

CCString& CCString::insert(size_t pos, const char* cstr) {
	internalStr.insert(pos, cstr);
	return *this;
}

CCString& CCString::insert(size_t pos, char c) {
	internalStr.insert(pos, 1, c);
	return *this;
}

CCString& CCString::erase(size_t pos, size_t length) {
	internalStr.erase(pos, length);
	return *this;
}

CCString CCString::subString(size_t pos, size_t length) {
	return CCString(internalStr.substr(pos, length));
}
