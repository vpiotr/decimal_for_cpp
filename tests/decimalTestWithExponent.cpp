//
// Created by piotr on 7/19/21.
//

#include "decimal.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(decimalWithExponent)
{
    // build positive values
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, 0), dec::decimal<4>("11"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, 2), dec::decimal<4>("1100"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, -2), dec::decimal<4>("0.11"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, 1), dec::decimal<4>("110"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(11, -1), dec::decimal<4>("1.1"));

    // build negative values
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, 0), dec::decimal<4>("-11"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, 2), dec::decimal<4>("-1100"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, -2), dec::decimal<4>("-0.11"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, 1), dec::decimal<4>("-110"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(-11, -1), dec::decimal<4>("-1.1"));

    // build zero
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(0, -1), dec::decimal<4>("0"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(0, 2), dec::decimal<4>("0"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(0, 0), dec::decimal<4>("0"));

    // add, get, set
    dec::decimal<4> temp;
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, 111213, -3), dec::decimal<4>("111.213"));
    dec::decimal<4> a;
    a.setWithExponent(30, -2);
    temp += a;
    BOOST_CHECK_EQUAL(temp, dec::decimal<4>("111.5130"));

    dec::DEC_INT64 m;
    int e;
    temp.getWithExponent(m, e);
    BOOST_CHECK_EQUAL(m, 111513);
    BOOST_CHECK_EQUAL(e, -3);

    // rounding
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, 111213, -5), dec::decimal<4>("1.1121"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, 111215, -5), dec::decimal<4>("1.1122"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, -111213, -5), dec::decimal<4>("-1.1121"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(temp, -111215, -5), dec::decimal<4>("-1.1122"));

    // check overflow
    BOOST_CHECK_EQUAL(dec::decimal<0>::buildWithExponent(1, 23), dec::decimal<0>("0"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(1, 23), dec::decimal<4>("0"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(1, 19), dec::decimal<4>("0"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(1, 15), dec::decimal<4>("0"));

    // check underflow
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(1, -19), dec::decimal<4>("0"));
    BOOST_CHECK_EQUAL(dec::decimal<4>::buildWithExponent(1, -15), dec::decimal<4>("0"));
}

