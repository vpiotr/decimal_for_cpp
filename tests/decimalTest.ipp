/////////////////////////////////////////////////////////////////////////////
// Name:        decimalTest.ipp
// Purpose:     Test decimal type.
// Author:      Piotr Likus
// Modified by:
// Created:     31/10/2010
// Licence:     BSD
/////////////////////////////////////////////////////////////////////////////

#include "decimal.h"
#include <boost/integer_traits.hpp>

template <typename T>
std::string toString(const T &arg) {
    std::ostringstream	out;
    out << arg;
    return(out.str());
}

/*
template <typename T>
T fromString (const std::string &str) {
    std::istringstream is(str);
    T t;
    is >> t;
    return t;
}
*/

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

   balance = +balance;
   BOOST_CHECK(balance == dec::decimal2(-4.45));

   balance = -balance;
   BOOST_CHECK(balance == dec::decimal2(4.45));

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
   dec::decimal<12> longDec1("6321311.121761616789");
   dec::decimal<12> longDec2;

   std::string s = toString(longDec1);
   longDec2 = dec::fromString<dec::decimal<12> >(s);

   BOOST_TEST_MESSAGE("longDec1: " << longDec1);
   BOOST_TEST_MESSAGE("longDec1 as string: " << s);
   BOOST_TEST_MESSAGE("longDec2: " << longDec2);
   BOOST_REQUIRE_EQUAL(longDec1, longDec2);
}

BOOST_AUTO_TEST_CASE(decimalHighEndVals)
{
   // with decimal<16> we need to provide constants with strings not floating points.
   dec::decimal<16> a;
   dec::decimal<16> b;
   dec::decimal<16> c;

   a = dec::decimal<16>("1.1111111111111111");
   b = dec::decimal<16>(2.0);

   c = a + b;
   BOOST_CHECK(c == dec::decimal<16>("3.1111111111111111"));

   c = a - b;
   BOOST_CHECK(c == dec::decimal<16>("-0.8888888888888889"));

   c = a * b;
   BOOST_CHECK(c == dec::decimal<16>("2.2222222222222222"));

   c = a / b;
   BOOST_CHECK(c == dec::decimal<16>("0.5555555555555556"));
}

BOOST_AUTO_TEST_CASE(decimalUHighEndVals)
{
   dec::decimal<17> a;
   dec::decimal<17> b;
   dec::decimal<17> c;

   a = dec::decimal<17>("1.1");
   b = dec::decimal<17>("2.0");

   c = a + b;
   BOOST_CHECK(c == dec::decimal<17>("3.1"));

   c = a - b;
   BOOST_CHECK(c ==dec::decimal<17>("-0.9"));

   c = a * b;
   BOOST_CHECK(c == dec::decimal<17>("2.2"));

   c = a / b;
   BOOST_CHECK(c == dec::decimal<17>("0.55"));
}

BOOST_AUTO_TEST_CASE(decimalMaxUIntCtor)
{
  unsigned int uint_max = boost::integer_traits<unsigned int>::const_max;
  std::string uint_max_txt = toString(uint_max);
  dec::decimal<2> a(uint_max);
  BOOST_REQUIRE_EQUAL( a, dec::decimal<2>(uint_max_txt) );
  BOOST_REQUIRE_EQUAL( a.getAsInteger(), uint_max);
}

BOOST_AUTO_TEST_CASE(decimalAsInteger)
{
   // rounded value
   dec::decimal<6> a;
   a = dec::decimal<6>("2305843009213.693952");
   dec::DEC_INT64 expectedValue = 2305843009214;
   BOOST_REQUIRE_EQUAL( a.getAsInteger(), expectedValue);

   // int64 value (>0)
   expectedValue = 23058430092136939;
   dec::decimal<1> b = dec::decimal<1>(toString(expectedValue));
   BOOST_REQUIRE_EQUAL( b.getAsInteger(), expectedValue);

   // int64 value (<0)
   expectedValue = -23058430092136939;
   dec::decimal<1> c = dec::decimal<1>(toString(expectedValue));
   BOOST_REQUIRE_EQUAL( c.getAsInteger(), expectedValue);
}

