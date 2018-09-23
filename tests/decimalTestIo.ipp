/////////////////////////////////////////////////////////////////////////////
// Name:        decimalTest.ipp
// Purpose:     Test decimal type - I/O functions.
// Author:      Piotr Likus
// Modified by:
// Created:     18/06/2012
// Licence:     BSD
/////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "decimal.h"

using namespace std;

BOOST_AUTO_TEST_CASE(decimalIo)
        {
                dec::decimal<4> a(123.17);
                dec::decimal<4> b;
                cout << "a = " << a << endl;
                cout << "Enter b and press enter: ";
                cin >> b;
                cout << "b = " << b << endl;
                cout << "a + b = " << a+b << endl;
        }

BOOST_AUTO_TEST_CASE(decimalReadme)
        {
                using namespace dec;
                // the following declares currency variable with 2 decimal points
                // initialized with integer value (can be also floating-point)
                decimal<2> value(143125);

                // to use non-decimal constants you need to convert them to decimal
                value = value / decimal_cast<2>(333.0);

                // output values
                cout << "Result is: " << value << endl;

                // to mix decimals with different precision use decimal_cast
                decimal<6> exchangeRate(12.1234);
                value = decimal_cast<2>(decimal_cast<6>(value) * exchangeRate);
                cout << "Result 2 is: " << value << endl;
                // this should display something like "5210.64"
                cout << "Result 2<6> is: " << decimal_cast<6>(value) << endl;
                // this should display something like "5210.640000"
        }
