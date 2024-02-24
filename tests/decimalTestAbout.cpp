//
// Demo of features mentioned in project README
// Created by piotr on 7/19/21.
//

#include "decimal.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(decimalAbout) {
    using namespace dec;
    using namespace std;

    // the following declares currency variable with 2 decimal points
    // initialized with integer value (can be also floating-point)
    decimal<2> value(143125);

    // displays: Value #1 is: 143125.00
    cout << "Value #1 is: " << value << endl;

    // declare precise value with digits after decimal point
    decimal<2> b("0.11");

    // perform calculations as with any other numeric type
    value += b;

    // displays: Value #2 is: 143125.11
    cout << "Value #2 is: " << value << endl;

    // automatic rounding performed here
    value /= 1000;

    // displays: Value #3 is: 143.13
    cout << "Value #3 is: " << value << endl;

    // integer multiplication and division can be used directly in expression
    // when integer is on right side
    // displays: Value: 143.13 * 2 is: 286.26
    cout << "Value: " << value << " * 2 is: " << (value * 2) << endl;

    // to use integer on left side you need to cast it
    // displays: Value: 2 * 143.13 is: 286.26
    cout << "Value: 2 * " << value << " is: " << (decimal_cast<2>(2) * value) << endl;

    // to use non-integer constants in expressions you need to use decimal_cast
    value = value * decimal_cast<2>("3.33") / decimal_cast<2>(333.0);

    // displays: Value #4 is: 1.43
    cout << "Value #4 is: " << value << endl;

    // to mix decimals with different precision use decimal_cast
    // it will round result automatically
    decimal<6> exchangeRate(12.1234);
    value = decimal_cast<2>(decimal_cast<6>(value) * exchangeRate);

    // displays: Value #5 is: 17.34
    cout << "Value #5 is: " << value << endl;

    // with doubles you would have to perform rounding each time it is required:
    double dvalue = 143125.0;
    dvalue += 0.11;
    dvalue /= 1000.0;
    dvalue = round(dvalue * 100.0) / 100.0;
    dvalue = (dvalue * 3.33) / 333.0;
    dvalue = round(dvalue * 100.0) / 100.0;
    dvalue = dvalue * 12.1234;
    dvalue = round(dvalue * 100.0) / 100.0;

    // displays: Value #5 calculated with double is: 17.34
    cout << "Value #5 calculated with double is: " << fixed << setprecision(2) << dvalue << endl;

    // supports optional strong typing, e.g.
    // depending on configuration mixing precision can be forbidden
    // or handled automatically
    decimal<2> d2("12.03");
    decimal<4> d4("123.0103");

    // compiles always
    d2 += d2;
    d2 += decimal_cast<2>(d4);
    d4 += decimal_cast<4>(d2);

#if DEC_TYPE_LEVEL >= 2
    // potential precision loss
    // this will fail to compile if you define DEC_TYPE_LEVEL = 0 or 1
    d2 += d4;
#endif

#if DEC_TYPE_LEVEL >= 1
    // (possibly unintentional) mixed precision without type casting
    // this will fail to compile if you define DEC_TYPE_LEVEL = 0
    d4 += d2;
#endif

    // for default setup displays: mixed d2 = 417.15
    cout << "mixed d2 = " << d2 << endl;
    // for default setup displays: mixed d4 = 687.2303
    cout << "mixed d4 = " << d4 << endl;
}

