/////////////////////////////////////////////////////////////////////////////
// Name:        decimalTest.cpp
// Purpose:     Test decimal type.
// Author:      Piotr Likus
// Modified by:
// Created:     31/10/2010
// Licence:     BSD
/////////////////////////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>
#include "decimal.h"

template <typename T> 
std::string toString(const T &arg) {
    std::ostringstream	out;
    out << arg.getAsDouble();
    return(out.str());
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

   balance = balance / dec::decimal2(10.0);
   BOOST_TEST_MESSAGE("balance-9: " << toString(balance));
   BOOST_CHECK(balance == dec::decimal2(-4.5));

   balance *= dec::decimal2(3.0);
   BOOST_CHECK(balance == dec::decimal2(-13.5));

   BOOST_CHECK(sizeof(dec::decimal2::raw_data_t) > 0);

   balance /= dec::decimal2(10.0);
   BOOST_CHECK(balance == dec::decimal2(-1.35));

   balance -= dec::decimal2(0.1);
   BOOST_CHECK(balance == dec::decimal2(-1.45));

   balance = balance - dec::decimal2(3);
   BOOST_CHECK(balance == dec::decimal2(-4.45));

   BOOST_CHECK(balance.abs() == dec::decimal2(4.45));

   BOOST_CHECK(balance.getAsInteger() == -4);

   BOOST_TEST_MESSAGE("balance-end: " << toString(balance));
}

BOOST_AUTO_TEST_CASE(decimalRounding)
{
   dec::decimal<2> balance;

   balance = dec::decimal2(3.156);
   BOOST_CHECK(balance == dec::decimal2(3.16));

   balance = dec::decimal2(3.155);
   BOOST_CHECK(balance == dec::decimal2(3.16));

   balance = dec::decimal2(3.154);
   BOOST_CHECK(balance == dec::decimal2(3.15));

   balance = dec::decimal2(-3.156);
   BOOST_CHECK(balance == dec::decimal2(-3.16));

   balance = dec::decimal2(-3.155);
   BOOST_CHECK(balance == dec::decimal2(-3.16));

   balance = dec::decimal2(-3.154);
   BOOST_CHECK(balance == dec::decimal2(-3.15));
}

