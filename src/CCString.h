/*
 * CCString.h
 *
 *  Created on: 21.01.2019
 *      Author: jan
 */

#ifndef CCSTRING_H_
#define CCSTRING_H_

#include <string>

class CCString {
public:
	CCString() = default;
	CCString(const CCString&) = default;

	CCString(std::string);
	CCString(const char* cstr);

	virtual ~CCString();

	std::string toString();

	CCString& append(CCString& ccStr);
	CCString& append(std::string str);
	CCString& append(const char* cstr);
	CCString& append(char c);
//	/* + addition operator */
//
//	CCString& replace(CCString);
//	CCString& replace(char c, int pos);
//
//
//
//
//	//iterators for use alongside std::string;
//	std::string::iterator begin();
//	std::string::iterator end();
//	std::string::reverse_iterator rbegin();
//	std::string::reverse_iterator rend();
//	std::string::const_iterator cbegin();
//	std::string::const_iterator cend();

private:
	std::string internalStr;

};

#endif /* CCSTRING_H_ */
