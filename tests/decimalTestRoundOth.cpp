//
// Created by piotr on 7/19/21.
//

#include "decimal.h"

BOOST_AUTO_TEST_CASE(testFloor) {
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2).floor(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2.4).floor(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2.9).floor(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2.7).floor(), -3);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2).floor(), -2);
}

BOOST_AUTO_TEST_CASE(testCeil) {
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2).ceil(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2.4).ceil(), 3);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2.9).ceil(), 3);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2.7).ceil(), -2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2).ceil(), -2);
}

BOOST_AUTO_TEST_CASE(testRound) {
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2).round(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2.4).round(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2.9).round(), 3);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2.7).round(), -3);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2.4).round(), -2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2).round(), -2);
}

BOOST_AUTO_TEST_CASE(testTrunc) {
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2).trunc(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2.4).trunc(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(2.9).trunc(), 2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2.7).trunc(), -2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2.4).trunc(), -2);
    BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-2).trunc(), -2);
}

