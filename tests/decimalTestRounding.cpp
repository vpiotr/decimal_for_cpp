//
// Created by piotr on 7/19/21.
//

#include "decimal.h"

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
    round_tester_t(const std::string &name) : m_name(name) {}

    void test_assign(const std::string &src_value, const std::string &expected_value) {
        dec::decimal<Prec, RoundPolicy> a;
        dec::decimal<Prec, RoundPolicy> expected;

        a = dec::decimal<Prec, RoundPolicy>(src_value);
        expected = dec::decimal<Prec, RoundPolicy>(expected_value);

        BOOST_CHECK_MESSAGE(a == expected,
                            "assign decimal<" << Prec << ", " << m_name << ">(" << src_value << ") is " << a
                                              << ", expecting: " << expected_value);
    }

    void test_div2(const std::string &src_value, const std::string &expected_value) {
        dec::decimal<Prec, RoundPolicy> a;
        dec::decimal<Prec, RoundPolicy> expected;

        a = dec::decimal<Prec, RoundPolicy>(src_value) / dec::decimal<Prec, RoundPolicy>("2.0");
        expected = dec::decimal<Prec, RoundPolicy>(expected_value);

        BOOST_CHECK_MESSAGE(a == expected,
                            "div2 decimal<" << Prec << ", " << m_name << ">(" << src_value << ") is " << a
                                            << ", expecting: " << expected_value);
    }

    void test_div(const std::string &a_value, int b_value, const std::string &expected_value) {
        dec::decimal<Prec, RoundPolicy> a;
        dec::decimal<Prec, RoundPolicy> expected;

        a = dec::decimal<Prec, RoundPolicy>(a_value) / dec::decimal_cast<Prec, RoundPolicy>(b_value);
        expected = dec::decimal<Prec, RoundPolicy>(expected_value);

        BOOST_CHECK_MESSAGE(a == expected,
                            "divn decimal<" << Prec << ", " << m_name << ">(" << a_value << ", " << b_value << ") is "
                                            << a << ", expecting: " << expected_value);
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

