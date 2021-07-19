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

#ifndef DEC_NO_CPP11
#include <type_traits>
#endif

std::string uint_to_string(unsigned int arg) {
    std::ostringstream out;
    out << arg;
    return (out.str());
}

    BOOST_AUTO_TEST_CASE(decimalAsInteger)
{
    // rounded value
    dec::decimal<6> a;
    a = dec::decimal<6>("2305843009213.693952");
    dec::DEC_INT64 expectedValue = 2305843009214;
    BOOST_CHECK_EQUAL(a.getAsInteger(), expectedValue);

    // int64 value (>0)
    expectedValue = 23058430092136939;
    dec::decimal<1> b = dec::decimal<1>(expectedValue);
    BOOST_CHECK_EQUAL(b.getAsInteger(), expectedValue);

    // int64 value (<0)
    expectedValue = -23058430092136939;
    dec::decimal<1> c = dec::decimal<1>(expectedValue);
    BOOST_CHECK_EQUAL(c.getAsInteger(), expectedValue);
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
    BOOST_CHECK(decimal<4>().pack(2, 5121) == decimal<4>(2.5121));
    BOOST_CHECK(d.pack(1, 1) == decimal<4>("1.0001"));
    BOOST_CHECK(d.pack(1, 1000) == decimal<4>("1.1"));
    BOOST_CHECK(d.pack(0, 5121) == decimal<4>(0.5121));
    BOOST_CHECK(d.pack(0, -5121) == decimal<4>(-0.5121));
    BOOST_CHECK(d.pack(1, 5121) == decimal<4>(1.5121));
    BOOST_CHECK(d.pack(-1, 0) == decimal<4>(-1.0));
    BOOST_CHECK(d.pack(-1, -5121) == decimal<4>(-1.5121));
    BOOST_CHECK(d.pack(-1, 5121) != decimal<4>(-1.5121));
    BOOST_CHECK(d.pack(1, -5121) != decimal<4>(-1.5121));
    // trimming (no rounding) for default pack
    BOOST_CHECK(d.pack(1, 51210) != decimal<4>("1.5121"));
    BOOST_CHECK(d.pack(1, 51215) != decimal<4>("1.5121"));
    // pack with rounding
    BOOST_CHECK(decimal_cast<4>(decimal<5>().pack(1, 51215)) == decimal<4>("1.5122"));
    BOOST_CHECK(d.pack_rounded<5>(1, 51216) == decimal<4>("1.5122"));
    BOOST_CHECK(d.pack_rounded<5>(1, 51215) == decimal<4>("1.5122"));
    BOOST_CHECK(d.pack_rounded<5>(1, 51214) == decimal<4>("1.5121"));
    BOOST_CHECK(d.pack_rounded<5>(1, 51211) == decimal<4>("1.5121"));
}

    BOOST_AUTO_TEST_CASE(decimalZeroPrec)
{
    using namespace dec;
    decimal<0> d;

    // integer values
    BOOST_CHECK(decimal<0>().pack(25121, 0) == decimal<0>(25121));
    BOOST_CHECK(d.pack(5121, 0) == decimal<0>(5121));

    // trimming (no rounding in pack)
    BOOST_CHECK(decimal<0>().pack(2, 5121) == decimal<0>(2));
    BOOST_CHECK(d.pack(0, 5121) == decimal<0>(0));
    BOOST_CHECK(d.pack(0, -5121) == decimal<0>(0));
    BOOST_CHECK(d.pack(1, 5121) == decimal<0>(1));
    BOOST_CHECK(d.pack(-1, -5121) == decimal<0>(-1));
    BOOST_CHECK(d.pack(-1, 5121) == decimal<0>(-1));

    // for rounding use decimal_cast from source prec
    d = decimal_cast<0>(decimal<4>().pack(-1, -5121));
    BOOST_CHECK(d == decimal<0>(-2));
}

    BOOST_AUTO_TEST_CASE(decimalComparison)
{
    const double pi = 3.1415926;
    BOOST_CHECK_EQUAL(dec::decimal<8>(0.), 0);
    BOOST_CHECK_EQUAL(dec::decimal<8>(0.), 0u);
#ifdef DEC_HANDLE_LONG
    BOOST_CHECK_EQUAL(dec::decimal<8>(0.), 0L);
#endif
    BOOST_CHECK_EQUAL(dec::decimal<8>(0.), 0.);
    BOOST_CHECK_EQUAL(dec::decimal<8>(pi), pi);
    BOOST_CHECK_EQUAL(dec::decimal<8>(pi), dec::decimal<8>(pi));
    BOOST_CHECK(dec::decimal<8>(pi) < 5);
    BOOST_CHECK(dec::decimal<8>(pi) <= 5);
    BOOST_CHECK(dec::decimal<8>(pi) <= pi);
    BOOST_CHECK(dec::decimal<8>(pi) > 3);
    BOOST_CHECK(dec::decimal<8>(pi) >= 3);
    BOOST_CHECK(dec::decimal<8>(pi) >= pi);
    BOOST_CHECK(dec::decimal<8>(pi) != 3);
    BOOST_CHECK(dec::decimal<8>(pi) != 3.0);
}

    BOOST_AUTO_TEST_CASE(decimalSign)
{
    dec::decimal<4> d(-4.1234);
    BOOST_CHECK(d.sign() < 0);

    d *= -1;
    BOOST_CHECK(d.sign() > 0);

    d = -d;
    BOOST_CHECK(d.sign() < 0);

    d = d.abs();
    BOOST_CHECK(d.sign() > 0);

    d -= d;
    BOOST_CHECK(d.sign() == 0);
}

    BOOST_AUTO_TEST_CASE(decimalFloatConstructorHighPrec) {
    dec::decimal<2> d1(3.1549999999999998);
    dec::decimal<2> d2("3.1549999999999998");
    BOOST_CHECK_EQUAL(d1, d2);

    dec::decimal<2> d3(-3.1549999999999998);
    dec::decimal<2> d4("-3.1549999999999998");
    BOOST_CHECK_EQUAL(d3, d4);

    dec::decimal<2> d5;
    d5.setAsDouble(3.1549999999999998);
    BOOST_CHECK_EQUAL(d5, d2);

    dec::decimal<2> d6;
    d6 = 3.1549999999999998;
    BOOST_CHECK_EQUAL(d6, d2);
}

