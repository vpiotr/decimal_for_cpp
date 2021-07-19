//
// Created by piotr on 7/19/21.
//

#include "decimal.h"

BOOST_AUTO_TEST_CASE(decimalHighEndVals)
        {
                // with decimal<16> we need to provide constants with strings not floating points.
                dec::decimal < 16 > a;
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
                dec::decimal < 17 > a;
        dec::decimal<17> b;
        dec::decimal<17> c;

        a = dec::decimal<17>("1.1");
        b = dec::decimal<17>("2.0");

        c = a + b;
        BOOST_CHECK(c == dec::decimal<17>("3.1"));

        c = a - b;
        BOOST_CHECK(c == dec::decimal<17>("-0.9"));

        c = a * b;
        BOOST_CHECK(c == dec::decimal<17>("2.2"));

        c = a / b;
        BOOST_CHECK(c == dec::decimal<17>("0.55"));
        }

BOOST_AUTO_TEST_CASE(decimalMaxUIntCtor)
        {
                unsigned int uint_max = boost::integer_traits<unsigned int>::const_max;
        std::string uint_max_txt = uint_to_string(uint_max);
        dec::decimal<2> a(uint_max);
        BOOST_CHECK_EQUAL(a, dec::decimal<2>(uint_max_txt));
        BOOST_CHECK_EQUAL(a.getAsInteger(), uint_max);
        }

BOOST_AUTO_TEST_CASE(decimalMaxInt64Ctor)
        {
                const dec::decimal<7> n(922337203685.4);
                BOOST_CHECK(n.getAsInteger() == 922337203685);
        }

