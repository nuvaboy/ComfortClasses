#include "gtest/gtest.h"
#include "../src/CCDecimal.h"

using namespace std;

#define GROUP_TESTS

//MA
#ifdef GROUP_TESTS
#define GROUP_TEST(GroupName, TestBase, TestName) \
TEST(TestBase##_##GroupName, TestName)
#else
#define GROUP_TEST(GroupName, TestBase, TestName) \
TEST_F(TestBase, TestName)
#endif

class CCDecimalTest: public ::testing::Test {
public:

	CCDecimalTest() {
	}
	//testing only
	static void setDigit(CCDecimal& dec, unsigned int pos, int8_t value) {

		dec.digit[pos] = value;
		if (pos >= dec.used && value != 0) {
			dec.used = pos + 1;
		}
	}

	static void setNegative(CCDecimal& dec, bool isNegative) {

		dec.isNegative = isNegative;
	}

	static void setDigits(CCDecimal& dec, int count, ...) {

		//cout << count << endl;

		va_list arguments;             // A place to store the list of arguments
		va_start(arguments, count); // Initializing arguments to store all values after count
		for (int i = count - 1; i >= 0; i--) {
			int val = (int) (va_arg(arguments, int));
			setDigit(dec, i, val);
			//cout << va_arg(arguments, int) << endl;
		}
		// Adds the next value in argument list to sum.
		va_end(arguments);

	}

	static void setShift(CCDecimal& dec, int shift) {

		dec.shift = shift;
	}
};

void isEqual(const CCDecimal& value, const CCDecimal& cmp) {
	bool eq = (value == cmp);
	EXPECT_TRUE(eq);
	if (eq == false) {
		cout << "value was: " << value.toString(MAX - 1) << " correct value: "
				<< cmp.toString(MAX - 1) << endl;
	}
}

void testAdd(string s1, string s2, string r) {
	CCDecimal d1 = s1;
	CCDecimal d2 = s2;
	CCDecimal result = r;
	isEqual(d2 + d1, result);
}
void testSub(string s1, string s2, string r) {
	CCDecimal d1 = s1;
	CCDecimal d2 = s2;
	CCDecimal result = r;
	isEqual(d1 - d2, result);
}
void testMult(string s1, string s2, string r) {
	CCDecimal d1 = s1;
	CCDecimal d2 = s2;
	CCDecimal result = r;
	isEqual(d1 * d2, result);
}
void testDiv(string s1, string s2, string r) {
	CCDecimal d1 = s1;
	CCDecimal d2 = s2;
	CCDecimal result = r;
	isEqual(d1 / d2, result);
}

void testLessThan(string s1, string s2, bool r) {
	CCDecimal d1 = s1;
	CCDecimal d2 = s2;

	bool b = d1 < d2;
	EXPECT_EQ(b, r);

	if (b != r) {
		cout << "  d1: " << d1.toString(MAX - 1) << " d2: " << d2.toString(MAX - 1) << endl;
	}

}
void testMoreThan(string s1, string s2, bool r) {
	CCDecimal d1 = s1;
	CCDecimal d2 = s2;

	bool b = d1 > d2;
	EXPECT_EQ(b, r);

	if (b != r) {
		cout << "  d1: " << d1.toString(MAX - 1) << " d2: " << d2.toString(MAX - 1) << endl;
	}
}
void testEqual(string s1, string s2, bool r) {
	CCDecimal d1 = s1;
	CCDecimal d2 = s2;

	bool b = d1 == d2;
	EXPECT_EQ(b, r);
	EXPECT_EQ(d1 != d2, !r);

	if (b != r) {
		cout << "  d1: " << d1.toString(MAX - 1) << " d2: " << d2.toString(MAX - 1) << endl;
	}
}

void testFromStr(string input, const CCDecimal& ref) {
	isEqual(CCDecimal(input), ref);
}
void testToStr(const string s1, const string r, bool scientific = false) {
	CCDecimal d1 = s1;
	EXPECT_EQ(d1.toString(scientific), r);
}
void testToStr(const string r) {
	CCDecimal d1 = r;
	EXPECT_EQ(d1.toString(), r);
}
void testToDouble(const string s1, const double r){
	CCDecimal d1 = s1;
	EXPECT_EQ(CCDecimal(d1.toDouble()), CCDecimal(r));
}


//void testToStrSci(const string s1, const string r){
//	CCDecimal d1 = s1;
//	EXPECT_EQ(d1.toString(true), r);
//}

// Getter / Setter ########################################
GROUP_TEST(GetterSetter, CCDecimalTest, defaultPrecision) {
	CCDecimal::setGlobalPrecision(6);
	int defaultPrecision = CCDecimal::getGlobalPrecision();
	EXPECT_EQ(defaultPrecision, 6);

	CCDecimal d1 = 45;
	d1.setLocalPrecision(3);
	EXPECT_EQ(d1.getPrecision(), 3);

	CCDecimal d2 = d1;
	EXPECT_EQ(d2.getPrecision(), 3);

}

// Addition ###############################################
GROUP_TEST(Addition, CCDecimalTest, add_shift) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: match left
	testAdd("12.3", "89.25", "101.55");

	//Id. 1: match right
	testAdd("268555", "123", "268678");

	//Id. 2: overlap left
	testAdd("565.8555", "566524.4", "567090.2555");

	//Id. 3: overlap right
	testAdd("8561000", "32565", "8593565");

	//Id. 4: overlap both
	testAdd("0.00027", "0.0128665", "0.0131365");

	//Id. 5: overlap none
	testAdd("512.2552", "89.95", "602.2052");

	//Id. 6: touch left
	testAdd("5885.255", "1280000", "1285885.255");

	//Id. 7: touch right
	testAdd("5885.255", "0.000942", "5885.255942");

	//Id. 8: gap left
	testAdd("5885.255", "6300000", "6305885.255");

	//Id. 9: gap right
	testAdd("5885.255", "0.0000078", "5885.2550078");
}

