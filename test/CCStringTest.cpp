/*
 * CCString_test.cpp
 *
 *  Created on: 20.12.2018
 *      Author: jan
 */
#include "gtest/gtest.h"
#include "../src/CCString.h"

//#define GROUP_TESTS

#ifdef GROUP_TESTS
#define GROUP_TEST(GroupName, TestBase, TestName) \
TEST(TestBase##_##GroupName, TestName)
#else
#define GROUP_TEST(GroupName, TestBase, TestName) \
TEST(TestBase, TestName)
#endif

GROUP_TEST(constructors, CCString, constructFromCString) {
	CCString ccstr1("abcde");
	CCString ccstr2 = "fghij";
	ASSERT_EQ(ccstr1.toString(), std::string("abcde"));
	ASSERT_EQ(ccstr2.toString(), std::string("fghij"));
}

GROUP_TEST(constructors, CCString, constructFromStdString) {
	std::string str1 = "abcde";
	std::string str2("fghij");
	CCString ccstr1 = str1;
	CCString ccstr2(str2);
	ASSERT_EQ(ccstr1.toString(), str1);
	ASSERT_EQ(ccstr2.toString(), str2);
	//check for independence of copied string
	str2 += "klmno";
	str1 += str2;
	ASSERT_NE(ccstr1.toString(), str1);
	ASSERT_NE(ccstr1.toString(), str2);
}

GROUP_TEST(constructors, CCString, constructCopy) {
	CCString ccstr1("abc");
	CCString ccstr2(ccstr1);
	ccstr1 = "def";
	ASSERT_NE(ccstr1.toString(), ccstr2.toString());
	ASSERT_EQ(ccstr1.toString(), std::string("def"));
	ASSERT_EQ(ccstr2.toString(), std::string("abc"));
}

GROUP_TEST(comparisons, CCString, operatorEqual) {
	CCString ccstr1("a"), ccstr2("b"), ccstr3("b");
	ASSERT_NE(ccstr1, ccstr2);
	ASSERT_NE(ccstr1, ccstr3);
	ASSERT_EQ(ccstr2, ccstr3);
}

GROUP_TEST(comparisons, CCString, operatorLessThan) {
	CCString ccstr1("a"), ccstr2("aa"), ccstr3("b");
	ASSERT_EQ(ccstr2 <= ccstr2, ccstr2 >= ccstr2);
	ASSERT_LT(ccstr1, ccstr2);
	ASSERT_GT(ccstr3, ccstr2);
	ASSERT_EQ(ccstr1 < ccstr3, ccstr2 < ccstr3);
}

GROUP_TEST(conversions, CCString, castToConstCString) {
	CCString ccstr("abc");
	ASSERT_NO_FATAL_FAILURE(static_cast<std::string>(ccstr));
	std::string str = static_cast<std::string>(ccstr);
	str.append("def");
	ASSERT_EQ(str, "abcdef");
	EXPECT_NE(ccstr, "abcdef");
	ccstr = str;
	ASSERT_EQ(ccstr, "abcdef");

	ccstr = "abc";
	char cstr[10];
	strcpy(cstr, static_cast<const char*>(ccstr));
	ASSERT_EQ(ccstr, cstr);

	std::stringstream sstream;
	ASSERT_NO_FATAL_FAILURE(sstream << ccstr);
}

GROUP_TEST(append, CCString, append_operator) {
	CCString ccstr;
	ccstr += "abc";
	ccstr += std::string("def");
	ccstr += CCString("ghi");
	ccstr += 'j';
	ASSERT_EQ(ccstr, "abcdefghij");
}
