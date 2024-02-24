//
// Created by piotr on 7/19/21.
//

#include "decimal.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(decimalTypeLevel)
{
    using namespace dec;
    dec::decimal<4> d4("-4.1234");
    dec::decimal<8> d8("3.91726271");

    // --- assignment

#if DEC_TYPE_LEVEL >= 2
    d4 = d8; // will not compile on levels < 2
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d4; // will not compile on level 0
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a + b

#if DEC_TYPE_LEVEL >= 2
    d4 = d4 + d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d8 + d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a += b

#if DEC_TYPE_LEVEL >= 2
    d4 += d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 += d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a - b

#if DEC_TYPE_LEVEL >= 2
    d4 = d4 - d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d8 - d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a -= b

#if DEC_TYPE_LEVEL >= 2
    d4 -= d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 -= d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a * b

#if DEC_TYPE_LEVEL >= 2
    d4 = d4 * d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d8 * d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a *= b

#if DEC_TYPE_LEVEL >= 2
    d4 *= d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 *= d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a / b

#if DEC_TYPE_LEVEL >= 2
    d4 = d4 / d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 = d8 / d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif

    // --- a /= b

    d4 = decimal_cast<4>(1234);
    d8 = decimal_cast<8>(3);

#if DEC_TYPE_LEVEL >= 2
    d4 /= d8;
    BOOST_CHECK(d4 != decimal_cast<4>(0));
#endif
#if DEC_TYPE_LEVEL >= 1
    d8 /= d4;
    BOOST_CHECK(d8 != decimal_cast<8>(0));
#endif
}

