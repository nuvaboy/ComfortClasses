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
	static const size_t allOfStringPos = std::string::npos;

	CCString() = default;
	CCString(const CCString&) = default;

	CCString(std::string);
	CCString(const char* cstr);
	CCString(char c);

	virtual ~CCString();

	std::string toString() const;
	operator const char*();

	friend std::ostream& operator<<(std::ostream& os, const CCString& ccstr);
	friend std::istream& operator>>(std::istream& in, CCString& ccstr);

	size_t length() const;

	char& at(size_t index);
	char& operator[](size_t index);
	const char& at(size_t index) const;
	const char& operator[](size_t index) const;

	bool operator==(const CCString& other) const;
	bool operator<(const CCString& other) const;

	CCString& append(const CCString& ccStr);
	CCString& append(const std::string& str);
	CCString& append(const char* cstr);
	CCString& append(char c);

	CCString& operator+=(const CCString& ccStr);
	CCString& operator+=(const std::string& str);
	CCString& operator+=(const char* cstr);
	CCString& operator+=(char c);

	friend CCString operator+(CCString lhs, const CCString& rhs);
	friend CCString operator+(CCString lhs, const std::string& rhs);
	friend CCString operator+(CCString lhs, const char* rhs);
	friend CCString operator+(CCString lhs, char rhs);

	CCString& replace(size_t pos, const CCString& ccStr);
	CCString& replace(size_t pos, const std::string& str);
	CCString& replace(size_t pos, const char* cstr);
	CCString& replace(size_t pos, char c);

	CCString& insert(size_t pos, const CCString& ccStr);
	CCString& insert(size_t pos, const std::string& str);
	CCString& insert(size_t pos, const char* cstr);
	CCString& insert(size_t pos, char c);

	CCString& erase(size_t pos, size_t length);

	CCString subString(size_t pos, size_t length) const;

	size_t find(const CCString& ccstr, size_t pos = 0) const;
	size_t find(const std::string& str, size_t pos = 0) const;
	size_t find(const char* cstr, size_t pos = 0) const;
	size_t find(char c, size_t pos = 0) const;

	size_t findLast(const CCString& ccstr,
			size_t pos = std::string::npos) const;
	size_t findLast(const std::string& str,
			size_t pos = std::string::npos) const;
	size_t findLast(const char* cstr, size_t pos = std::string::npos) const;
	size_t findLast(char c, size_t pos = std::string::npos) const;

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

/* as per https://en.cppreference.com/w/cpp/language/operators */
inline bool operator!=(const CCString& lhs, const CCString& rhs) {
	return !(lhs == rhs);
}
inline bool operator>(const CCString& lhs, const CCString& rhs) {
	return rhs < lhs;
}
inline bool operator<=(const CCString& lhs, const CCString& rhs) {
	return !(lhs > rhs);
}
inline bool operator>=(const CCString& lhs, const CCString& rhs) {
	return !(lhs < rhs);
}

#endif /* CCSTRING_H_ */
