Purpose:     Decimal data type support, for COBOL-like fixed-point
             operations on currency/money values.
Author:      Piotr Likus
Created:     03/01/2011
Licence:     BSD
Version:     1.0

This data type is designed to perform calculation with on-fly roundings
& to support correct compare function (floating-point compare is unreliable).

Values are stored internally using 64-bit integer, so maximum number of
digits is 18.

Precision is user-defined, so you can use this data type for currency rates.

Example usage:
  #include "decimal.h"

  using namespace dec;

  // the following declares currency variable with 2 decimal points
  // initialized with integer value (can be also floating-point)
  decimal<2> value(143125);

  // to use non-decimal constants you need to convert them to decimal
  value = value / decimal<2>(333);

  // output values using conversion to double
  cout << "Result is: " << value.getAsDouble() << endl;
  // this should display something like "4337.12"


Directory structure:
\doc     - documentation
\include - headers
\test    - unit tests, Boost-based

