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
//	static const size_t allOfStringPos = std::string::npos;

	CCString() = default;
	CCString(const CCString&) = default;

	//constructors for textual types
	CCString(const std::string& str);
	CCString(const char* cstr);
	CCString(char c);
	//constructor for boolean type
	CCString(bool b);
	//constructors for numeric types:
	CCString(int16_t number);
	CCString(int32_t number);
	CCString(int64_t number);
	CCString(uint16_t number);
	CCString(uint32_t number);
	CCString(uint64_t number);
	CCString(float number, bool hiPrec = false);
	CCString(double number, bool hiPrec = false);
	CCString(long double number, bool hiPrec = false);

	//catch invalid types; if needed, add constructor above
	template<typename type>
	/*  can't convert to CCString    */CCString(type) = delete;

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
	CCString& append(int16_t i);
	CCString& append(int32_t i);
	CCString& append(int64_t i);
	CCString& append(uint16_t i);
	CCString& append(uint32_t i);
	CCString& append(uint64_t i);
	CCString& append(float f);
	CCString& append(double d);
	CCString& append(long double d);

	template<typename type>
	/* operand type not supported   */CCString& append(const type&) = delete;

	CCString& operator+=(const CCString& ccStr);
	CCString& operator+=(const std::string& str);
	CCString& operator+=(const char* cstr);
	CCString& operator+=(char c);
	CCString& operator+=(int16_t i);
	CCString& operator+=(int32_t i);
	CCString& operator+=(int64_t i);
	CCString& operator+=(uint16_t i);
	CCString& operator+=(uint32_t i);
	CCString& operator+=(uint64_t i);
	CCString& operator+=(float f);
	CCString& operator+=(double d);
	CCString& operator+=(long double d);

	template<typename type>
	/* operand type not supported   */CCString& operator+=(const type&) = delete;

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
	size_t find(const std::string& str, size_t pos) const;
	size_t find(const char* cstr, size_t pos) const;
	size_t find(char c, size_t pos) const;

	size_t findLast(const CCString& ccstr,
			size_t pos = std::string::npos) const;
	size_t findLast(const std::string& str, size_t pos) const;
	size_t findLast(const char* cstr, size_t pos) const;
	size_t findLast(char c, size_t pos) const;

private:
	std::string internalStr;
};

CCString operator+(const CCString& lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, const char* rhs);
CCString operator+(const char* lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, const std::string& rhs);
CCString operator+(const std::string& lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, char rhs);
CCString operator+(char lhs, const CCString& rhs);

CCString operator+(const CCString& lhs, int16_t rhs);
CCString operator+(int16_t lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, int32_t rhs);
CCString operator+(int32_t lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, int64_t rhs);
CCString operator+(int64_t lhs, const CCString& rhs);

CCString operator+(const CCString& lhs, uint16_t rhs);
CCString operator+(uint16_t lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, uint32_t rhs);
CCString operator+(uint32_t lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, uint64_t rhs);
CCString operator+(uint64_t lhs, const CCString& rhs);

CCString operator+(const CCString& lhs, float rhs);
CCString operator+(float lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, double rhs);
CCString operator+(double lhs, const CCString& rhs);
CCString operator+(const CCString& lhs, long double rhs);
CCString operator+(long double lhs, const CCString& rhs);

template<typename type1, typename type2>
/* operand type not supported   */CCString operator+(const type1&, const type2&) = delete;

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
