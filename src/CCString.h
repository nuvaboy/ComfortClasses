/*
 * CCString.h
 *
 *  Created on: 21.01.2019
 *      Author: jan
 */

#ifndef CCSTRING_H_
#define CCSTRING_H_

#include <cstddef>
#include <string>

class CCString {
public:
	CCString() = default;
	CCString(const CCString&) = default;

	CCString(std::string);
	CCString(const char* cstr);

	virtual ~CCString();

	std::string toString();
	size_t length() const;
	char& at(size_t index);

	CCString& append(const CCString& ccStr);
	CCString& append(const std::string& str);
	CCString& append(const char* cstr);
	CCString& append(char c);

	CCString& operator+=(const CCString& ccStr);
	CCString& operator+=(const std::string& str);
	CCString& operator+=(const char* cstr);
	CCString& operator+=(char c);

	CCString& replace(size_t pos, const CCString& ccStr);
	CCString& replace(size_t pos, const std::string& str);
	CCString& replace(size_t pos, const char* cstr);

	CCString& insert(size_t pos, const CCString& ccStr);
	CCString& insert(size_t pos, const std::string& str);
	CCString& insert(size_t pos, const char* cstr);

	CCString& erase(size_t pos, size_t length);

	CCString subString(size_t pos, size_t length);
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
