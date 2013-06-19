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

template <typename T>
T fromString (const std::string &str) {
    std::istringstream is(str);
    T t;
    is >> t;
    return t;
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
   dec::decimal<12> longDec1(6321311.121761616789);
   dec::decimal<12> longDec2;

   std::string s = toString(longDec1);
   longDec2 = fromString<dec::decimal<12> >(s);
   BOOST_CHECK(longDec1 == longDec2);
}


BOOST_AUTO_TEST_CASE(decimalUnpack)
{
  using namespace dec;

  decimal<4> d(-0.5);
  int64 before, after;
  d.unpack(before, after);
  BOOST_CHECK(before == 0);
  BOOST_CHECK(after == -5000);

  decimal<4> d1(-1.5);
  d1.unpack(before, after);
  BOOST_CHECK(before == -1);
  BOOST_CHECK(after == -5000);

  decimal<4> d2(1.5121);
  d2.unpack(before, after);
  BOOST_CHECK(before == 1);
  BOOST_CHECK(after == 5121);

  decimal<4> d3(0.5121);
  d3.unpack(before, after);
  BOOST_CHECK(before == 0);
  BOOST_CHECK(after == 5121);
}

BOOST_AUTO_TEST_CASE(decimalPack)
{
  using namespace dec;
  decimal<4> d;
  BOOST_CHECK(decimal<4>().pack(2,5121) == decimal<4>(2.5121));
  BOOST_CHECK(d.pack(0,5121) == decimal<4>(0.5121));
  BOOST_CHECK(d.pack(0,-5121) == decimal<4>(-0.5121));
  BOOST_CHECK(d.pack(1,5121) == decimal<4>(1.5121));
  BOOST_CHECK(d.pack(-1,-5121) == decimal<4>(-1.5121));
}

BOOST_AUTO_TEST_CASE(decimalString)
{
  using namespace dec;
  decimal<4> d(4.1234);
  std::string spos = toString(d);
  BOOST_CHECK(d == fromString<decimal<4> >(spos));

  d = d * decimal_cast<4>(-1);
  BOOST_CHECK(d < decimal_cast<4>(0));

  std::string sneg = toString(d);
  BOOST_CHECK(d == fromString<decimal<4> >(sneg));
}