GROUP_TEST(Addition, CCDecimalTest, add_tz) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: dp, one tz
	testAdd("12.34", "3.46", "15.8");

	//Id. 1: dp, two tz
	testAdd("12.34", "3.66", "16");

	//Id. 2: no dp, one tz
	testAdd("1234", "346", "1580");

	//Id. 3: no dp, two tz
	testAdd("956885", "3115", "960000");
}

GROUP_TEST(Addition, CCDecimalTest, add_cut) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: cut 0 digits, result reached maximal capacity (MAX)
	testAdd("1234567890123456789012345678.1", "15.892", "1234567890123456789012345693.992");

	//Id. 1: cut 1 digit, result now fits into max capacity (MAX)
	testAdd("2345678901234567890123456789.01", "5.6482", "2345678901234567890123456794.658");

	//Id. 2: cut 4 digits, further reduced by trailing zeroes (changed!!!!!!!!!!!!!!!!!, v. Jan)
	testAdd("3456789012345678901234567898.123", "1.8774785", "3456789012345678901234567900.000");
}

GROUP_TEST(Addition, CCDecimalTest, add_special) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: result is 0
	testAdd("0", "0", "0");

	//Id. 1: addition causes temporal overflow, fixed by removal of trailing zero
	testAdd("987654321098765432109876543210.9", "100000000000000000000000000000.1",
			"1087654321098765432109876543211");

	//Id. 2: addition causes overflow, (occupied space outside of the max. capacity)
	ASSERT_THROW(
			CCDecimal("987654321098765432109876543210.8")
					+ CCDecimal("100000000000000000000000000000.1"), std::overflow_error);

	//Id. 3: addition causes overflow (could not preserve required precision)
	ASSERT_THROW(CCDecimal("287654321098765432109876543210.9") + CCDecimal("5.13"),
			std::overflow_error);

}

GROUP_TEST(Addition, CCDecimalTest, add_whiteBox) {
	CCDecimal::setGlobalPrecision(2);

	//removal of trailing zeroes after cutting, due to capacity constraint
	testAdd("9.1234567890123456789012345678904", "0.077", "9.20045678901234567890123456789");

	//cutting
	testAdd("1.012345678901234567890123456789", "345.23", "346.2423456789012345678901234568");

	//cutting=>overflow (precision constraint)
	CCDecimal a("10123456789012345678901234567.89");
	CCDecimal b("90000000000000000000000000000.02");
	ASSERT_THROW(a + b, std::overflow_error);

	//no cutting
	testAdd("15566.478", "19.23", "15585.708");

	//carry in MSD, no overflow
	testAdd("95.23", "4.812", "100.042");

	//trailing zeroes, after dp
	testAdd("123.456", "5.244", "128.700");

	//trailing zeroes, directly after dp
	testAdd("123.456", "5.544", "129.000");

	//trailing zeroes, surpassing dp
	testAdd("123.456", "6.544", "130.000");

	//temporal overflow, fixed by trailing zeroes
	testAdd("12345678901234567890123456789.01", "90000000000000000000000000000.09",
			"102345678901234567890123456789.10");

	//overflow (Capacity constraint)
	EXPECT_THROW(
			CCDecimal("12345678901234567890123456789.01")
					+ CCDecimal("90000000000000000000000000000.08"), std::overflow_error);

}

