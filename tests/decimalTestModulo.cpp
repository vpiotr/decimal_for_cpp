//
// Created by piotr on 7/19/21.
//

#include "decimal.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testModOperInt) {
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(15) % 4, 3);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-15) % 4, -3);

        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(11.9) % 4, 3.9);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(12.1) % 4, 0.1);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(12.5) % 4, 0.5);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(12.9) % 4, 0.9);

        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-11.9) % 4, -3.9);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-12.1) % 4, -0.1);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-12.5) % 4, -0.5);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-12.9) % 4, -0.9);
}

BOOST_AUTO_TEST_CASE(testModOperSamePrec) {
        dec::decimal<2> b(2);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(11.6) % b, 1.6);
        dec::decimal<2> c(2.1);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(11.6) % c, 1.1);
        dec::decimal<2> d(1);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(-0.8) % d, -0.8);
}

BOOST_AUTO_TEST_CASE(testModOperLowerPrec) {
        dec::decimal<2> b(2);
        BOOST_CHECK_EQUAL(dec::decimal_cast<3>(11.6) % b, 1.6);
        dec::decimal<2> c(2.1);
        BOOST_CHECK_EQUAL(dec::decimal_cast<3>(11.6) % c, 1.1);
}

BOOST_AUTO_TEST_CASE(testModOperHigherPrec) {
        dec::decimal<3> b(2);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(11.6) % b, 1.6);
        dec::decimal<3> c(2.1);
        BOOST_CHECK_EQUAL(dec::decimal_cast<2>(11.6) % c, 1.1);
}

