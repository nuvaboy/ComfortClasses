/*
 * CCString.cpp
 *
 *  Created on: 21.01.2019
 *      Author: jan
 */

#include "CCString.h"

#include <string>

//CCString::CCString() {
//	// TODO Auto-generated constructor stub
//
//}

CCString::CCString(std::string str) :
		CCString() {
	internalStr = str;
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

CCString& CCString::append(CCString& ccStr) {
	return append(ccStr.internalStr);
}

CCString& CCString::append(const char* cstr){
	return append(std::string(cstr));
}

CCString& CCString::append(char c){
	this->internalStr.push_back(c);
	return *this;
}

CCString& CCString::append(std::string str) {
	this->internalStr.append(str);
	return *this;
}