// Subtraction ############################################
GROUP_TEST(Subtraction, CCDecimalTest, sub_shift) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: match left
	testSub("89.25", "12.3", "76.95");

	//Id. 1: match right
	testSub("268555", "723", "267832");

	//Id. 2: overlap right
	testSub("8561000", "32565", "8528435");

	//Id. 3: overlap none
	testSub("512.2552", "89.95", "422.3052");

	//Id. 4: touch left
	testSub("1280000", "5885.255", "1274114.745");

	//Id. 5: touch right
	testSub("5885.255", "0.000942", "5885.254058");

	//Id. 6: gap left
	testSub("6300000", "5885.255", "6294114.745");

	//Id. 7: gap right
	testSub("5885.255", "0.0000078", "5885.2549922");
}

GROUP_TEST(Subtraction, CCDecimalTest, sub_cut) {

	CCDecimal::setGlobalPrecision(2);
	//Id. 0: cut 0 digits, result reached maximal capacity (MAX)
	testSub("123456789012345678901234.5", "12345.678901", "123456789012345678888888.821099");

	//Id. 1.1: cut zero digits, because carry into MSD caused one leading zero
	testSub("100456789012345678901234.5", "1121135421421000012345.67890123",
			"99335653590924678888888.82109877");

	//Id. 1.2: cut zero digits, because carry into MSD caused one leading zero
	testSub("100000000000000000000000", "0.67890123", "99999999999999999999999.32109877");

	//Id. 2: cut three digits, result reached maximal capacity
	testSub("123456789012345678901234.5", "17.5678901234", "123456789012345678901216.9321098");

	//Id. 3: cut one digit and reduce size by four more trailing zeroes
	testSub("123456789012345678901234.5678042", "18.62380412", "123456789012345678901215.944");

	//Id. 4: cut one digit and reduce size by ten more trailing zeroes (flooding into head)
	testSub("123456789012345678901034.5678042", "34.56780412", "123456789012345678901000");

	//new
	testSub("1234567890123456789012345678.907",
			                           "6.2059",
		    "1234567890123456789012345672.701");
}

GROUP_TEST(Subtraction, CCDecimalTest, sub_special) {

	CCDecimal::setGlobalPrecision(2);

	//Id. 0: both decimals are zero
	testSub("0", "0", "0");

	//Id. 1: a - 0 = a (a > 0)
	testSub("4644848.62355", "0", "4644848.62355");

	//Id. 2: a - b = 0 (a,b > 0)
	testSub("589.523", "589.523", "0");

	//Id. 3: subtraction causes overflow (could not preserve required precision)
	EXPECT_THROW(CCDecimal("12345678901234567890123456789.01") - CCDecimal("0.853"),
			std::overflow_error);
//new
	EXPECT_THROW(
			CCDecimal("100012345678900000000000000000000")
		      - CCDecimal("12345678901234567890123456789"), std::overflow_error);


	EXPECT_THROW(CCDecimal("10000000000000000000000100000000") -
			                             CCDecimal("67890123"),
			std::overflow_error);
	//			           "10000000000000000000000032109877");


}

GROUP_TEST(Subtraction, CCDecimalTest, sub_zeroes) {
	testSub("1234", "1201", "33"); // 2 lz into the body
	testSub("1234.001", "1234", "0.001"); //6 lz into the tail
	testSub("1234", "134", "1100"); //2 tz only in the body
	testSub("1034", "34", "1000"); //3 tz into the head
}

GROUP_TEST(Subtraction, CCDecimalTest, sub_normal) {

	CCDecimal::setGlobalPrecision(MAX - 1);

	testSub("10", "3", "7");
	testSub("7689", "7600", "89");

	testSub("123.45", "12.3", "111.15");
	testSub("123.45", "3.456", "119.994");
	testSub("123.45", "0.00006", "123.44994"); ///dvsjdsdj,sdj
	testSub("123.45", "120.05", "3.4");
	testSub("1.000000000000000000000000000001", "0.00000000000000000000000000000175", //stop
			"0.9999999999999999999999999999992");

	testSub("1.000000000000000000000000000011", "0.00000000000000000000000000001175",
			"0.9999999999999999999999999999992");

	//testSub("7689", "89","7600");
	//testSub("7689", "7089","600");
	//testSub("123.456", "0.00005","123.45596" );//123.45595
	testSub("123.45", "0.00003", "123.44997");
	testSub("123.45", "23", "100.45");
	testSub("1.45", "0.55", "0.9");
	testSub("1", "0.1", "0.9");

	//test: tail_length == opSmall.used
	CCDecimal::setGlobalPrecision(1);
	testSub("12345678901234567890123456789.01", "0.003", "12345678901234567890123456789");
}

// Conversion #############################################
/* Constructs the same decimal in different ways.
 * It is expected to generate the same result in each case.*/
