Purpose:     Decimal data type support, for COBOL-like fixed-point
             operations on currency/money values.
Author:      Piotr Likus
Created:     03/01/2011
Modified:    19/09/2016
Licence:     BSD
Version:     1.11

This data type is designed to perform calculation with on-fly  roundings
&  to  support  correct  compare  function  (floating-point  compare  is
unreliable).

Values are stored internally using 64-bit integer, so maximum number of
digits is 18.

Precision is user-defined, so you can use this data  type  for  currency
rates.

To store decimal in file you can use "unbiased" functions or use stream i/o.

Example usage:
  #include "decimal.h"

  using namespace dec;

  // the following declares currency variable with 2 decimal points
  // initialized with integer value (can be also floating-point)
  decimal<2> value(143125);

  // to use non-decimal constants you need to convert them to decimal
  value = value / decimal_cast<2>(333.0);

  // output values
  cout << "Result is: " << value << endl;
  // this should display something like "429.80"

  // to mix decimals with different precision use decimal_cast
  decimal<6> exchangeRate(12.1234);
  value = decimal_cast<2>(decimal_cast<6>(value) * exchangeRate);

  // to fast multiply decimal by int value you can perform:
  decimal<6> exchangeRate2("12.123124");
  cout << "Rate: " << exchangeRate2 << " multiplied by 2 is: " << (exchangeRate2 * 2) << endl;
  // this should display "24.246248"

  cout << "Result 2 is: " << value << endl;
  // this should display something like "5210.64"

  cout << "Result 2<6> is: " << decimal_cast<6>(value) << endl;
  // this should display something like "5210.640000"

For more examples please see \test directory.

Directory structure:
\doc     - documentation (licence etc.)
\include - headers
\test    - unit tests, Boost-based

Code documentation can be generated using Doxygen:
http://www.doxygen.org/

Tested compilers:
- VS2015 Community 
- VS2013 Community Update 4
- Code::Blocks 13.12 + MinGW + gcc 4.8.4

Uses C++11 by default, define DEC_NO_CPP11 symbol if your compiler does not support this standard.
To use custom namespace, define DEC_NAMESPACE symbol which should contain your target namespace for decimal type.
