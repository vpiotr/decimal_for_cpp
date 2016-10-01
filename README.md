# About     
Decimal data type support, for COBOL-like fixed-point operations on currency/money values.

Author: Piotr Likus

Created: 03/01/2011

Modified: 25/09/2016

Licence: BSD

Version: 1.12

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
// displays: Value: 143.13 * 2 is: 286.26
cout << "Value: " << value << " * 2 is: " << (value * 2) << endl;

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
```

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

- VS2015 Community 
- VS2013 Community Update 4
- Code::Blocks 13.12 + MinGW + gcc 4.8.4

Uses C++11 by default, define DEC_NO_CPP11 symbol if your compiler does not support this standard.
To use custom namespace, define DEC_NAMESPACE symbol which should contain your target namespace for decimal type.
