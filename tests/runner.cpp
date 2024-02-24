/////////////////////////////////////////////////////////////////////////////
// Name:        runner.cpp
// Project:     decimal
// Purpose:     Test runner for decimal library
//              Include it as the only .CPP file.
// Author:      Piotr Likus
// Modified by:
// Created:     01/02/2014
// Licence:     BSD
/////////////////////////////////////////////////////////////////////////////

#ifdef __MINGW32__
// Mingw doesn't define putenv() needed by Boost.Test
extern int putenv(char*);
#endif

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "C++ Unit Tests for decimal"
#include <boost/test/included/unit_test.hpp>
using namespace boost::unit_test;

#include "decimal.h"

BOOST_AUTO_TEST_CASE(decimalBasicTest) {
    using namespace dec;
    using namespace std;

    decimal<2> value1(143125);
    decimal<2> value2("143125");

    BOOST_CHECK_EQUAL(value1, value2);
}

