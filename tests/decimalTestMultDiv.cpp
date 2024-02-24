//
// Created by piotr on 7/19/21.
//

#include "decimal.h"
#include <boost/test/unit_test.hpp>

class mult_div_tester {
public:
    void test_md(dec::int64 a, dec::int64 b, dec::int64 divisor, dec::int64 expected) {
        using namespace dec;
        int64 res = dec_utils<def_round_policy>::multDiv(a, b, divisor);

        BOOST_CHECK_MESSAGE(res == expected,
                            "multDiv(" << a << ", " << b << ", " << divisor << ") is " << res << ", expecting: "
                                       << expected);
    }
};

BOOST_AUTO_TEST_CASE(multDiv)
{

    {

        //test condition:
        //   if ((value1 % divisor) == 0 || (value2 % divisor == 0)) {
        //       return value1 * (value2 / divisor) + (value1 / divisor) * (value2 % divisor);
        //   }

        mult_div_tester tester;

        tester.test_md(333, 425, 3, 47175);
        tester.test_md(425, 333, 3, 47175);

        tester.test_md(-333, 425, 3, -47175);
        tester.test_md(-425, 333, 3, -47175);

        tester.test_md(333, -425, 3, -47175);
        tester.test_md(425, -333, 3, -47175);

        tester.test_md(-333, -425, 3, 47175);
        tester.test_md(-425, -333, 3, 47175);

        //test condition:
        // if both modulo != 0 and no overflow in x = (value1 % divisor) * (value2 % divisor) then
        //    add div_policy.div_rounded(x, divisor) to result and return

        tester.test_md(333, 424, 5, 28238); // round down
        tester.test_md(333, 593, 7, 28210); // round up

        tester.test_md(-333, 424, 5, -28238);
        tester.test_md(-333, 593, 7, -28210);

        tester.test_md(333, -424, 5, -28238);
        tester.test_md(333, -593, 7, -28210);

        tester.test_md(-333, -424, 5, 28238);
        tester.test_md(-333, -593, 7, 28210);

        // test step 3/4 - with overflow on x on decimal part but one of decimal parts has gcd with divisor > 1
        tester.test_md(438241312999, 3681227029158, 876482625990, 1840613514596);

        // test step 5 - overflow after gcd
        tester.test_md(438241312999, 3681227029158 + 222121, 876482625990, 1840613625656);
    }
}

