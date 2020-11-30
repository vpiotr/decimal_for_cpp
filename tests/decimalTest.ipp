/////////////////////////////////////////////////////////////////////////////
// Name:        decimalTest.ipp
// Purpose:     Test decimal type.
// Author:      Piotr Likus
// Modified by:
// Created:     31/10/2010
// Licence:     BSD
/////////////////////////////////////////////////////////////////////////////

#include "decimal.h"
#include <boost/integer_traits.hpp>
#include <type_traits>

/*
template <typename T>
std::string toString(const T &arg) {
    std::ostringstream	out;
    out << arg;
    return(out.str());
}
*/

std::string toString(unsigned int arg) {
    std::ostringstream	out;
    out << arg;
    return(out.str());
}

/*
template <typename T>
T fromString (const std::string &str) {
    std::istringstream is(str);
    T t;
    is >> t;
    return t;
}
*/

BOOST_AUTO_TEST_CASE(decimalAbout) {
        using namespace dec;
        using namespace std;

        // the following declares currency variable with 2 decimal points
        // initialized with integer value (can be also floating-point)
        decimal<2> value(143125);

        // displays: Value #1 is: 143125.00
        cout << "Value #1 is: " << value << endl;

        // declare precise value with digits after decimal point
        decimal<2> b("0.11");

        // perform calculations as with any other numeric type
        value += b;

        // displays: Value #2 is: 143125.11
        cout << "Value #2 is: " << value << endl;

        // automatic rounding performed here
        value /= 1000;

        // displays: Value #3 is: 143.13
        cout << "Value #3 is: " << value << endl;

        // integer multiplication and division can be used directly in expression
        // when integer is on right side
        // displays: Value: 143.13 * 2 is: 286.26
        cout << "Value: " << value << " * 2 is: " << (value * 2) << endl;

        // to use integer on left side you need to cast it
        // displays: Value: 2 * 143.13 is: 286.26
        cout << "Value: 2 * " << value << " is: " << (decimal_cast<2>(2) * value) << endl;

        // to use non-integer constants in expressions you need to use decimal_cast
        value = value * decimal_cast<2>("3.33") / decimal_cast<2>(333.0);

        // displays: Value #4 is: 1.43
        cout << "Value #4 is: " << value << endl;

        // to mix decimals with different precision use decimal_cast
        // it will round result automatically
        decimal<6> exchangeRate(12.1234);
        value = decimal_cast<2>(decimal_cast<6>(value) * exchangeRate);

        // displays: Value #5 is: 17.34
        cout << "Value #5 is: " << value << endl;

        // with doubles you would have to perform rounding each time it is required:
        double dvalue = 143125.0;
        dvalue += 0.11;
        dvalue /= 1000.0;
        dvalue = round(dvalue * 100.0)/100.0;
        dvalue = (dvalue * 3.33) / 333.0;
        dvalue = round(dvalue * 100.0)/100.0;
        dvalue = dvalue * 12.1234;
        dvalue = round(dvalue * 100.0)/100.0;

        // displays: Value #5 calculated with double is: 17.34
        cout << "Value #5 calculated with double is: " << fixed << setprecision(2) << dvalue << endl;

        // supports optional strong typing, e.g.
        // depending on configuration mixing precision can be forbidden
        // or handled automatically
        decimal<2> d2("12.03");
        decimal<4> d4("123.0103");

        // compiles always
        d2 += d2;
        d2 += decimal_cast<2>(d4);
        d4 += decimal_cast<4>(d2);

        #if DEC_TYPE_LEVEL >= 2
        // potential precision loss
        // this will fail to compile if you define DEC_TYPE_LEVEL = 0 or 1
        d2 += d4;
        #endif

        #if DEC_TYPE_LEVEL >= 1
        // (possibly unintentional) mixed precision without type casting
        // this will fail to compile if you define DEC_TYPE_LEVEL = 0
        d4 += d2;
        #endif

        // for default setup displays: mixed d2 = 417.15
        cout << "mixed d2 = " << d2 << endl;
        // for default setup displays: mixed d4 = 687.2303
        cout << "mixed d4 = " << d4 << endl;
}

BOOST_AUTO_TEST_CASE(decimalArithmetic)
        {
                dec::decimal<2> balance;
                dec::decimal<2> a(123);
                dec::decimal<2> t(246);

                balance = 0;
                BOOST_TEST_MESSAGE("balance-0: " << toString(balance));
                BOOST_CHECK(balance == dec::decimal2(0));

                balance += a;
                BOOST_TEST_MESSAGE("balance-1: " << toString(balance));
                BOOST_CHECK(balance != t);

                balance += a;
                BOOST_TEST_MESSAGE("balance-2: " << toString(balance));
                BOOST_CHECK(balance == t);

                balance /= dec::decimal2(2);
                BOOST_TEST_MESSAGE("balance-3: " << toString(balance));
                BOOST_CHECK(balance == a);

                balance *= dec::decimal2(2);
                BOOST_TEST_MESSAGE("balance-4: " << toString(balance));
                BOOST_CHECK(balance == t);

                balance = balance / dec::decimal2(3);
                BOOST_TEST_MESSAGE("balance-5: " << toString(balance));
                BOOST_CHECK(balance == dec::decimal2(82));

                balance = balance + dec::decimal2(3);
                BOOST_TEST_MESSAGE("balance-6: " << toString(balance));
                BOOST_CHECK(balance == dec::decimal2(85));

                balance = balance - dec::decimal2(90);
                BOOST_TEST_MESSAGE("balance-7: " << toString(balance));
                BOOST_CHECK(balance == dec::decimal2(-5));

                balance = balance * dec::decimal2(9);
                BOOST_TEST_MESSAGE("balance-8: " << toString(balance));
                BOOST_CHECK(balance == dec::decimal2(-45));
                BOOST_CHECK(balance == dec::decimal2(-45.0));

                // check div
                balance = balance / dec::decimal2(10.0);
                BOOST_TEST_MESSAGE("balance-9: " << toString(balance));
                BOOST_CHECK(balance == dec::decimal2(-4.5));

                balance *= dec::decimal2(3.0);
                BOOST_CHECK(balance == dec::decimal2(-13.5));

                BOOST_CHECK(sizeof(dec::decimal2::raw_data_t) > 0);

                balance /= 10;
                BOOST_CHECK(balance == dec::decimal2(-1.35));

                balance -= dec::decimal2(0.1);
                BOOST_CHECK(balance == dec::decimal2(-1.45));

                balance = balance - dec::decimal2(3);
                BOOST_CHECK(balance == dec::decimal2(-4.45));

                BOOST_CHECK(balance.abs() == dec::decimal2(4.45));

                BOOST_CHECK(balance.getAsInteger() == -4);

                balance = +balance;
                BOOST_CHECK(balance == dec::decimal2(-4.45));

                balance = -balance;
                BOOST_CHECK(balance == dec::decimal2(4.45));

                BOOST_TEST_MESSAGE("balance-end: " << toString(balance));
        }