// test with values internally > 2^32
BOOST_AUTO_TEST_CASE(decimalMidOverflow)
{
   dec::decimal<6> a;
   dec::decimal<6> b;
   dec::decimal<6> c;
   dec::decimal<6> expected;

   a = dec::decimal<6>("2305843009213.693952"); // 2^61
   b = dec::decimal<6>("2.000001"); // value < 2^32

   c = a * b;
   expected = dec::decimal<6>("4611688324270.397118");

   // overflow, so calculation via floating point, error will be <> 0, but should be small
   BOOST_CHECK((c - expected).abs() < dec::decimal<6>("1.0"));
}

BOOST_AUTO_TEST_CASE(decimalDefRoundPolicy)
{
  using namespace dec;
  decimal<1, def_round_policy> a("0.5");
  decimal<1, def_round_policy> b("2");
  decimal<1, def_round_policy> c;

  c = a / b;
  dec::decimal<1, def_round_policy> expected("0.3");
  BOOST_CHECK(c == expected);
}

BOOST_AUTO_TEST_CASE(decimalNoRoundPolicy)
{
  using namespace dec;
  decimal<1, null_round_policy> a("0.5");
  decimal<1, null_round_policy> b("2");
  decimal<1, null_round_policy> c;

  c = a / b;
  dec::decimal<1, null_round_policy> expected("0.2");
  BOOST_CHECK(c == expected);
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
  BOOST_CHECK(d.pack(1,1) == decimal<4>("1.0001"));
  BOOST_CHECK(d.pack(1,1000) == decimal<4>("1.1"));
  BOOST_CHECK(d.pack(0,5121) == decimal<4>(0.5121));
  BOOST_CHECK(d.pack(0,-5121) == decimal<4>(-0.5121));
  BOOST_CHECK(d.pack(1,5121) == decimal<4>(1.5121));
  BOOST_CHECK(d.pack(-1,0) == decimal<4>(-1.0));
  BOOST_CHECK(d.pack(-1,-5121) == decimal<4>(-1.5121));
  BOOST_CHECK(d.pack(-1,5121) != decimal<4>(-1.5121));
  BOOST_CHECK(d.pack(1,-5121) != decimal<4>(-1.5121));
  // trimming (no rounding) for default pack
  BOOST_CHECK(d.pack(1,51210) != decimal<4>("1.5121"));
  BOOST_CHECK(d.pack(1,51215) != decimal<4>("1.5121"));
  // pack with rounding
  BOOST_CHECK(decimal_cast<4>(decimal<5>().pack(1,51215)) == decimal<4>("1.5122"));
  BOOST_CHECK(d.pack_rounded<5>(1,51216) == decimal<4>("1.5122"));
  BOOST_CHECK(d.pack_rounded<5>(1,51215) == decimal<4>("1.5122"));
  BOOST_CHECK(d.pack_rounded<5>(1,51214) == decimal<4>("1.5121"));
  BOOST_CHECK(d.pack_rounded<5>(1,51211) == decimal<4>("1.5121"));
}

BOOST_AUTO_TEST_CASE(decimalZeroPrec)
{
  using namespace dec;
  decimal<0> d;

  // integer values
  BOOST_CHECK(decimal<0>().pack(25121,0) == decimal<0>(25121));
  BOOST_CHECK(d.pack(5121,0) == decimal<0>(5121));

  // trimming (no rounding in pack)
  BOOST_CHECK(decimal<0>().pack(2,5121) == decimal<0>(2));
  BOOST_CHECK(d.pack(0,5121) == decimal<0>(0));
  BOOST_CHECK(d.pack(0,-5121) == decimal<0>(0));
  BOOST_CHECK(d.pack(1,5121) == decimal<0>(1));
  BOOST_CHECK(d.pack(-1,-5121) == decimal<0>(-1));
  BOOST_CHECK(d.pack(-1,5121) == decimal<0>(-1));

  // for rounding use decimal_cast from source prec
  d = decimal_cast<0>(decimal<4>().pack(-1,-5121));
  BOOST_CHECK(d == decimal<0>(-2));
}

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
