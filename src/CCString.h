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

	template<typename type> //catch invalid types
	/*can't convert to CCString     */CCString(type) = delete;

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
	template<typename type>
	CCString& append(const type& input) {
		/*needs conversion             */return append(CCString(input));
	}

	template<typename type>
	CCString& operator+=(const type& input) {
		/*needs conversion             */return append(CCString(input));
	}

	template<typename leftType, typename rightType>
	friend CCString operator+(const leftType& lhs, const rightType& rhs) {
		CCString copy(lhs);
		/*needs conversion             */copy += CCString(rhs);
		return copy;
	}

//	template<typename leftType, typename rightType>
//	friend CCString operator-(const leftType&, const rightType) = delete;
//	template<typename leftType, typename rightType>
//	friend CCString operator*(const leftType&, const rightType) = delete;
//	template<typename leftType, typename rightType>
//	friend CCString operator/(const leftType&, const rightType) = delete;
//	template<typename leftType, typename rightType>
//	friend CCString operator%(const leftType&, const rightType) = delete;
//	template<typename leftType, typename rightType>
//	friend CCString operator&(const leftType&, const rightType) = delete;
//	template<typename leftType, typename rightType>
//	friend CCString operator|(const leftType&, const rightType) = delete;
//	template<typename leftType, typename rightType>
//	friend CCString operator^(const leftType&, const rightType) = delete;
//	template<typename type>
//	friend CCString operator!(const type) = delete;

	CCString& replace(size_t pos, const CCString& ccStr);
	template<typename type>
	CCString& replace(size_t pos, const type& obj) {
		return replace(pos, CCString(obj));
	}

	CCString& insert(size_t pos, const CCString& ccStr);
	template<typename type>
	CCString& insert(size_t pos, const type& obj) {
		return insert(pos, CCString(obj));
	}

	CCString& erase(size_t pos, size_t length);

	CCString subString(size_t pos, size_t length) const;

	size_t find(const CCString& ccstr, size_t pos = 0) const;
	template<typename type>
	size_t find(const type& obj, size_t pos = 0) const {
		/*needs conversion			*/return find(CCString(obj), pos);
	}
	size_t findLast(const CCString& ccstr,
			size_t pos = std::string::npos) const;
	template<typename type>
	size_t findLast(const type& obj, size_t pos = std::string::npos) const {
		/*needs conversion			*/return findLast(CCString(obj), pos);
	}

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
