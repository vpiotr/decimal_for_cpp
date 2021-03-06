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

BOOST_AUTO_TEST_SUITE ( test )
#include "decimalTest.ipp"
#include "decimalTestAbout.cpp"
#include "decimalTestArithmetic.cpp"
#include "decimalTestRounding.cpp"
#include "decimalTestRoundOth.cpp"
#include "decimalTestMultDiv.cpp"
#include "decimalTestMult.cpp"
#include "decimalTestDiv.cpp"
#include "decimalTestString.cpp"
#include "decimalTestTypeLevel.cpp"
#include "decimalTestWithExponent.cpp"
#include "decimalTestModulo.cpp"
//#include "decimalTestIo.ipp"
BOOST_AUTO_TEST_SUITE_END( )
