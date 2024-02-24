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

    // the following declares currency variable with 2 decimal points
    // initialized with integer value (can be also floating-point)
    decimal<2> value(143125);

    // displays: Value #1 is: 143125.00
    cout << "Value #1 is: " << value << endl;
}

//BOOST_AUTO_TEST_SUITE ( test )
//#include "decimalTest.cpp"
//#include "decimalTestAbout.cpp"
//#include "decimalTestArithmetic.cpp"
//#include "decimalTestDiv.cpp"
//#include "decimalTestEdgeCases.cpp"
//#include "decimalTestModulo.cpp"
//#include "decimalTestMult.cpp"
//#include "decimalTestMultDiv.cpp"
//#include "decimalTestRounding.cpp"
//#include "decimalTestRoundOth.cpp"
//#include "decimalTestString.cpp"
//#include "decimalTestTypeLevel.cpp"
//#include "decimalTestWithExponent.cpp"
//BOOST_AUTO_TEST_SUITE_END( )
