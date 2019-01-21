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

