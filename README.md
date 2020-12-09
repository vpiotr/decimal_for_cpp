# About     
Decimal data type support, for COBOL-like fixed-point operations on currency/money values.

![decimal_for_cpp](https://github.com/vpiotr/decimal_for_cpp/workflows/decimal_for_cpp/badge.svg?branch=master)

Author: Piotr Likus

Created: 03/01/2011

Modified: 30/11/2020

Licence: BSD

Version: 1.17


This data type is designed to perform calculation with on-fly  roundings
&  to  support  correct  compare  function  (floating-point  compare  is
unreliable).

Values are stored internally using 64-bit integer, so maximum number of
digits is 18.

Precision is user-defined, so you can use this data  type  for  currency
rates.

To store decimal in file you can use "unbiased" functions or use stream i/o.

# Examples

Example usage:
```c++
#include "decimal.h"

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

// supports decimal and thousand separator localization
dec::decimal_format format(',', '.');

std::string srcText   = "315499999999999.98";
std::string formatted = "315.499.999.999.999,98";
dec::decimal<2> srcDecimal(srcText);

BOOST_CHECK_EQUAL(dec::toString(srcDecimal, format), formatted);

```

# Supported rounding modes:

* def_round_policy: default rounding (arithmetic)
* null_round_policy: round towards zero = truncate
* half_down_round_policy: round half towards negative infinity
* half_up_round_policy: round half towards positive infinity
* half_even_round_policy: bankers' rounding, convergent rounding, statistician's rounding, Dutch rounding, Gaussian rounding
* ceiling_round_policy: round towards positive infinity
* floor_round_policy: round towards negative infinity
* round_down_round_policy: round towards zero = truncate
* round_up_round_policy: round away from zero

In order to use one of these rounding modes you need to declare your decimal variable like this:

    dec::decimal<2, half_even_round_policy> a;
    
and it will perform required rounding automatically - for example during assignment or arithmetic operations.    

# Testing

In order to test the library:

    cd ~/tmp
    git clone https://github.com/vpiotr/decimal_for_cpp.git
    cd decimal_for_cpp
    mkdir _build
    cd _build
    
    # to create makefile with test support
    cmake ..    
    # or
    cmake -DBUILD_TESTING=ON ..

    # to create makefile without test support (and to avoid Boost unit testing)
    cmake -DBUILD_TESTING=OFF ..
    
    # to build or install library (only required for testing)
    make all
    
    # to execute all test runners 
    make test
    
    # to execute specific runner
    ./test_runner


# Other information
For more examples please see \test directory.

Directory structure:
```
\doc     - documentation (licence etc.)
\include - headers
\test    - unit tests, Boost-based
```

Code documentation can be generated using Doxygen:
http://www.doxygen.org/

Tested compilers:

- VS2019 Community (MSVC++ 14.2)
- gcc 9.3.0

Uses C++11 by default, define DEC_NO_CPP11 symbol if your compiler does not support this standard.
To use custom namespace, define DEC_NAMESPACE symbol which should contain your target namespace for decimal type.
For full list of configuration options see "Config section" in decimal.h file.

For list of project contributors, currently open issues or latest version see project site:
https://github.com/vpiotr/decimal_for_cpp


