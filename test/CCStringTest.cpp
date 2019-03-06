/*
 * CCString_test.cpp
 *
 *  Created on: 20.12.2018
 *      Author: jan
 */
#include "gtest/gtest.h"
#include "../src/CCString.h"

#include <sstream>

#define GROUP_TESTS

#ifdef GROUP_TESTS
#define GROUP_TEST(GroupName, TestBase, TestName) \
TEST(TestBase##_##GroupName, TestName)
#else
#define GROUP_TEST(GroupName, TestBase, TestName) \
TEST(TestBase, TestName)
#endif

GROUP_TEST(constructors, CCStringTest, fromCString) {
	CCString ccstr1("abcde");
	CCString ccstr2 = "fghij";
	EXPECT_EQ(ccstr1.toString(), std::string("abcde"));
	EXPECT_EQ(ccstr2.toString(), std::string("fghij"));
}

GROUP_TEST(constructors, CCStringTest, fromStdString) {
	std::string str1 = "abcde";
	std::string str2("fghij");
	CCString ccstr1 = str1;
	CCString ccstr2(str2);
	EXPECT_EQ(ccstr1.toString(), str1);
	EXPECT_EQ(ccstr2.toString(), str2);
	//check for independence of copied string
	str2 += "klmno";
	str1 += str2;
	EXPECT_NE(ccstr1.toString(), str1);
	EXPECT_NE(ccstr1.toString(), str2);
}

GROUP_TEST(constructors, CCStringTest, copy) {
	CCString ccstr1("abc");
	CCString ccstr2(ccstr1);
	ccstr1 = "def";
	EXPECT_NE(ccstr1.toString(), ccstr2.toString());
	EXPECT_EQ(ccstr1.toString(), std::string("def"));
	EXPECT_EQ(ccstr2.toString(), std::string("abc"));
}

GROUP_TEST(comparisons, CCStringTest, operatorEqual) {
	CCString ccstr1("a"), ccstr2("b"), ccstr3("b");
	EXPECT_NE(ccstr1, ccstr2);
	EXPECT_NE(ccstr1, ccstr3);
	EXPECT_EQ(ccstr2, ccstr3);
}

GROUP_TEST(comparisons, CCStringTest, operatorLessThan) {
	CCString ccstr1("a"), ccstr2("aa"), ccstr3("b");
	EXPECT_EQ(ccstr2 <= ccstr2, ccstr2 >= ccstr2);
	EXPECT_LT(ccstr1, ccstr2);
	EXPECT_GT(ccstr3, ccstr2);
	EXPECT_EQ(ccstr1 < ccstr3, ccstr2 < ccstr3);
}

GROUP_TEST(conversions, CCStringTest, castToConstCString) {
	CCString ccstr("abc");
	EXPECT_NO_FATAL_FAILURE(static_cast<std::string>(ccstr));
	std::string str = static_cast<std::string>(ccstr);
	str.append("def");
	EXPECT_EQ(str, "abcdef");
	EXPECT_NE(ccstr, "abcdef");
	ccstr = str;
	EXPECT_EQ(ccstr, "abcdef");

	ccstr = "abc";
	char cstr[10];
	strcpy(cstr, static_cast<const char*>(ccstr));
	EXPECT_EQ(ccstr, cstr);
}

GROUP_TEST(append, CCStringTest, appendMethod) {
	CCString ccstr;
	ccstr.append("abc");
	ccstr.append(std::string("def"));
	ccstr.append(CCString("ghi"));
	ccstr.append('j');
	EXPECT_EQ(ccstr, CCString("abcdefghij"));
}

GROUP_TEST(append, CCStringTest, appendOperator) {
	CCString ccstr;
	ccstr << "abc";
	ccstr << std::string("def");
	ccstr << CCString("ghi");
	ccstr << 'j';
	EXPECT_EQ(ccstr, CCString("abcdefghij"));
}

GROUP_TEST(append, CCStringTest, concatenateOperator) {
	CCString ccstr;
	ccstr = ccstr << std::string("abc");
	ccstr = ccstr << "def";
	ccstr = ccstr << 'g';
	ccstr = ccstr << CCString("hij");
	EXPECT_EQ(ccstr, CCString("abcdefghij"));
}

GROUP_TEST(edit, CCStringTest, replace) {
	CCString ccstr = 1234567890;
	ccstr.replaceAt(0, CCString("abc"));
	EXPECT_EQ(ccstr, CCString("abc4567890"));
	ccstr.replaceAt(3, std::string("def"));
	EXPECT_EQ(ccstr, CCString("abcdef7890"));
	ccstr.replaceAt(6, "ghi");
	EXPECT_EQ(ccstr, CCString("abcdefghi0"));
	ccstr.replaceAt(9, 'j');
	EXPECT_EQ(ccstr, CCString("abcdefghij"));
}

GROUP_TEST(edit, CCStringTest, insert_erase) {
	CCString ccstr = 1234567890;
	ccstr.insert(0, CCString("abc"));
	ccstr.erase(3, 3);
	EXPECT_EQ(ccstr, CCString("abc4567890"));
	ccstr.insert(3, std::string("def"));
	ccstr.erase(6, 3);
	EXPECT_EQ(ccstr, CCString("abcdef7890"));
	ccstr.insert(6, "ghi");
	ccstr.erase(9, 3);
	EXPECT_EQ(ccstr, CCString("abcdefghi0"));
	ccstr.insert(9, 'j');
	ccstr.erase(10, 1);
	EXPECT_EQ(ccstr, CCString("abcdefghij"));
}

GROUP_TEST(stream, CCStringTest, streamOperator) {
	std::stringstream strBuf;
	CCString ccstr1("123");
	CCString ccstr2('4');
	strBuf << ccstr1 << ccstr2 << '5' << 6;
	CCString ccstr3;
	strBuf >> ccstr3;
	EXPECT_EQ(ccstr3, CCString(123456));
}

GROUP_TEST(find, CCStringTest, finds) {
	CCString ccstr("abcdefgdefhdef");
	EXPECT_EQ(ccstr.find(CCString("bcd")), (size_t )1);
	EXPECT_EQ(ccstr.findLast(CCString("fgd")), (size_t )5);
	EXPECT_EQ(ccstr.find("def", ccstr.find(std::string("efg"))),
			ccstr.findLast("def", ccstr.findLast(std::string("hde"))));
	EXPECT_EQ(ccstr.find('1'), static_cast<size_t>(-1));
	EXPECT_EQ(ccstr.findLast('1'), static_cast<size_t>(-1));
}