BOOST_AUTO_TEST_CASE(decimalMultiplyPrec)
        {
                // check mult prec
                BOOST_CHECK_EQUAL(dec::decimal<4>("0.0001") * dec::decimal<4>("1.0000"), dec::decimal<4>("0.0001"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("0.0010") * dec::decimal<4>("1.1000"), dec::decimal<4>("0.0011"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("2.0100") * dec::decimal<4>("1.1000"), dec::decimal<4>("2.211"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("2.5010") * dec::decimal<4>("1.5000"), dec::decimal<4>("3.7515"));

                // check mult neg 1
                BOOST_CHECK_EQUAL(dec::decimal<4>("-0.0001") * dec::decimal<4>("1.0000"), dec::decimal<4>("-0.0001"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-0.0010") * dec::decimal<4>("1.1000"), dec::decimal<4>("-0.0011"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-2.0100") * dec::decimal<4>("1.1000"), dec::decimal<4>("-2.211"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-2.5010") * dec::decimal<4>("1.5000"), dec::decimal<4>("-3.7515"));

                // check mult neg 2
                BOOST_CHECK_EQUAL(dec::decimal<4>("0.0001") * dec::decimal<4>("-1.0000"), dec::decimal<4>("-0.0001"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("0.0010") * dec::decimal<4>("-1.1000"), dec::decimal<4>("-0.0011"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("2.0100") * dec::decimal<4>("-1.1000"), dec::decimal<4>("-2.211"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("2.5010") * dec::decimal<4>("-1.5000"), dec::decimal<4>("-3.7515"));

                // check mult both neg
                BOOST_CHECK_EQUAL(dec::decimal<4>("-0.0001") * dec::decimal<4>("-1.0000"), dec::decimal<4>("0.0001"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-0.0010") * dec::decimal<4>("-1.1000"), dec::decimal<4>("0.0011"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-2.0100") * dec::decimal<4>("-1.1000"), dec::decimal<4>("2.211"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-2.5010") * dec::decimal<4>("-1.5000"), dec::decimal<4>("3.7515"));

                // check medium prec - near 32 bits
                BOOST_CHECK_EQUAL(dec::decimal<9>("1.5") * dec::decimal<9>("2.1000"), dec::decimal<9>("3.15"));
                BOOST_CHECK_EQUAL(dec::decimal<9>("1.000000005") * dec::decimal<9>("2"), dec::decimal<9>("2.000000010"));
                BOOST_CHECK_EQUAL(dec::decimal<9>("1.80000001")  * dec::decimal<9>("2"), dec::decimal<9>("3.600000020"));
                BOOST_CHECK_EQUAL(dec::decimal<9>("1.80000001")  * dec::decimal<9>("2"), dec::decimal<9>("3.600000020"));

                // check as dec 6
                BOOST_CHECK_EQUAL(dec::decimal<6>("35000.000005") * dec::decimal<6>("54123.654133"), dec::decimal<6>("1894327894.925618"));
        }

BOOST_AUTO_TEST_CASE(decimalMultiplyInt)
        {
                BOOST_CHECK_EQUAL(dec::decimal<4>("0.0001") * 2, dec::decimal<4>("0.0002"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("0.1001") * 3, dec::decimal<4>("0.3003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("1.1001") * 3, dec::decimal<4>("3.3003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("1.0001") * 3, dec::decimal<4>("3.0003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("1.0") * 3, dec::decimal<4>("3.0"));

                BOOST_CHECK_EQUAL(dec::decimal<4>("0.0001") * -2, dec::decimal<4>("-0.0002"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("0.1001") * -3, dec::decimal<4>("-0.3003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("1.1001") * -3, dec::decimal<4>("-3.3003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("1.0001") * -3, dec::decimal<4>("-3.0003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("1.0") * -3, dec::decimal<4>("-3.0"));

                BOOST_CHECK_EQUAL(dec::decimal<4>("-0.0001") * 2, dec::decimal<4>("-0.0002"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-0.1001") * 3, dec::decimal<4>("-0.3003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-1.1001") * 3, dec::decimal<4>("-3.3003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-1.0001") * 3, dec::decimal<4>("-3.0003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-1.0") * 3, dec::decimal<4>("-3.0"));

                BOOST_CHECK_EQUAL(dec::decimal<4>("-0.0001") * -2, dec::decimal<4>("0.0002"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-0.1001") * -3, dec::decimal<4>("0.3003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-1.1001") * -3, dec::decimal<4>("3.3003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-1.0001") * -3, dec::decimal<4>("3.0003"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("-1.0") * -3, dec::decimal<4>("3.0"));
        }

BOOST_AUTO_TEST_CASE(decimalDivInt)
        {
                BOOST_CHECK_EQUAL(dec::decimal<4>("1.0001") / 2,  dec::decimal<4>("0.5001"));
                BOOST_CHECK_EQUAL(dec::decimal<4>("2.0010") / 2L, dec::decimal<4>("1.0005"));
        }

BOOST_AUTO_TEST_CASE(decimalWithExponent)
        {
                // build positive values
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, 0),  dec::decimal<4>("11"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, 2),  dec::decimal<4>("1100"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, -2),  dec::decimal<4>("0.11"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, 1),  dec::decimal<4>("110"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, -1),  dec::decimal<4>("1.1"));

                // build negative values
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, 0),  dec::decimal<4>("-11"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, 2),  dec::decimal<4>("-1100"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, -2),  dec::decimal<4>("-0.11"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, 1),  dec::decimal<4>("-110"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, -1),  dec::decimal<4>("-1.1"));

                // build zero
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(0, -1),  dec::decimal<4>("0"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(0, 2),  dec::decimal<4>("0"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(0, 0),  dec::decimal<4>("0"));

                // add, get, set
                dec::decimal<4> temp;
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, 111213, -3),  dec::decimal<4>("111.213"));
                dec::decimal<4> a;
                a.setWithExponent(30, -2);
                temp += a;
                BOOST_CHECK_EQUAL(temp,  dec::decimal<4>("111.5130"));

                dec::DEC_INT64 m;
                int e;
                temp.getWithExponent(m, e);
                BOOST_CHECK_EQUAL(m, 111513);
                BOOST_CHECK_EQUAL(e, -3);

                // rounding
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, 111213, -5),  dec::decimal<4>("1.1121"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, 111215, -5),  dec::decimal<4>("1.1122"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, -111213, -5),  dec::decimal<4>("-1.1121"));
                BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, -111215, -5),  dec::decimal<4>("-1.1122"));
        }

BOOST_AUTO_TEST_CASE(decimalRounding)
        {
                dec::decimal<2> balance;

                balance = dec::decimal2(3.156);
                BOOST_CHECK(balance == dec::decimal2(3.16));

                balance = dec::decimal2("3.155");
                BOOST_CHECK(balance == dec::decimal2(3.16));

                balance = dec::decimal2(3.154);
                BOOST_CHECK(balance == dec::decimal2(3.15));

                balance = dec::decimal2(-3.156);
                BOOST_CHECK(balance == dec::decimal2(-3.16));

                balance = dec::decimal2("-3.155");
                BOOST_CHECK(balance == dec::decimal2(-3.16));

                balance = dec::decimal2(-3.154);
                BOOST_CHECK(balance == dec::decimal2(-3.15));

                balance = dec::decimal2(3.67);
                balance = balance * dec::decimal2(3.67);
                BOOST_CHECK(balance == dec::decimal2(13.47));

                balance = dec::decimal2(3.67);
                balance = balance / dec::decimal2(1.27);
                BOOST_CHECK(balance == dec::decimal2(2.89));

                // test precision mixing
                balance = dec::decimal2(3.67);
                dec::decimal<6> crate(1.4567);
                balance = dec::decimal_cast<2>((dec::decimal_cast<6>(balance) * crate));
                BOOST_CHECK(balance == dec::decimal2(5.35));

                // test int overflow
                dec::decimal<10> largeValue(1311.12176161);
                balance = dec::decimal_cast<2>(largeValue * dec::decimal_cast<10>(10));
                BOOST_CHECK(balance == dec::decimal2(13111.22));

                // test serialization
                dec::decimal<12> longDec1("6321311.121761616789");
                dec::decimal<12> longDec2;

                std::string s = toString(longDec1);
                longDec2 = dec::fromString<dec::decimal<12> >(s);

                BOOST_TEST_MESSAGE("longDec1: " << longDec1);
                BOOST_TEST_MESSAGE("longDec1 as string: " << s);
                BOOST_TEST_MESSAGE("longDec2: " << longDec2);
                BOOST_CHECK_EQUAL(longDec1, longDec2);
        }

BOOST_AUTO_TEST_CASE(decimalHighEndVals)
        {
                // with decimal<16> we need to provide constants with strings not floating points.
                dec::decimal<16> a;
                dec::decimal<16> b;
                dec::decimal<16> c;

                a = dec::decimal<16>("1.1111111111111111");
                b = dec::decimal<16>(2.0);

                c = a + b;
                BOOST_CHECK(c == dec::decimal<16>("3.1111111111111111"));

                c = a - b;
                BOOST_CHECK(c == dec::decimal<16>("-0.8888888888888889"));

                c = a * b;
                BOOST_CHECK(c == dec::decimal<16>("2.2222222222222222"));

                c = a / b;
                BOOST_CHECK(c == dec::decimal<16>("0.5555555555555556"));
        }

BOOST_AUTO_TEST_CASE(decimalUHighEndVals)
        {
                dec::decimal<17> a;
                dec::decimal<17> b;
                dec::decimal<17> c;

                a = dec::decimal<17>("1.1");
                b = dec::decimal<17>("2.0");

                c = a + b;
                BOOST_CHECK(c == dec::decimal<17>("3.1"));

                c = a - b;
                BOOST_CHECK(c ==dec::decimal<17>("-0.9"));

                c = a * b;
                BOOST_CHECK(c == dec::decimal<17>("2.2"));

                c = a / b;
                BOOST_CHECK(c == dec::decimal<17>("0.55"));
        }

BOOST_AUTO_TEST_CASE(decimalMaxUIntCtor)
        {
                unsigned int uint_max = boost::integer_traits<unsigned int>::const_max;
                std::string uint_max_txt = toString(uint_max);
                dec::decimal<2> a(uint_max);
                BOOST_CHECK_EQUAL( a, dec::decimal<2>(uint_max_txt) );
                BOOST_CHECK_EQUAL( a.getAsInteger(), uint_max);
        }

BOOST_AUTO_TEST_CASE(decimalAsInteger)
        {
                // rounded value
                dec::decimal<6> a;
                a = dec::decimal<6>("2305843009213.693952");
                dec::DEC_INT64 expectedValue = 2305843009214;
                BOOST_CHECK_EQUAL( a.getAsInteger(), expectedValue);

                // int64 value (>0)
                expectedValue = 23058430092136939;
                dec::decimal<1> b = dec::decimal<1>(expectedValue);
                BOOST_CHECK_EQUAL( b.getAsInteger(), expectedValue);

                // int64 value (<0)
                expectedValue = -23058430092136939;
                dec::decimal<1> c = dec::decimal<1>(expectedValue);
                BOOST_CHECK_EQUAL( c.getAsInteger(), expectedValue);
        }

// test with values internally > 2^32
BOOST_AUTO_TEST_CASE(decimalMidOverflow)
        {
                dec::decimal<6> a;
                dec::decimal<6> b;
                dec::decimal<6> c;
                dec::decimal<6> expected;

                a = dec::decimal<6>("2305843009213.693952"); // 2^61
                b = dec::decimal<6>("2.000001"); // value < 2^32

                c = a * b;
                expected = dec::decimal<6>("4611688324270.397118");

                // overflow, so calculation via floating point, error will be <> 0, but should be small
                BOOST_CHECK((c - expected).abs() < dec::decimal<6>("1.0"));
        }

BOOST_AUTO_TEST_CASE(decimalDefRoundPolicy)
        {
                using namespace dec;
                decimal<1, def_round_policy> a("0.5");
                decimal<1, def_round_policy> b("2");
                decimal<1, def_round_policy> c;

                c = a / b;
                dec::decimal<1, def_round_policy> expected("0.3");
                BOOST_CHECK(c == expected);
        }

BOOST_AUTO_TEST_CASE(decimalNoRoundPolicy)
        {
                using namespace dec;
                decimal<1, null_round_policy> a("0.5");
                decimal<1, null_round_policy> b("2");
                decimal<1, null_round_policy> c;

                c = a / b;
                dec::decimal<1, null_round_policy> expected("0.2");
                BOOST_CHECK(c == expected);
        }

template<int Prec, class RoundPolicy>
class round_tester_t {
public:
    round_tester_t(const std::string &name): m_name(name) {  }

    void test_assign(const std::string &src_value, const std::string &expected_value) {
        dec::decimal<Prec, RoundPolicy> a;
        dec::decimal<Prec, RoundPolicy> expected;

        a = dec::decimal<Prec, RoundPolicy>(src_value);
        expected = dec::decimal<Prec, RoundPolicy>(expected_value);

        BOOST_CHECK_MESSAGE( a == expected, "assign decimal<" << Prec << ", " << m_name << ">(" << src_value << ") is " << a << ", expecting: " << expected_value );
    }

    void test_div2(const std::string &src_value, const std::string &expected_value) {
        dec::decimal<Prec, RoundPolicy> a;
        dec::decimal<Prec, RoundPolicy> expected;

        a = dec::decimal<Prec, RoundPolicy>(src_value) / dec::decimal<Prec, RoundPolicy>("2.0");
        expected = dec::decimal<Prec, RoundPolicy>(expected_value);

        BOOST_CHECK_MESSAGE( a == expected, "div2 decimal<" << Prec << ", " << m_name << ">(" << src_value << ") is " << a << ", expecting: " << expected_value );
    }

    void test_div(const std::string &a_value, int b_value, const std::string &expected_value) {
        dec::decimal<Prec, RoundPolicy> a;
        dec::decimal<Prec, RoundPolicy> expected;

        a = dec::decimal<Prec, RoundPolicy>(a_value) / dec::decimal_cast<Prec, RoundPolicy>(b_value);
        expected = dec::decimal<Prec, RoundPolicy>(expected_value);

        BOOST_CHECK_MESSAGE( a == expected, "divn decimal<" << Prec << ", " << m_name << ">(" << a_value << ", " << b_value << ") is " << a << ", expecting: " << expected_value );
    }

private:
    std::string m_name;
};

BOOST_AUTO_TEST_CASE(decimalRoundingPolicyOther)
        {
                using namespace dec;
                {
                    round_tester_t<1, def_round_policy> tester("default");

                    tester.test_assign("0.16", "0.2");
                    tester.test_assign("0.15", "0.2");
                    tester.test_assign("0.14", "0.1");

                    tester.test_assign("0.06", "0.1");
                    tester.test_assign("0.05", "0.1");
                    tester.test_assign("0.04", "0.0");

                    tester.test_assign("-0.04", "-0.0");
                    tester.test_assign("-0.05", "-0.1");
                    tester.test_assign("-0.06", "-0.1");

                    tester.test_assign("-0.14", "-0.1");
                    tester.test_assign("-0.15", "-0.2");
                    tester.test_assign("-0.16", "-0.2");

                    tester.test_div("3.20", 20, "0.2");
                    tester.test_div("3.00", 20, "0.2");
                    tester.test_div("2.80", 20, "0.1");

                    tester.test_div("1.20", 20, "0.1");
                    tester.test_div("1.00", 20, "0.1");
                    tester.test_div("0.80", 20, "0.0");

                    tester.test_div("-0.80", 20, "-0.0");
                    tester.test_div("-1.00", 20, "-0.1");
                    tester.test_div("-1.20", 20, "-0.1");

                    tester.test_div("-2.80", 20, "-0.1");
                    tester.test_div("-3.00", 20, "-0.2");
                    tester.test_div("-3.20", 20, "-0.2");
                }

                {
                    round_tester_t<1, half_down_round_policy> tester("half_down");

                    tester.test_assign("0.16", "0.2");
                    tester.test_assign("0.15", "0.1");
                    tester.test_assign("0.14", "0.1");

                    tester.test_assign("0.06", "0.1");
                    tester.test_assign("0.05", "0.0");
                    tester.test_assign("0.04", "0.0");

                    tester.test_assign("-0.04", "0.0");
                    tester.test_assign("-0.05", "-0.1");
                    tester.test_assign("-0.06", "-0.1");

                    tester.test_assign("-0.14", "-0.1");
                    tester.test_assign("-0.15", "-0.2");
                    tester.test_assign("-0.16", "-0.2");

                    tester.test_div("3.20", 20, "0.2");
                    tester.test_div("3.00", 20, "0.1");
                    tester.test_div("2.80", 20, "0.1");

                    tester.test_div("1.20", 20, "0.1");
                    tester.test_div("1.00", 20, "0.0");
                    tester.test_div("0.80", 20, "0.0");

                    tester.test_div("-0.80", 20, "0.0");
                    tester.test_div("-1.00", 20, "-0.1");
                    tester.test_div("-1.20", 20, "-0.1");

                    tester.test_div("-2.80", 20, "-0.1");
                    tester.test_div("-3.00", 20, "-0.2");
                    tester.test_div("-3.20", 20, "-0.2");
                }

                {
                    round_tester_t<1, half_up_round_policy> tester("half_up");

                    tester.test_assign("0.16", "0.2");
                    tester.test_assign("0.15", "0.2");
                    tester.test_assign("0.14", "0.1");

                    tester.test_assign("0.06", "0.1");
                    tester.test_assign("0.05", "0.1");
                    tester.test_assign("0.04", "0.0");

                    tester.test_assign("-0.04", "0.0");
                    tester.test_assign("-0.05", "-0.0");
                    tester.test_assign("-0.06", "-0.1");

                    tester.test_assign("-0.14", "-0.1");
                    tester.test_assign("-0.15", "-0.1");
                    tester.test_assign("-0.16", "-0.2");

                    tester.test_div("3.20", 20, "0.2");
                    tester.test_div("3.00", 20, "0.2");
                    tester.test_div("2.80", 20, "0.1");

                    tester.test_div("1.20", 20, "0.1");
                    tester.test_div("1.00", 20, "0.1");
                    tester.test_div("0.80", 20, "0.0");

                    tester.test_div("-0.80", 20, "0.0");
                    tester.test_div("-1.00", 20, "-0.0");
                    tester.test_div("-1.20", 20, "-0.1");

                    tester.test_div("-2.80", 20, "-0.1");
                    tester.test_div("-3.00", 20, "-0.1");
                    tester.test_div("-3.20", 20, "-0.2");
                }

                {
                    round_tester_t<1, ceiling_round_policy> tester("ceiling");

                    tester.test_assign("0.16", "0.2");
                    tester.test_assign("0.15", "0.2");
                    tester.test_assign("0.14", "0.2");

                    tester.test_assign("0.06", "0.1");
                    tester.test_assign("0.05", "0.1");
                    tester.test_assign("0.04", "0.1");

                    tester.test_assign("-0.04", "0.0");
                    tester.test_assign("-0.05", "0.0");
                    tester.test_assign("-0.06", "0.0");

                    tester.test_assign("-0.14", "-0.1");
                    tester.test_assign("-0.15", "-0.1");
                    tester.test_assign("-0.16", "-0.1");

                    tester.test_div("3.20", 20, "0.2");
                    tester.test_div("3.00", 20, "0.2");
                    tester.test_div("2.80", 20, "0.2");

                    tester.test_div("1.20", 20, "0.1");
                    tester.test_div("1.00", 20, "0.1");
                    tester.test_div("0.80", 20, "0.1");

                    tester.test_div("-0.80", 20, "0.0");
                    tester.test_div("-1.00", 20, "0.0");
                    tester.test_div("-1.20", 20, "0.0");

                    tester.test_div("-2.80", 20, "-0.1");
                    tester.test_div("-3.00", 20, "-0.1");
                    tester.test_div("-3.20", 20, "-0.1");
                }

                {
                    round_tester_t<1, floor_round_policy> tester("floor");

                    tester.test_assign("0.16", "0.1");
                    tester.test_assign("0.15", "0.1");
                    tester.test_assign("0.14", "0.1");

                    tester.test_assign("0.06", "0.0");
                    tester.test_assign("0.05", "0.0");
                    tester.test_assign("0.04", "0.0");

                    tester.test_assign("-0.04", "-0.1");
                    tester.test_assign("-0.05", "-0.1");
                    tester.test_assign("-0.06", "-0.1");

                    tester.test_assign("-0.14", "-0.2");
                    tester.test_assign("-0.15", "-0.2");
                    tester.test_assign("-0.16", "-0.2");

                    tester.test_div("3.20", 20, "0.1");
                    tester.test_div("3.00", 20, "0.1");
                    tester.test_div("2.80", 20, "0.1");

                    tester.test_div("1.20", 20, "0.0");
                    tester.test_div("1.00", 20, "0.0");
                    tester.test_div("0.80", 20, "0.0");

                    tester.test_div("-0.80", 20, "-0.1");
                    tester.test_div("-1.00", 20, "-0.1");
                    tester.test_div("-1.20", 20, "-0.1");

                    tester.test_div("-2.80", 20, "-0.2");
                    tester.test_div("-3.00", 20, "-0.2");
                    tester.test_div("-3.20", 20, "-0.2");
                }

                {
                    round_tester_t<1, round_down_round_policy> tester("round-down");

                    tester.test_assign("0.16", "0.1");
                    tester.test_assign("0.15", "0.1");
                    tester.test_assign("0.14", "0.1");

                    tester.test_assign("0.06", "0.0");
                    tester.test_assign("0.05", "0.0");
                    tester.test_assign("0.04", "0.0");

                    tester.test_assign("-0.04", "0.0");
                    tester.test_assign("-0.05", "0.0");
                    tester.test_assign("-0.06", "0.0");

                    tester.test_assign("-0.14", "-0.1");
                    tester.test_assign("-0.15", "-0.1");
                    tester.test_assign("-0.16", "-0.1");

                    tester.test_div("3.20", 20, "0.1");
                    tester.test_div("3.00", 20, "0.1");
                    tester.test_div("2.80", 20, "0.1");

                    tester.test_div("1.2", 20, "0.0");
                    tester.test_div("1.0", 20, "0.0");
                    tester.test_div("0.8", 20, "0.0");

                    tester.test_div("-0.80", 20, "0.0");
                    tester.test_div("-1.00", 20, "0.0");
                    tester.test_div("-1.20", 20, "0.0");

                    tester.test_div("-2.80", 20, "-0.1");
                    tester.test_div("-3.00", 20, "-0.1");
                    tester.test_div("-3.20", 20, "-0.1");
                }

                {
                    round_tester_t<1, round_up_round_policy> tester("round-up");

                    tester.test_assign("0.16", "0.2");
                    tester.test_assign("0.15", "0.2");
                    tester.test_assign("0.14", "0.2");

                    tester.test_assign("0.06", "0.1");
                    tester.test_assign("0.05", "0.1");
                    tester.test_assign("0.04", "0.1");

                    tester.test_assign("-0.04", "-0.1");
                    tester.test_assign("-0.05", "-0.1");
                    tester.test_assign("-0.06", "-0.1");

                    tester.test_assign("-0.14", "-0.2");
                    tester.test_assign("-0.15", "-0.2");
                    tester.test_assign("-0.16", "-0.2");

                    tester.test_div("3.20", 20, "0.2");
                    tester.test_div("3.00", 20, "0.2");
                    tester.test_div("2.80", 20, "0.2");

                    tester.test_div("1.20", 20, "0.1");
                    tester.test_div("1.00", 20, "0.1");
                    tester.test_div("0.80", 20, "0.1");

                    tester.test_div("-0.80", 20, "-0.1");
                    tester.test_div("-1.00", 20, "-0.1");
                    tester.test_div("-1.20", 20, "-0.1");

                    tester.test_div("-2.80", 20, "-0.2");
                    tester.test_div("-3.00", 20, "-0.2");
                    tester.test_div("-3.20", 20, "-0.2");
                }

                {
                    round_tester_t<1, half_even_round_policy> tester("half-even");

                    tester.test_assign("0.16", "0.2");
                    tester.test_assign("0.15", "0.2");
                    tester.test_assign("0.14", "0.1");

                    tester.test_assign("0.06", "0.1");
                    tester.test_assign("0.05", "0.0");
                    tester.test_assign("0.04", "0.0");

                    tester.test_assign("-0.04", "0.0");
                    tester.test_assign("-0.05", "0.0");
                    tester.test_assign("-0.06", "-0.1");

                    tester.test_assign("-0.14", "-0.1");
                    tester.test_assign("-0.15", "-0.2");
                    tester.test_assign("-0.16", "-0.2");

                    tester.test_div("3.20", 20, "0.2");
                    tester.test_div("3.00", 20, "0.2");
                    tester.test_div("2.80", 20, "0.1");

                    tester.test_div("1.20", 20, "0.1");
                    tester.test_div("1.00", 20, "0.0");
                    tester.test_div("0.80", 20, "0.0");

                    tester.test_div("-0.80", 20, "0.0");
                    tester.test_div("-1.00", 20, "0.0");
                    tester.test_div("-1.20", 20, "-0.1");

                    tester.test_div("-2.80", 20, "-0.1");
                    tester.test_div("-3.00", 20, "-0.2");
                    tester.test_div("-3.20", 20, "-0.2");
                }
        }

BOOST_AUTO_TEST_CASE(decimalUnpack)
        {
                using namespace dec;

                decimal<4> d(-0.5);
                int64 before, after;
                d.unpack(before, after);
                BOOST_CHECK(before == 0);
                BOOST_CHECK(after == -5000);

                decimal<4> d1(-1.5);
                d1.unpack(before, after);
                BOOST_CHECK(before == -1);
                BOOST_CHECK(after == -5000);

                decimal<4> d2(1.5121);
                d2.unpack(before, after);
                BOOST_CHECK(before == 1);
                BOOST_CHECK(after == 5121);

                decimal<4> d3(0.5121);
                d3.unpack(before, after);
                BOOST_CHECK(before == 0);
                BOOST_CHECK(after == 5121);
        }

BOOST_AUTO_TEST_CASE(decimalPack)
        {
                using namespace dec;
                decimal<4> d;
                BOOST_CHECK(decimal<4>().pack(2,5121) == decimal<4>(2.5121));
                BOOST_CHECK(d.pack(1,1) == decimal<4>("1.0001"));
                BOOST_CHECK(d.pack(1,1000) == decimal<4>("1.1"));
                BOOST_CHECK(d.pack(0,5121) == decimal<4>(0.5121));
                BOOST_CHECK(d.pack(0,-5121) == decimal<4>(-0.5121));
                BOOST_CHECK(d.pack(1,5121) == decimal<4>(1.5121));
                BOOST_CHECK(d.pack(-1,0) == decimal<4>(-1.0));
                BOOST_CHECK(d.pack(-1,-5121) == decimal<4>(-1.5121));
                BOOST_CHECK(d.pack(-1,5121) != decimal<4>(-1.5121));
                BOOST_CHECK(d.pack(1,-5121) != decimal<4>(-1.5121));
                // trimming (no rounding) for default pack
                BOOST_CHECK(d.pack(1,51210) != decimal<4>("1.5121"));
                BOOST_CHECK(d.pack(1,51215) != decimal<4>("1.5121"));
                // pack with rounding
                BOOST_CHECK(decimal_cast<4>(decimal<5>().pack(1,51215)) == decimal<4>("1.5122"));
                BOOST_CHECK(d.pack_rounded<5>(1,51216) == decimal<4>("1.5122"));
                BOOST_CHECK(d.pack_rounded<5>(1,51215) == decimal<4>("1.5122"));
                BOOST_CHECK(d.pack_rounded<5>(1,51214) == decimal<4>("1.5121"));
                BOOST_CHECK(d.pack_rounded<5>(1,51211) == decimal<4>("1.5121"));
        }

BOOST_AUTO_TEST_CASE(decimalZeroPrec)
        {
                using namespace dec;
                decimal<0> d;

                // integer values
                BOOST_CHECK(decimal<0>().pack(25121,0) == decimal<0>(25121));
                BOOST_CHECK(d.pack(5121,0) == decimal<0>(5121));

                // trimming (no rounding in pack)
                BOOST_CHECK(decimal<0>().pack(2,5121) == decimal<0>(2));
                BOOST_CHECK(d.pack(0,5121) == decimal<0>(0));
                BOOST_CHECK(d.pack(0,-5121) == decimal<0>(0));
                BOOST_CHECK(d.pack(1,5121) == decimal<0>(1));
                BOOST_CHECK(d.pack(-1,-5121) == decimal<0>(-1));
                BOOST_CHECK(d.pack(-1,5121) == decimal<0>(-1));

                // for rounding use decimal_cast from source prec
                d = decimal_cast<0>(decimal<4>().pack(-1,-5121));
                BOOST_CHECK(d == decimal<0>(-2));
        }

BOOST_AUTO_TEST_CASE(decimalString)
        {
                using namespace dec;
                decimal<4> d(4.1234);
                std::string dTxt = toString(d);
                BOOST_CHECK(d == fromString<decimal<4> >(dTxt));

                // fromString converts with rounding
                BOOST_CHECK(d == decimal<4>().pack(4,1234));
                BOOST_CHECK(d == fromString<decimal<4> >("4.1234"));
                BOOST_CHECK(d == fromString<decimal<4> >("4.12341"));
                BOOST_CHECK(d != fromString<decimal<4> >("4.12345"));
                BOOST_CHECK(decimal<5>().pack(4,12341) == fromString<decimal<5> >("4.12341"));

                // test negative values
                d = d * decimal_cast<4>(-1);
                BOOST_CHECK(d < decimal_cast<4>(0));

                std::string sneg = toString(d);
                BOOST_TEST_MESSAGE("sneg: " << sneg);
                BOOST_TEST_MESSAGE("d: " << d);
                BOOST_TEST_MESSAGE("sneg-to-decimal: " << (fromString<decimal<4> >(sneg)));
                BOOST_CHECK(d == fromString<decimal<4> >(sneg));
        }

BOOST_AUTO_TEST_CASE(decimalComparison)
        {
                const double pi = 3.1415926;
                BOOST_CHECK_EQUAL(dec::decimal<8>(0.), 0);
                BOOST_CHECK_EQUAL(dec::decimal<8>(0.), 0u);
                BOOST_CHECK_EQUAL(dec::decimal<8>(0.), 0L);
                BOOST_CHECK_EQUAL(dec::decimal<8>(0.), 0.);
                BOOST_CHECK_EQUAL(dec::decimal<8>(pi), pi);
                BOOST_CHECK_EQUAL(dec::decimal<8>(pi), dec::decimal<8>(pi));
                BOOST_CHECK(dec::decimal<8>(pi) < 5);
                BOOST_CHECK(dec::decimal<8>(pi) <= 5);
                BOOST_CHECK(dec::decimal<8>(pi) <= pi);
                BOOST_CHECK(dec::decimal<8>(pi) > 3);
                BOOST_CHECK(dec::decimal<8>(pi) >= 3);
                BOOST_CHECK(dec::decimal<8>(pi) >= pi);
                BOOST_CHECK(dec::decimal<8>(pi) != 3);
                BOOST_CHECK(dec::decimal<8>(pi) != 3.0);
        }

BOOST_AUTO_TEST_CASE(decimalSign)
        {
                dec::decimal<4> d(-4.1234);
                BOOST_CHECK(d.sign() < 0);

                d *= -1;
                BOOST_CHECK(d.sign() > 0);

                d = -d;
                BOOST_CHECK(d.sign() < 0);

                d = d.abs();
                BOOST_CHECK(d.sign() > 0);

                d -= d;
                BOOST_CHECK(d.sign() == 0);
        }

class mult_div_tester {
public:
    void test_md(dec::int64 a, dec::int64 b, dec::int64 divisor, dec::int64 expected) {
        using namespace dec;
        int64 res = dec_utils<def_round_policy>::multDiv(a, b, divisor);

        BOOST_CHECK_MESSAGE( res == expected, "multDiv(" << a << ", " << b << ", " << divisor << ") is " << res << ", expecting: " << expected );
    }
};

BOOST_AUTO_TEST_CASE(multDiv)
        {

                {

                        //test condition:
                        //   if ((value1 % divisor) == 0 || (value2 % divisor == 0)) {
                        //       return value1 * (value2 / divisor) + (value1 / divisor) * (value2 % divisor);
                        //   }

                        mult_div_tester tester;

                        tester.test_md(333, 425, 3, 47175);
                        tester.test_md(425, 333, 3, 47175);

                        tester.test_md(-333, 425, 3, -47175);
                        tester.test_md(-425, 333, 3, -47175);

                        tester.test_md(333, -425, 3, -47175);
                        tester.test_md(425, -333, 3, -47175);

                        tester.test_md(-333, -425, 3, 47175);
                        tester.test_md(-425, -333, 3, 47175);

                        //test condition:
                        // if both modulo != 0 and no overflow in x = (value1 % divisor) * (value2 % divisor) then
                        //    add div_policy.div_rounded(x, divisor) to result and return

                        tester.test_md(333, 424, 5, 28238); // round down
                        tester.test_md(333, 593, 7, 28210); // round up

                        tester.test_md(-333, 424, 5, -28238);
                        tester.test_md(-333, 593, 7, -28210);

                        tester.test_md(333, -424, 5, -28238);
                        tester.test_md(333, -593, 7, -28210);

                        tester.test_md(-333, -424, 5, 28238);
                        tester.test_md(-333, -593, 7, 28210);

                        // test step 3/4 - with overflow on x on decimal part but one of decimal parts has gcd with divisor > 1
                        tester.test_md(438241312999, 3681227029158, 876482625990, 1840613514596);

                        // test step 5 - overflow after gcd
                        tester.test_md(438241312999, 3681227029158+222121, 876482625990, 1840613625656);
                }
        }

BOOST_AUTO_TEST_CASE(decimalTypeLevel)
{
    using namespace dec;
    dec::decimal<4> d4("-4.1234");
    dec::decimal<8> d8("3.91726271");

    // --- assignment

#if DEC_TYPE_LEVEL >= 2
    d4 = d8; // will not compile on levels < 2
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d4; // will not compile on level 0
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a + b

#if DEC_TYPE_LEVEL >= 2
    d4 = d4 + d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d8 + d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a += b

#if DEC_TYPE_LEVEL >= 2
    d4 += d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 += d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a - b

#if DEC_TYPE_LEVEL >= 2
    d4 = d4 - d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d8 - d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a -= b

#if DEC_TYPE_LEVEL >= 2
    d4 -= d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 -= d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a * b

#if DEC_TYPE_LEVEL >= 2
    d4 = d4 * d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d8 * d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a *= b

#if DEC_TYPE_LEVEL >= 2
    d4 *= d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 *= d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a / b

#if DEC_TYPE_LEVEL >= 2
    d4 = d4 / d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d8 / d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a /= b

    d4 = decimal_cast<4>(1234);
    d8 = decimal_cast<8>(3);

#if DEC_TYPE_LEVEL >= 2
    d4 /= d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 /= d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif
}

BOOST_AUTO_TEST_CASE(decimalFloatConstructorHighPrec) {
    dec::decimal<2> d1(3.1549999999999998);
    dec::decimal<2> d2("3.1549999999999998");
    BOOST_CHECK_EQUAL(d1, d2);

    dec::decimal<2> d3(-3.1549999999999998);
    dec::decimal<2> d4("-3.1549999999999998");
    BOOST_CHECK_EQUAL(d3, d4);

    dec::decimal<2> d5;
    d5.setAsDouble(3.1549999999999998);
    BOOST_CHECK_EQUAL(d5, d2);

    dec::decimal<2> d6;
    d6 = 3.1549999999999998;
    BOOST_CHECK_EQUAL(d6, d2);
}

BOOST_AUTO_TEST_CASE(decimalToString) {

    std::string expected1 = "3.1549999999999998";
    dec::decimal<16> d1(expected1);

    BOOST_CHECK_EQUAL(dec::toString(d1), expected1);

    std::string expected2 = "315499999999999.98";
    dec::decimal<2> d2(expected2);

    BOOST_CHECK_EQUAL(dec::toString(d2), expected2);
}

BOOST_AUTO_TEST_CASE(decimalFromString) {

    std::string value1 = "3.1549999999999998";
    dec::decimal<16> d1a (3.1549999999999998);
    dec::decimal<16> d1b(value1);

    BOOST_CHECK_EQUAL(d1a, d1b);

    std::string value2 = "31549999999999.98";
    dec::decimal<2> d2a  (31549999999999.98);
    dec::decimal<2> d2b(value2);

    BOOST_CHECK_EQUAL(d2a, d2b);
}

BOOST_AUTO_TEST_CASE(decimalToStringWithFormat) {

    dec::decimal_format format(',', '.');

    std::string value1    = "3.1549999999999998";
    std::string expected1 = "3,1549999999999998";
    dec::decimal<16> d1(value1);

    BOOST_CHECK_EQUAL(dec::toString(d1, format), expected1);

    std::string value2    = "315499999999999.98";
    std::string expected2 = "315.499.999.999.999,98";
    dec::decimal<2> d2(value2);

    BOOST_CHECK_EQUAL(dec::toString(d2, format), expected2);
}

BOOST_AUTO_TEST_CASE(decimalFromStringWithFormat) {

    dec::decimal_format format(',', '.');

    std::string value1 = "3,1549999999999998";
    dec::decimal<16> d1a (3.1549999999999998);
    dec::decimal<16> d1b(dec::fromString<dec::decimal<16>>(value1, format));

    BOOST_CHECK_EQUAL(d1a, d1b);

    std::string value2 = "31.549.999.999.999,98";
    dec::decimal<2> d2a  (31549999999999.98);
    dec::decimal<2> d2b(dec::fromString<dec::decimal<2>>(value2, format));

    BOOST_CHECK_EQUAL(d2a, d2b);
}

BOOST_AUTO_TEST_CASE(trivialAndNoThrowConstructor) {
    BOOST_CHECK_EQUAL(std::is_trivial<dec::decimal<6>>::value, false);

    BOOST_CHECK_EQUAL(std::is_trivially_constructible<dec::decimal<6>>::value, false);
    BOOST_CHECK_EQUAL(std::is_nothrow_constructible<dec::decimal<6>>::value, true);

    BOOST_CHECK_EQUAL(std::is_trivially_default_constructible<dec::decimal<6>>::value, false);
    BOOST_CHECK_EQUAL(std::is_nothrow_default_constructible<dec::decimal<6>>::value, true);

    BOOST_CHECK_EQUAL(std::is_trivially_copy_constructible<dec::decimal<6>>::value, true);
    BOOST_CHECK_EQUAL(std::is_nothrow_copy_constructible<dec::decimal<6>>::value, true);

    BOOST_CHECK_EQUAL(std::is_trivially_copy_assignable<dec::decimal<6>>::value, true);
    BOOST_CHECK_EQUAL(std::is_nothrow_copy_assignable<dec::decimal<6>>::value, true);

    BOOST_CHECK_EQUAL(std::is_trivially_destructible<dec::decimal<6>>::value, true);
    BOOST_CHECK_EQUAL(std::is_nothrow_destructible<dec::decimal<6>>::value, true);
}
