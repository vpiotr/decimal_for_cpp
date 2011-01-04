Purpose:     Decimal data type support, for COBOL-like fixed-point
             operations on currency values.
Author:      Piotr Likus
Created:     03/01/2011
Licence:     BSD
Version:     1.0


Example usage:
  using namespace dec;
  decimal<2> value(143125);
  value = value / decimal<2>(333);
  cout << "Result is: " << value.getAsDouble() << endl;
  // should display something like "4337.12"

Directory structure:
\doc     - documentation
\include - headers
\test    - unit tests, Boost-based

