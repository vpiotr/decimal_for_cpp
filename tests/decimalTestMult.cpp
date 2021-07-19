//
// Created by piotr on 7/19/21.
//

#include "decimal.h"

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
    BOOST_CHECK_EQUAL(dec::decimal<9>("1.80000001") * dec::decimal<9>("2"), dec::decimal<9>("3.600000020"));
    BOOST_CHECK_EQUAL(dec::decimal<9>("1.80000001") * dec::decimal<9>("2"), dec::decimal<9>("3.600000020"));

    // check as dec 6
    BOOST_CHECK_EQUAL(dec::decimal<6>("35000.000005") * dec::decimal<6>("54123.654133"),
                      dec::decimal<6>("1894327894.925618"));
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