GROUP_TEST(Conversion, CCDecimalTest, onConstruction) {

	CCDecimal t0;
	CCDecimalTest::setDigits(t0, 3, 1, 0, 5);
	CCDecimalTest::setShift(t0, -1);

	CCDecimal t1;
	t1 = "10.5";

	CCDecimal t2("10.5");
	CCDecimal t3 = "10.5";
	CCDecimal t4 = 10.5;

	EXPECT_EQ(t0, t1);
	EXPECT_EQ(t1, t2);
	EXPECT_EQ(t2, t3);
	EXPECT_EQ(t3, t4);

}

GROUP_TEST(Conversion, CCDecimalTest, strToDec_normal) {

	cout << "cnv n 1" << endl;
	CCDecimal case0;
	CCDecimalTest::setDigits(case0, 3, 9, 8, 7);
	testFromStr("000987", case0);

	cout << "cnv n 2" << endl;
	CCDecimal case1;
	CCDecimalTest::setDigits(case1, 31, 1, 0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 9, 8, 7, 6, 5, 4, 3, 2,
			1, 0, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	CCDecimalTest::setShift(case1, 3);
	testFromStr("1098765432109876543210987654321000", case1);

	cout << "cnv n 3" << endl;
	CCDecimal case2;
	CCDecimalTest::setDigits(case2, 1, 7);
	CCDecimalTest::setShift(case2, -1);

	testFromStr("0.7", case2);

	cout << "cnv n 4" << endl;
	CCDecimal case3;
	CCDecimalTest::setDigits(case3, 2, 3, 6);
	CCDecimalTest::setShift(case3, -5);
	CCDecimalTest::setNegative(case3, true);

	testFromStr("-0.00036", case3);

	CCDecimal case4;
	CCDecimalTest::setDigits(case4, 1, 4);
	CCDecimalTest::setShift(case4, 1);

	testFromStr("40", case4);

	CCDecimal case5;
	CCDecimalTest::setDigits(case5, 31, 5, 2, 7, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 9, 8, 7, 6, 5, 4, 3,
			2, 1, 0, 9, 8, 7, 6, 5, 4, 3, 2);
	CCDecimalTest::setNegative(case5, true);
	CCDecimalTest::setShift(case5, -29);
	testFromStr("-52.79876543210987654321098765432", case5);

}

GROUP_TEST(Conversion, CCDecimalTest, strToDec_zero) {
	//zero
	CCDecimal zero;
//	testFromStr("", zero);
	testFromStr("0", zero);
	testFromStr(".0", zero);
	testFromStr("0.00", zero);
	testFromStr("0E5", zero);

}

GROUP_TEST(Conversion, CCDecimalTest, strToDec_overflow) {
	CCDecimal::setGlobalPrecision(2);

	//overflow (max size)
	EXPECT_THROW(CCDecimal("98765432109876543210987654321098"), std::overflow_error)<< " max size" << endl;
	EXPECT_THROW(CCDecimal("98765432109876543210987654321.298"), std::overflow_error)<< " max size" << endl;
	EXPECT_NO_THROW(CCDecimal("9876543210987654321098765432.1298"))<< " max size" << endl;
	EXPECT_NO_THROW(CCDecimal("987654321098765432109876543.21298"))<< " max size" << endl;
	EXPECT_THROW(CCDecimal("987654321098765432109876543212.98"), std::overflow_error)<< " max size" << endl;
	//overflow (max size)
	EXPECT_NO_THROW(CCDecimal("9.876543210987555989888854555666543210987654321098"))<< " max size" << endl;

	//exponent overflow
	EXPECT_THROW(CCDecimal("1e99999999999999999999"), std::overflow_error)<< " exp overflow" << endl;
	EXPECT_THROW(CCDecimal("0E9999999999999999999999999999"), std::overflow_error);
}

GROUP_TEST(Conversion, CCDecimalTest, strToDec_illegalArgument) {
	//overflow (max size)
	EXPECT_THROW(CCDecimal("1.5.7"), std::invalid_argument)<< " more than one decimal point";

	EXPECT_THROW(CCDecimal("15.5 9"), std::invalid_argument);

	cout << endl;
}

//constrFromStr ###########################################
GROUP_TEST(constructFromString, CCDecimal, fromStr_string_length_zero) {
	//Id. 0: string is empty
//	CCDecimal ccd1("");
//	CCDecimal ccd2;
//
//	EXPECT_EQ(ccd1, ccd2);
	EXPECT_THROW(CCDecimal ccd1(""), std::invalid_argument);
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_string_char_set) {
	//Id. 0: invalid character at the end
	EXPECT_THROW(CCDecimal ccd("12345E3_"), std::invalid_argument);
	//Id. 1: invalid character at the start
	EXPECT_THROW(CCDecimal ccd("  6789.1234"), std::invalid_argument);

	//Id. 2: most of all valid characters
	EXPECT_NO_THROW(CCDecimal ccd("+.23456789e-9"));
	//Id. 3: remaining valid characters
	EXPECT_NO_THROW(CCDecimal ccd("1E0"));
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_format_mantissa_has_digit) {
	//Id. 0: only sign
	EXPECT_THROW(CCDecimal ccd("-."), std::invalid_argument);
	//Id. 1: only sign, though with exponent
	EXPECT_THROW(CCDecimal ccd("+E1"), std::invalid_argument);
	//Id. 2: only decimal point
	EXPECT_THROW(CCDecimal ccd("."), std::invalid_argument);

	//Id. 3: valid number, with mantissa of 0
	EXPECT_NO_THROW(CCDecimal ccd("-0.E+1"));
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_format_single_sign_in_front) {
	//Id. 0: two signs at the start
	EXPECT_THROW(CCDecimal ccd("+-123456.0987"), std::invalid_argument);
	//Id. 1: two signs, one in front of the decimal point
	EXPECT_THROW(CCDecimal ccd("-4567+.0198"), std::invalid_argument);
	//Id. 2: two signs, one behind the decimal point
	EXPECT_THROW(CCDecimal ccd("+9876.-543"), std::invalid_argument);
	//Id. 3: one sign at the end
	EXPECT_THROW(CCDecimal ccd("5678.90123+"), std::invalid_argument);

	//Id. 4: '+' at the start
	EXPECT_NO_THROW(CCDecimal ccd("+3"));
	//Id. 5: no sign
	EXPECT_NO_THROW(CCDecimal ccd("4"));
	//Id. 6: '-' at the start
	EXPECT_NO_THROW(CCDecimal ccd("-5"));
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_format_single_point_in_middle) {
	//Id. 0: two decimal points, apart from one another
	EXPECT_THROW(CCDecimal ccd("-.87646756.7"), std::invalid_argument);
	//Id. 1: two decimal points, one behind the other
	EXPECT_THROW(CCDecimal ccd("+123456..0987"), std::invalid_argument);
	//Id. 2: decimal point before sign
	EXPECT_THROW(CCDecimal ccd(".-334795351"), std::invalid_argument);

	//Id. 3: point in the beginning
	EXPECT_NO_THROW(CCDecimal ccd("+.1234"));
	//Id. 4: point in the middle
	EXPECT_NO_THROW(CCDecimal ccd("12.34"));
	//Id. 5: point at the end
	EXPECT_NO_THROW(CCDecimal ccd("-1234."));
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_format_exponent) {
	//Id. 0: point
	EXPECT_THROW(CCDecimal ccd("123456e+789.0"), std::invalid_argument);
	//Id. 1: sign in the middle
	EXPECT_THROW(CCDecimal ccd("-23456E4327-734"), std::invalid_argument);
	//Id. 2: more than one sign
	EXPECT_THROW(CCDecimal ccd("63456832e-+25439"), std::invalid_argument);
	//Id. 3: exponent only
	EXPECT_THROW(CCDecimal ccd("E101"), std::invalid_argument);

	//Id. 4: negative exponent
	EXPECT_NO_THROW(CCDecimal ccd("123e-456"));
	//Id. 5: positive exponent
	EXPECT_NO_THROW(CCDecimal ccd("456E+78901"));
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_value_sign) {
	CCDecimal ccd1("0"), ccd3("+0"), ccd5("-0");

	CCDecimal ccd2, ccd4, ccd6;
	CCDecimalTest::setNegative(ccd6, true);

	//Id. 0: no sign
	EXPECT_EQ(ccd1, ccd2);
	//Id. 1: plus
	EXPECT_EQ(ccd3, ccd4);
	//Id. 2: minus
	EXPECT_EQ(ccd5, ccd6);
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_value_leading_zeroes) {
	CCDecimal ccd1("0012"), ccd3("00.34"), ccd5("0.056");

	CCDecimal ccd2, ccd4, ccd6;
	CCDecimalTest::setDigits(ccd2, 2, 1, 2);
	CCDecimalTest::setShift(ccd2, 0);
	CCDecimalTest::setDigits(ccd4, 2, 3, 4);
	CCDecimalTest::setShift(ccd4, -2);
	CCDecimalTest::setDigits(ccd6, 2, 5, 6);
	CCDecimalTest::setShift(ccd6, -3);

	//Id. 0: before point
	EXPECT_EQ(ccd1, ccd2);
	//Id. 1: in front of point
	EXPECT_EQ(ccd3, ccd4);
	//Id. 2: after point
	EXPECT_EQ(ccd5, ccd6);
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_value_trailing_zeroes) {
	CCDecimal ccd1("120.00"), ccd3("123.00"), ccd5("123.40");

	CCDecimal ccd2, ccd4, ccd6;
	CCDecimalTest::setDigits(ccd2, 2, 1, 2);
	CCDecimalTest::setShift(ccd2, 1);
	CCDecimalTest::setDigits(ccd4, 3, 1, 2, 3);
	CCDecimalTest::setShift(ccd4, 0);
	CCDecimalTest::setDigits(ccd6, 4, 1, 2, 3, 4);
	CCDecimalTest::setShift(ccd6, -1);

	//Id. 0: over point
	EXPECT_EQ(ccd1, ccd2);
	//Id. 1: up to point
	EXPECT_EQ(ccd3, ccd4);
	//Id. 2: behind point
	EXPECT_EQ(ccd5, ccd6);
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_value_lead_and_trail_0s) {
	CCDecimal ccd1("00123.4500");
	CCDecimal ccd3("000.00e123");

	CCDecimal ccd2, ccd4;
	CCDecimalTest::setDigits(ccd2, 5, 1, 2, 3, 4, 5);
	CCDecimalTest::setShift(ccd2, -2);

	//Id. 0: with other digits
	EXPECT_EQ(ccd1, ccd2);
	//Id. 1: only zeroes
	EXPECT_EQ(ccd3, ccd4);
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_value_exponent_equivalence) {
	CCDecimal ccd1("98765432123456789"), ccd2("9.8765432123456789e16"), ccd3("1.2345678987654321"),
			ccd4("12345678987654321E-16");

	CCDecimal ccd5, ccd6;
	CCDecimalTest::setDigits(ccd5, 17, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	CCDecimalTest::setDigits(ccd6, 17, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	CCDecimalTest::setShift(ccd6, -16);

	//Id. 0: over point
	EXPECT_EQ(ccd1, ccd2);
	EXPECT_EQ(ccd1, ccd5);
	//Id. 1: over point
	EXPECT_EQ(ccd3, ccd4);
	EXPECT_EQ(ccd3, ccd6);
}

GROUP_TEST(constructFromString, CCDecimal, fromStr_value_exponent_overflow) {

	//Id. 0: abs(exponent) next to INT_MAX
	EXPECT_THROW(CCDecimal ccd("1E-2147483648"), std::overflow_error);
	EXPECT_NO_THROW(CCDecimal ccd("1E-2147483647"));

	//Id. 0: abs(exponent + shift) > INT_MAX
	EXPECT_THROW(CCDecimal ccd(".1E-2147483647"), std::overflow_error);
	EXPECT_THROW(CCDecimal ccd(".01E-2147483647"), std::overflow_error);

	//Id. 0: zero with exponent should lead to overflow
	EXPECT_THROW(CCDecimal ccd("0e99999999999999999999"), std::overflow_error);

	//Id. 0: abs(exponent) > INT_MAX && abs(exponent + shift) <= INT_MAX
	EXPECT_NO_THROW(CCDecimal ccd(".01E2147483649"));
	EXPECT_NO_THROW(CCDecimal ccd("100E-2147483649"));
}

//toString, round #########################################
GROUP_TEST(Conversion, CCDecimalTest, toString_normal) {

	CCDecimal::setGlobalPrecision(2);

	//Id. 0: used = 0
	testToStr("0");

	//Id.2 : shift = 0
	testToStr("-1234");

	//Id. 3: shift > 0 (trailing zeroes)
	testToStr("678000");

	//Id. 4 : shift < 0,  (digits after dp)
	testToStr("-987.65");

	//Id. 5: shift < 0, used < -shift (leading zeroes)
	testToStr("0.09");

}

GROUP_TEST(Conversion, CCDecimalTest, toString_sci) {

	CCDecimal::setGlobalPrecision(2);

	testToStr("0", "0.00e+000", true);
	testToStr("8", "8.00e+000", true);
	testToStr("0.00012", "1.20e-004", true);
	testToStr("2500", "2.50e+003", true);
	testToStr("1239", "1.24e+003", true);

}

GROUP_TEST(Conversion, CCDecimalTest, toString_round) {
	CCDecimal::setGlobalPrecision(2);

	//Id.0 : round down
	testToStr("12.341", "12.34");

	//Id. 1: round down
	testToStr("12.345", "12.35");

	//Id. 2: round up
	testToStr("12.001", "12");

	//Id. 3: round down results in tz
	testToStr("19.995", "20");

	//Id. 4: round up results in tz
	testToStr("0.001", "0");

	//Id. 5: round down results in lz
	testToStr("0.009", "0.01"); //round up results in lz

	//new################################################################

	//carry into unoccupied space
	testToStr("99.99999", "100");

	EXPECT_THROW(CCDecimal("55").toString(-1), std::out_of_range);

	//CCDecimal("555").toString(std::numeric_limits<int32_t>::max());

}

//Multiplication ##########################################
GROUP_TEST(Multiplication, CCDecimalTest, mult_special) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: multiplying with zero
	testMult("145456", "0", "0"); //test case 0.1
	testMult("0", "58.888", "0"); //test case 0.2

	//Id. 1: multiplying with one
	testMult("566", "1", "566"); //test case 1.1
	testMult("1", "556.23", "556.23"); //test case 1.2

	//Id. 2: overflow due to precision constraint
	EXPECT_THROW(CCDecimal("123456789012345.6") * CCDecimal("1234567890123456.78"),
			std::overflow_error); // 152415787532388268983387568023.168);

	//Id. 3: overflow due to capacity constraint
	CCDecimal a = "1234567890123456";
	CCDecimal b = "12345678901234567";
	EXPECT_THROW(a * b, std::overflow_error);
	// 15241578753238825910684444703552

	//new ##############################################################################################################

	//Id. 4: reaches maximal capacity (MAX=31)
	testMult("1199999999999999",
			 "1199999999999999",
			 "1439999999999997600000000000001");

	//Id. 5: Can cut one more digit to prevent overflow after the last carry would have caused one.
	testMult("5.234567890123456","5.234567890123456","27.40070099631152972687092138394");


	//ov in MSD?
//	EXPECT_THROW(CCDecimal("9999999999999999") *
//			     CCDecimal("9999999999999999"),
//			std::overflow_error);
	//99999999999999980000000000000001

	//(CCDecimal("1199999999999999")*CCDecimal("1199999999999999"));

	//30 = 15+15

	//testMult("9", "9", "81");
	//testMult("21", "21", "441");

	testMult("12359999999999929", "556565565656789", "6879150391517872523844838367981");

	//a: b: c: : : :""
	//":a:b:" => vor

	//""
	//":"
	/*
	 * if (match){
	 *    vor match
	 * }
	 * else{
	 *  remainder
	 * }
	 */
	//
	/* This test case is very critical !!!!!
	 worstcase (msi1 + msi2 = MAX, ), bestcase (msi1 + msi2 = MAX - 1) 15 + 15 = 30 */
	//testMult("9.999999999999978", "9.999999999999978", "99.99999999999956000000000000048");
}

GROUP_TEST(Multiplication, CCDecimalTest, mult_tz) {
	CCDecimal::setGlobalPrecision(2);

//new#######################################################
	testMult("125", "8", "1000");
}
GROUP_TEST(Multiplication, CCDecimalTest, mult_accuracy) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: test accuracy by multiplying with the inverse
	testMult("5896", "0.0001696065128900949796472184531886", "0.9999999999999999999999999999999"); //accuracy
}

// Division ###############################################
GROUP_TEST(Division, CCDecimalTest, div) {
	CCDecimal::setGlobalPrecision(2);

	//### special

	//divide by zero
	EXPECT_THROW(CCDecimal(15.0) / CCDecimal(0.0), std::domain_error);
	EXPECT_THROW(CCDecimal(0.0) / CCDecimal(0.0), std::domain_error);

	//divide by itself
	testDiv("123.249", "123.249", "1");

	CCDecimal drei = 3.0;
	CCDecimal eins = 1.0;
	CCDecimal r = (eins / drei) * drei;
	isEqual(r,  eins);

	testDiv("31.8", "3.2", "9.9375");
	testDiv("2", "5", "0.4");
	testDiv("9000", "15", "600");
	testDiv("26", "0.004", "6500");
	testDiv("0.0009", "24", "0.0000375");
	testDiv("1", "3", "0.3333333333333333333333333333333");
	testDiv("5", "523456789012345678901234567891",
			"0.000000000000000000000000000009551886812728061632818220006512");
	testDiv("4568997890123456789012345678901", //should not throw exception ?
			"5234567890123456789012345678901", "0.8728510138810516803809035462051");

	testDiv("0", "15.398", "0");

	testDiv("1689.5775", "1689.5775", "1");
	testDiv("1000", "250", "4");
	EXPECT_THROW(CCDecimal("98.23") / CCDecimal(), std::logic_error);

	EXPECT_THROW(CCDecimal("800000000000000000000000000000") /
			     CCDecimal("7"), std::overflow_error);


	CCDecimal a = "12.29";
	CCDecimal m = "5";
	isEqual(a % m, CCDecimal("2.29"));




}

//Aritmetic ###############################################
GROUP_TEST(Arithmetic, CCDecimalTest, arith_addsub) {

	CCDecimal::setGlobalPrecision(2);

	//Id. 0: sub (neg, pos) (a = b)
	testSub("3", "3", "0");

	//Id. 1: add (neg, pos)
	testAdd("-7", "10", "3");

	//Id. 2: add (neg, neg)
	testAdd("-7", "-3", "-10");

	//Id. 3: add (neg, pos)
	testAdd("-7", "3", "-4");

	//Id. 4: add (neg, neg)
	testAdd("-7", "-10", "-17");

	//Id. 5: sub (neg, pos)
	testSub("-7", "10", "-17");

	//Id. 6: sub (neg, neg)
	testSub("-7", "-3", "-4"); //-2 to -3

	//Id. 7: sub (neg, pos)
	testSub("-7", "3", "-10");

	//Id. 8: sub (neg, neg)
	testSub("-7", "-10", "3");
}

GROUP_TEST(Arithmetic, CCDecimalTest, arith_mult) {
	CCDecimal::setGlobalPrecision(2);

	//Id 0: mult (pos, neg)
	testMult("25", "-15", "-375");

	//Id 1: mult (neg, pos)
	testMult("-0.28", "2600", "-728");

	//Id 2: mult (pos, pos)
	testMult("156.28", "8", "1250.24");

	//Id 3: mult (neg, neg)
	testMult("-.89", "-12", "10.68");

}

//Comparison #############################################
GROUP_TEST(Comparison, CCDecimalTest, lessThan) {

	CCDecimal::setGlobalPrecision(2);

	//Id. 0: neg, pos, abs(value) is equal		=> TRUE
	testLessThan("-15", "15", true);

	//Id. 1: pos, pos, abs(value) is smaller 	=> TRUE
	testLessThan("3", "9.243", true);

	//Id. 2: neg, neg, abs(value) is bigger		=> TRUE
	testLessThan("-20", "-12", true);

	//Id. 3: neg, pos, abs(value) is 0			=> FALSE
	testLessThan("-0", "0", false);

	//Id. 4: neg, neg, abs(value) is smaller	=> FALSE
	testLessThan("-0.9999", "-1", false);

	//Id. 5: pos, pos, abs(value) is bigger 	=> FALSE
	testLessThan("0.000256", "0.00012", false);

	//Id. 6: pos, neg, abs(value) is equal		=> FALSE
	testLessThan("3500", "-3500", false);
}

GROUP_TEST(Comparison, CCDecimalTest, moreThan) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: neg, pos, abs(value) is equal		=> TRUE
	testMoreThan("-15", "15", false);

	//Id. 1: pos, pos, abs(value) is smaller 	=> TRUE
	testMoreThan("3", "9.243", false);

	//Id. 2: neg, neg, abs(value) is bigger		=> TRUE
	testMoreThan("-20", "-12", false);

	//Id. 3: neg, pos, abs(value) is 0			=> FALSE
	testMoreThan("0", "-0", false);

	//Id. 4: neg, neg, abs(value) is smaller	=> FALSE
	testMoreThan("-0.9999", "-1", true);

	//Id. 5: pos, pos, abs(value) is bigger 	=> FALSE
	testMoreThan("0.000256", "0.00012", true);

	//Id. 6: pos, neg, abs(value) is equal		=> FALSE
	testMoreThan("3500", "-3500", true);
}