#ifndef DEC_NO_CPP11
    BOOST_AUTO_TEST_CASE(trivialAndNoThrowConstructor) {
    BOOST_CHECK_EQUAL(std::is_trivial<dec::decimal<6>>::value, false);

    BOOST_CHECK_EQUAL(std::is_trivially_constructible<dec::decimal<6>>::value, false);
    BOOST_CHECK_EQUAL(std::is_nothrow_constructible<dec::decimal<6>>::value, true);

    BOOST_CHECK_EQUAL(std::is_trivially_default_constructible<dec::decimal<6>>::value, false);
    BOOST_CHECK_EQUAL(std::is_nothrow_default_constructible<dec::decimal<6>>::value, true);

    BOOST_CHECK_EQUAL(std::is_trivially_copy_constructible<dec::decimal<6>>::value, true);
    BOOST_CHECK_EQUAL(std::is_nothrow_copy_constructible<dec::decimal<6>>::value, true);

    BOOST_CHECK_EQUAL(std::is_trivially_copy_assignable<dec::decimal<6>>::value, true);
    BOOST_CHECK_EQUAL(std::is_nothrow_copy_assignable<dec::decimal<6>>::value, true);

    BOOST_CHECK_EQUAL(std::is_trivially_destructible<dec::decimal<6>>::value, true);
    BOOST_CHECK_EQUAL(std::is_nothrow_destructible<dec::decimal<6>>::value, true);
}
#endif

