//
// Created by piotr on 7/19/21.
//

#include "decimal.h"

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

        const std::string value1 = "3,1549999999999998";
        dec::decimal<16> d1a (3.1549999999999998);
        dec::decimal<16> d1b(dec::fromString<dec::decimal<16> >(value1, format));

        BOOST_CHECK_EQUAL(d1a, d1b);

        const std::string value2 = "31.549.999.999.999,98";
        dec::decimal<2> d2a  (31549999999999.98);
        dec::decimal<2> d2b(dec::fromString<dec::decimal<2> >(value2, format));

        BOOST_CHECK_EQUAL(d2a, d2b);
}