GROUP_TEST(Comparison, CCDecimalTest, equal) {
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: different sign
	testEqual("15.26", "-15.26", false);

	//Id. 1.0: different shift
	testEqual("35", "350", false);

	//Id. 1.1: different shift
	testEqual("-86.59", "-8.659", false);

	//Id. 2: different used
	testEqual("99.2", "99.21", false);

	//Id. 3.0: equal
	testEqual("-0.0035", "-0.0035", true);

	//Id. 3.1: equal
	testEqual("87200", "87200", true);

	//Id. 3.2: equal
	testEqual("-0", "0", true);

}

//Operators ##############################################
GROUP_TEST(Operators, CCDecimalTest, op_consistency) {
	CCDecimal a = "15.23";
	CCDecimal b = "3.4";

	CCDecimal c = a;
	isEqual(c /= b, a / b);

	c = a;
	isEqual(c *= b, a * b);

	c = a;
	isEqual(c += b, a + b);

	c = a;
	isEqual(c -= b, a - b);

	c = a;
	isEqual(c %= b, a % b);

}
GROUP_TEST(Operators, CCDecimalTest, op_incdec) {

	CCDecimal d = "0.026";
	isEqual(d++, CCDecimal("0.026"));
	isEqual(++d, CCDecimal("2.026"));
	isEqual(d--, CCDecimal("2.026"));
	isEqual(--d, CCDecimal("0.026"));
}

GROUP_TEST(Operators, CCDecimalTest, op_stream){
	CCDecimal::setGlobalPrecision(2);

	//Id. 0: not scientific notation
	cout << CCDecimal("123456") << endl;

	//Id. 1: scientific notation
	cout << std::scientific << CCDecimal("123.4567") << endl;
}


GROUP_TEST(Conversions, CCDecimalTest, toDouble){
	testToDouble("1234.123456789", 1234.123456789);
	testToDouble("123456789012345678901234567890100000000000000000", 123456789012345680000000000000000000000000000000.0);
}

