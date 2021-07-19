//
// Created by piotr on 7/19/21.
//

#include "decimal.h"

    BOOST_AUTO_TEST_CASE(decimalDivInt)
{
    BOOST_CHECK_EQUAL(dec::decimal<4>("1.0001") / 2, dec::decimal<4>("0.5001"));
#ifdef DEC_HANDLE_LONG
    BOOST_CHECK_EQUAL(dec::decimal<4>("2.0010") / 2L, dec::decimal<4>("1.0005"));
#endif
}

