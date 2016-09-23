/////////////////////////////////////////////////////////////////////////////
// Name:        decimal.h
// Purpose:     Decimal data type support, for COBOL-like fixed-point
//              operations on currency values.
// Author:      Piotr Likus
// Created:     03/01/2011
// Last change: 19/09/2016
// Version:     1.11
// Licence:     BSD
/////////////////////////////////////////////////////////////////////////////

#ifndef _DECIMAL_H__
#define _DECIMAL_H__

// ----------------------------------------------------------------------------
// Description
// ----------------------------------------------------------------------------
/// \file decimal.h
///
/// Decimal value type. Use for capital calculations.
/// Note: maximum handled value is: +9,223,372,036,854,775,807 (divided by prec)
///
/// Sample usage:
///   using namespace dec;
///   decimal<2> value(143125);
///   value = value / decimal_cast<2>(333);
///   cout << "Result is: " << value << endl;

// ----------------------------------------------------------------------------
// Config section
// ----------------------------------------------------------------------------
// - define DEC_EXTERNAL_INT64 if you do not want internal definition of "int64" data type
//   in this case define "DEC_INT64" somewhere
// - define DEC_EXTERNAL_ROUND if you do not want internal "round()" function
// - define DEC_CROSS_DOUBLE if you want to use double (instead of xdouble) for cross-conversions
// - define DEC_EXTERNAL_LIMITS to define by yourself DEC_MAX_INT32
// - define DEC_NO_CPP11 if your compiler does not support C++11

#include <iosfwd>
#include <iomanip>
#include <sstream>
#include <locale>

// --> include headers for limits and int64_t

#ifndef DEC_NO_CPP11
#include <cstdint>
#include <limits>

#else

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X) || (__cplusplus >= 201103L)
#include <cstdint>
#else
#include <stdint.h>
#endif // defined
#endif // DEC_NO_CPP11

// <--

// --> define DEC_MAX_INTxx, DEC_MIN_INTxx if required

#ifndef DEC_NAMESPACE
#define DEC_NAMESPACE dec
#endif // DEC_NAMESPACE

#ifndef DEC_EXTERNAL_LIMITS
#ifndef DEC_NO_CPP11
//#define DEC_MAX_INT32 ((std::numeric_limits<int32_t>::max)())
#define DEC_MAX_INT64 ((std::numeric_limits<int64_t>::max)())
#define DEC_MIN_INT64 ((std::numeric_limits<int64_t>::min)())
#else
//#define DEC_MAX_INT32 INT32_MAX
#define DEC_MAX_INT64 INT64_MAX
#define DEC_MIN_INT64 INT64_MIN
#endif // DEC_NO_CPP11
#endif // DEC_EXTERNAL_LIMITS

// <--

namespace DEC_NAMESPACE
{

// ----------------------------------------------------------------------------
// Simple type definitions
// ----------------------------------------------------------------------------

// --> define DEC_INT64 if required
#ifndef DEC_EXTERNAL_INT64
#ifndef DEC_NO_CPP11
typedef int64_t DEC_INT64;
#else
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed __int64 DEC_INT64;
#else
typedef signed long long DEC_INT64;
#endif
#endif
#endif // DEC_EXTERNAL_INT64
// <--

typedef DEC_INT64 int64;
// type for storing currency value internally
typedef int64 dec_storage_t;
typedef unsigned int uint;
// xdouble is an "extended double" - can be long double, __float128, _Quad - as you wish
typedef long double xdouble;


// ----------------------------------------------------------------------------
// Forward class definitions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Class definitions
// ----------------------------------------------------------------------------
template <int Prec> struct DecimalFactor {
    static const int64 value = 10 * DecimalFactor<Prec - 1>::value;
};

template <> struct DecimalFactor<0> {
    static const int64 value = 1;
};

template <> struct DecimalFactor<1> {
    static const int64 value = 10;
};

#ifndef DEC_EXTERNAL_ROUND

// round floating point value and convert to int64
template<class T>
inline int64 round(T value) {
  T val1;

  if (value < 0.0)
    val1 = value - 0.5;
  else
    val1 = value + 0.5;

  int64 intPart = static_cast<int64>(val1);

  return intPart;
}

// calculate output = round(a / b), where output, a, b are int64
inline bool div_rounded(int64 &output, int64 a, int64 b) {
    int64 divisorCorr = std::abs(b) / 2;
    if (a >= 0) {
        if (DEC_MAX_INT64 - a >= divisorCorr) {
            output = (a + divisorCorr) / b;
            return true;
        }
    }
    else {
        if (-(DEC_MIN_INT64 - a) >= divisorCorr) {
            output = (a - divisorCorr) / b;
            return true;
        }
    }

    output = 0;
    return false;
}

#endif // DEC_EXTERNAL_ROUND

// default rounding policy - to nearest integer
class def_round_policy {
public:
    template<class T>
    static int64 round(T value) {
        return DEC_NAMESPACE::round(value);
    }

    static bool div_rounded(int64 &output, int64 a, int64 b) {
        return DEC_NAMESPACE::div_rounded(output, a, b);
    }
};

// no-rounding policy (decimal places stripped)
class null_round_policy {
public:
    template<class T>
    static int64 round(T value) {
        return static_cast<int64>(value);
    }

    static bool div_rounded(int64 &output, int64 a, int64 b) {
        output = a / b;
        return true;
    }
};

enum { max_decimal_points = 18, mult_prec_max_prec = 9 };

template <int Prec, class RoundPolicy = def_round_policy>
class decimal {
public:
    typedef dec_storage_t raw_data_t;
    enum { decimal_points = Prec };

#ifdef DEC_CROSS_DOUBLE
    typedef double cross_float;
#else
    typedef xdouble cross_float;
#endif

    decimal() { init(0); }
    decimal(const decimal &src) { init(src); }
    explicit decimal(uint value) { init(value); }
    explicit decimal(int value) { init(value); }
    explicit decimal(int64 value) { init(value); }
    explicit decimal(xdouble value) { init(value); }
    explicit decimal(double value) { init(value); }
    explicit decimal(float value) { init(value); }
    explicit decimal(int64 value, int64 precFactor) { initWithPrec(value, precFactor); }
    explicit decimal(const std::string &value) { fromString(value, *this); }

    ~decimal() {}

    static int64 getPrecFactor() { return DecimalFactor<Prec>::value; }
    static int getDecimalPoints() { return Prec; }

    decimal & operator=(const decimal &rhs) {
        if (&rhs != this) m_value = rhs.m_value;
        return *this;
    }

    decimal & operator=(int64 rhs) {
        m_value = DecimalFactor<Prec>::value * rhs;
        return *this;
    }

    decimal & operator=(int rhs) {
        m_value = DecimalFactor<Prec>::value * rhs;
        return *this;
    }

    decimal & operator=(double rhs)
    {
        m_value = RoundPolicy::round(static_cast<double>(DecimalFactor<Prec>::value) * rhs);
        return *this;
    }

    bool operator==(const decimal &rhs) const {
        return (m_value == rhs.m_value);
    }

    bool operator<(const decimal &rhs) const {
        return (m_value < rhs.m_value);
    }

    bool operator<=(const decimal &rhs) const {
        return (m_value <= rhs.m_value);
    }

    bool operator>(const decimal &rhs) const {
        return (m_value > rhs.m_value);
    }

    bool operator>=(const decimal &rhs) const {
        return (m_value >= rhs.m_value);
    }

    bool operator!=(const decimal &rhs) const {
        return !(*this == rhs);
    }

    const decimal operator+(const decimal &rhs) const {
        decimal result = *this;
        result.m_value += rhs.m_value;
        return result;
    }

    decimal & operator+=(const decimal &rhs) {
      m_value += rhs.m_value;
      return *this;
    }

    const decimal operator+() const {
       return *this;
    }

    const decimal operator-() const {
       decimal result = *this;
       result.m_value = -result.m_value;
       return result;
    }

    const decimal operator-(const decimal &rhs) const {
        decimal result = *this;
        result.m_value -= rhs.m_value;
        return result;
    }

    decimal & operator-=(const decimal &rhs) {
        m_value -= rhs.m_value;
        return *this;
    }

    const decimal operator*(int rhs) const {
        decimal result = *this;
        result.m_value *= rhs;
        return result;
    }

    const decimal operator*(int64 rhs) const {
        decimal result = *this;
        result.m_value *= rhs;
        return result;
    }

    const decimal operator*(const decimal &rhs) const {
        decimal result = *this;
        //result.m_value = (result.m_value * rhs.m_value) / DecimalFactor<Prec>::value;
        if (Prec <= mult_prec_max_prec) {
            result.m_value =
                multPrec(result.m_value, rhs.m_value, DecimalFactor<Prec>::value);
        }
        else {
            result.m_value =
                multDiv(result.m_value, rhs.m_value, DecimalFactor<Prec>::value);
        }
        return result;
    }

    decimal & operator*=(int rhs) {
        m_value *= rhs;
        return *this;
    }

    decimal & operator*=(int64 rhs) {
        m_value *= rhs;
        return *this;
    }

    decimal & operator*=(const decimal &rhs) {
        //m_value = (m_value * rhs.m_value) / DecimalFactor<Prec>::value;

        if (Prec <= mult_prec_max_prec) {
            m_value =
                multPrec(m_value, rhs.m_value, DecimalFactor<Prec>::value);
        }
        else {
            m_value =
                multDiv(m_value, rhs.m_value, DecimalFactor<Prec>::value);
        }

        return *this;
    }

    const decimal operator/(int rhs) const {
        decimal result = *this;

        if (!RoundPolicy::div_rounded(result.m_value, this->m_value, rhs)) {
            result.m_value =
                multDiv(result.m_value, 1, rhs);
        }

        return result;
    }

    const decimal operator/(int64 rhs) const {
        decimal result = *this;

        if (!RoundPolicy::div_rounded(result.m_value, this->m_value, rhs)) {
            result.m_value =
                multDiv(result.m_value, 1, rhs);
        }

        return result;
    }

    const decimal operator/(const decimal &rhs) const {
        decimal result = *this;
        //result.m_value = (result.m_value * DecimalFactor<Prec>::value) / rhs.m_value;
        result.m_value =
            multDiv(result.m_value, DecimalFactor<Prec>::value, rhs.m_value);

        return result;
    }

    decimal & operator/=(int rhs) {
        if (!RoundPolicy::div_rounded(this->m_value, this->m_value, rhs)) {
            this->m_value =
                multDiv(this->m_value, 1, rhs);
        }
        return *this;
    }

    decimal & operator/=(int64 rhs) {
        if (!RoundPolicy::div_rounded(this->m_value, this->m_value, rhs)) {
            this->m_value =
                multDiv(this->m_value, 1, rhs);
        }
        return *this;
    }

    decimal & operator/=(const decimal &rhs) {
      //m_value = (m_value * DecimalFactor<Prec>::value) / rhs.m_value;
      m_value =
            multDiv(m_value, DecimalFactor<Prec>::value, rhs.m_value);

      return *this;
    }

    double getAsDouble() const { return static_cast<double>(m_value) / getPrecFactorDouble(); }

    void setAsDouble(double value)
    {
       double nval = value * getPrecFactorDouble();
       m_value = RoundPolicy::round(nval);
    }

    xdouble getAsXDouble() const { return static_cast<xdouble>(m_value) / getPrecFactorXDouble(); }

    void setAsXDouble(xdouble value)
    {
       xdouble nval = value * getPrecFactorXDouble();
       m_value = RoundPolicy::round(nval);
    }

    // returns integer value = real_value * (10 ^ precision)
    // use to load/store decimal value in external memory
    int64 getUnbiased() const { return m_value; }
    void setUnbiased(int64 value) { m_value = value; }

    decimal<Prec> abs() const {
        if (m_value >= 0)
            return *this;
        else
            return (decimal<Prec>(0) - *this);
    }

    /// returns value rounded to integer using active rounding policy
    int64 getAsInteger() const {
        int64 result;
        RoundPolicy::div_rounded(result, m_value, DecimalFactor<Prec>::value);
        return result;
    }

    /// overwrites internal value with integer
    void setAsInteger(int64 value) {
        m_value = DecimalFactor<Prec>::value * value;
    }

    /// Returns two parts: before and after decimal point
    /// For negative values both numbers are negative or zero.
    void unpack(int64 &beforeValue, int64 &afterValue) const {
      afterValue = m_value % DecimalFactor<Prec>::value;
      beforeValue = (m_value - afterValue) / DecimalFactor<Prec>::value;
    }

    /// Combines two parts (before and after decimal point) into decimal value.
    /// Both input values have to have the same sign for correct results.
    /// Does not perform any rounding or input validation - afterValue must be less than 10^prec.
    /// \param[in] beforeValue value before decimal point
    /// \param[in] afterValue value after decimal point multiplied by 10^prec
    /// \result Returns *this
    decimal &pack(int64 beforeValue, int64 afterValue) {
      if (Prec > 0) {
        m_value = beforeValue * DecimalFactor<Prec>::value;
        m_value += (afterValue % DecimalFactor<Prec>::value);
       } else
        m_value = beforeValue * DecimalFactor<Prec>::value;
      return *this;
    }

    /// Version of pack() with rounding, sourcePrec specifies precision of source values.
    /// See also @pack.
    template<int sourcePrec>
    decimal &pack_rounded(int64 beforeValue, int64 afterValue) {
      decimal<sourcePrec> temp;
      temp.pack(beforeValue, afterValue);
      decimal<Prec> result(temp.getUnbiased(), temp.getPrecFactor());

      *this = result;
      return *this;
    }

    static decimal buildWithExponent(int64 mantissa, int exponent) {
       decimal result;
       result.setWithExponent(mantissa, exponent);
       return result;
    }

    static decimal &buildWithExponent(decimal &output, int64 mantissa, int exponent) {
       output.setWithExponent(mantissa, exponent);
       return output;
    }

    void setWithExponent(int64 mantissa, int exponent) {

        int exponentForPack = exponent+Prec;

        if (exponentForPack < 0) {
            int64 newValue;

            if (!RoundPolicy::div_rounded(newValue, mantissa, pow10(-exponentForPack))) {
               newValue = 0;
            }

            m_value = newValue;
        } else {
            m_value = mantissa * pow10(exponentForPack);
        }
    }

    void getWithExponent(int64 &mantissa, int &exponent) const {
        int64 value = m_value;
        int exp = -Prec;

        // normalize
        while (value%10 == 0) {
            value /= 10;
            exp++;
        }

        mantissa = value;
        exponent = exp;
    }

protected:
    inline xdouble getPrecFactorXDouble() const { return static_cast<xdouble>(DecimalFactor<Prec>::value); }
    inline double getPrecFactorDouble() const { return static_cast<double>(DecimalFactor<Prec>::value); }

    int64 pow10(int n) {
        static const int64 decimalFactorTable[] =
           {1,
            10,
            100,
            1000,
            10000,
            100000,
            1000000,
            10000000,
            100000000,
            1000000000,
            10000000000,
            100000000000,
            1000000000000,
            10000000000000,
            100000000000000,
            1000000000000000,
            10000000000000000,
            100000000000000000,
            1000000000000000000};

        if (n >= 0 && n <= max_decimal_points) {
          return decimalFactorTable[n];
        } else {
          return 0;
        }
    }

    // calculate greatest common divisor
    static int64 gcd(int64 a, int64 b)
    {
        int64 c;
        while (a != 0) {
            c = a;
            a = b % a;
            b = c;
        }
        return b;
    }

    // count number of bits required to store given value
    static int bitcnt(int64 value) {
        int res = 0;
        while (value != 0) {
            value = value >> 1;
            ++res;
        }
        return res;
    }

    // result = (value1 * value2) / divisor
    inline static int64 multDiv(int64 value1, int64 value2, int64 divisor)
    {
      const int DEC_INT64_BITCNT_NO_SIGN = 63;

      // minimalize value1 & divisor
      if ((value1 != 0) && (divisor != 0))
      {
          int64 c = gcd(value1, divisor);
          if (c != 1) {
              value1 /= c;
              divisor /= c;
          }
      }

      // minimalize value2 & divisor
      if ((value2 != 0) && (divisor != 0))
      {
          int64 c = gcd(value2, divisor);
          if (c != 1) {
              value2 /= c;
              divisor /= c;
          }
      }

      int bitCnt1 = bitcnt(abs(value1));
      int bitCnt2 = bitcnt(abs(value2));

      if (bitCnt1 + bitCnt2 < DEC_INT64_BITCNT_NO_SIGN)
      {
          // no-overflow version
          int64 midRes = value1 * value2;

          int64 result;
          if (RoundPolicy::div_rounded(result, midRes, divisor))
              return result;
      }

      // overflow can occur - use less precise version
      return
          RoundPolicy::round(
                   static_cast<cross_float>(value1)
                   *
                   static_cast<cross_float>(value2)
                   /
                   static_cast<cross_float>(divisor)
               );
    }

    inline static int64 multPrec(int64 value1, int64 value2, int64 precValue)
    {
        const int64 value1abs = abs(value1);
        const int64 value2abs = abs(value2);
        const bool negative = (value1 < 0) != (value2 < 0);
        const int64 value1int = value1abs / precValue;
        const int64 value1dec = value1abs % precValue;
        const int64 value2int = value2abs / precValue;
        const int64 value2dec = value2abs % precValue;

        int64 resDecPart;
        if (!RoundPolicy::div_rounded(resDecPart, value1dec * value2dec, precValue))
           resDecPart = 0;

        int64 result = value1int * value2int * precValue
            + value1int * value2dec
            + value1dec * value2int
            + resDecPart;

        if (negative)
          result = -result;

        return result;
    }

    void init(const decimal &src) { m_value = src.m_value; }
    void init(uint value) { m_value = DecimalFactor<Prec>::value * value; }
    void init(int value) { m_value = DecimalFactor<Prec>::value * value; }
    void init(int64 value) { m_value = DecimalFactor<Prec>::value * value; }
    void init(xdouble value) {
      m_value =
          RoundPolicy::round(
             static_cast<xdouble>(DecimalFactor<Prec>::value) *
             value
         );
    }
    void init(double value) {
      m_value =
          RoundPolicy::round(
             static_cast<double>(DecimalFactor<Prec>::value) *
             value
         );
    }

    void init(float value) {
      m_value =
          RoundPolicy::round(
             static_cast<double>(DecimalFactor<Prec>::value) *
             static_cast<double>(value)
         );
    }
    void initWithPrec(int64 value, int64 precFactor) {
        int64 ownFactor = DecimalFactor<Prec>::value;

        if (ownFactor == precFactor) {
        // no conversion required
            m_value = value;
        } else {
        // conversion
          m_value =
              RoundPolicy::round(
                 static_cast<cross_float>(value)
                 *
                 (
                   static_cast<cross_float>(ownFactor) /
                   static_cast<cross_float>(precFactor)
                 )
             );
        }
    }

    template<typename T>
    static T abs(T value) {
      if (value < 0)
        return -value;
      else
        return value;
    }
protected:
    dec_storage_t m_value;
};

// ----------------------------------------------------------------------------
// Pre-defined types
// ----------------------------------------------------------------------------
typedef decimal<2> decimal2;
typedef decimal<4> decimal4;
typedef decimal<6> decimal6;

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------
template < int Prec, class T >
decimal<Prec> decimal_cast(const T &arg)
{
    return decimal<Prec>(arg.getUnbiased(), arg.getPrecFactor());
}

template < int Prec>
decimal<Prec> decimal_cast(double arg)
{
    return decimal<Prec>(arg);
}

// Note: this specialization is required due to error in VS 2010
// which incorrectly calculates expression like dec::decimal<6>(a * b)
// for uint arguments. Such expression is initialized with a strange number.
//
// So instead of:
//   c = dec::decimal<6>(a * b);
// use:
//   c = dec::decimal_cast<6>(a * b);
template < int Prec >
decimal<Prec> decimal_cast(uint arg)
{
    decimal<Prec> result(arg);
    return result;
}

template < int Prec >
decimal<Prec> decimal_cast(int arg)
{
    decimal<Prec> result(arg);
    return result;
}

/// Exports decimal to stream
/// Used format: {-}bbbb.aaaa where
/// {-} is optional '-' sign character
/// '.' is locale-dependent decimal point character
/// bbbb is stream of digits before decimal point
/// aaaa is stream of digits after decimal point
template <int prec, typename StreamType>
void toStream(const decimal<prec> &arg, StreamType &output) {
  using namespace std;

  int64 before, after;
  int sign;

  arg.unpack(before, after);
  sign = 1;

  if (before < 0)
  {
    sign = -1;
    before = -before;
  }

  if (after < 0)
  {
    sign = -1;
    after = -after;
  }

  if (sign < 0)
    output << "-";

  const char dec_point = use_facet<numpunct<char> >(output.getloc()).decimal_point();
  output << before;
  if(arg.getDecimalPoints() > 0)
  {
    output << dec_point;
    output << setw(arg.getDecimalPoints()) << setfill('0') << right << after;
  }
}

namespace details {

/// Extract values from stream ready to be packed to decimal
template <typename StreamType>
bool parse_unpacked(StreamType &input, int &sign, int64 &before, int64 &after, int &decimalDigits) {
    using namespace std;

    enum StateEnum { IN_SIGN, IN_BEFORE_FIRST_DIG, IN_BEFORE_DEC, IN_AFTER_DEC, IN_END} state = IN_SIGN;
    const numpunct<char> *facet = has_facet<numpunct<char> >(input.getloc())?
        &use_facet<numpunct<char> >(input.getloc()):
        NULL;
    const char dec_point = (facet != NULL)?facet->decimal_point():'.';
    const bool thousands_grouping = (facet != NULL)?(!facet->grouping().empty()):false;
    const char thousands_sep = (facet != NULL)?facet->thousands_sep():',';
    enum ErrorCodes { ERR_WRONG_CHAR = -1, ERR_NO_DIGITS = -2, ERR_WRONG_STATE = -3, ERR_STREAM_GET_ERROR = -4 };

    before = after = 0;
    sign = 1;

    int error = 0;
    int digitsCount = 0;
    int afterDigitCount = 0;
    char c;

    while ((input) && (state != IN_END))          // loop while extraction from file is possible
    {
        c = static_cast<char>(input.get());

        switch (state) {
          case IN_SIGN:
            if (c == '-') {
                sign = -1;
                state = IN_BEFORE_FIRST_DIG;
            } else if (c == '+') {
                state = IN_BEFORE_FIRST_DIG;
            } else if ((c >= '0') && (c <= '9'))
            {
                state = IN_BEFORE_DEC;
                before = static_cast<int>(c - '0');
                digitsCount++;
            } else if (c == dec_point) {
                state = IN_AFTER_DEC;
            } else if ((c != ' ') && (c != '\t')) {
                state = IN_END;
                error = ERR_WRONG_CHAR;
            }
            // else ignore char
            break;
         case IN_BEFORE_FIRST_DIG:
            if ((c >= '0') && (c <= '9')) {
                before = 10 * before + static_cast<int>(c - '0');
                state = IN_BEFORE_DEC;
                digitsCount++;
            } else if (c == dec_point) {
                state = IN_AFTER_DEC;
            } else {
                state = IN_END;
                error = ERR_WRONG_CHAR;
            }
            break;
         case IN_BEFORE_DEC:
            if ((c >= '0') && (c <= '9')) {
                before = 10 * before + static_cast<int>(c - '0');
                digitsCount++;
            } else if (c == dec_point) {
                state = IN_AFTER_DEC;
            } else if (thousands_grouping && c == thousands_sep) {
                ; // ignore the char
            } else {
                state = IN_END;
            }
            break;
         case IN_AFTER_DEC:
            if ((c >= '0') && (c <= '9')) {
                after = 10 * after + static_cast<int>(c - '0');
                afterDigitCount++;
                if (afterDigitCount >= DEC_NAMESPACE::max_decimal_points)
                   state = IN_END;
            } else {
                state = IN_END;
                if (digitsCount == 0) {
                    error = ERR_NO_DIGITS;
                }
            }
            break;
         default:
            error = ERR_WRONG_STATE;
            state = IN_END;
            break;
        } // switch state
  } // while stream good & not end

  decimalDigits = afterDigitCount;

  if (error >= 0)
  {

    if (sign < 0) {
        before = -before;
        after = -after;
    }

  } else {
    before = after = 0;
  }

  return (error >= 0);
} // function

}; // namespace

/// Converts stream of chars to decimal
/// Handles the following formats ('.' is selected from locale info):
/// \code
/// 123
/// -123
/// 123.0
/// -123.0
/// 123.
/// .123
/// 0.
/// -.123
/// \endcode
/// Spaces and tabs on the front are ignored.
/// Performs rounding when provided value has higher precision than in output type.
/// \param[in] input input stream
/// \param[out] output decimal value, 0 on error
/// \result Returns true if conversion succeeded
template <typename decimal_type, typename StreamType>
bool fromStream(StreamType &input, decimal_type &output) {
  int sign, afterDigits;
  int64 before, after;
  bool result = details::parse_unpacked(input, sign, before, after, afterDigits);
  if (result) {
    if (afterDigits <= decimal_type::decimal_points) {
        // direct mode
        int corrCnt = decimal_type::decimal_points - afterDigits;
        while(corrCnt > 0) {
          after *= 10;
          --corrCnt;
        }
        output.pack(before, after);
    } else {
        // rounding mode
        int corrCnt = afterDigits;
        int64 decimalFactor = 1;
        while(corrCnt > 0) {
          before *= 10;
          decimalFactor *= 10;
          --corrCnt;
        }
        decimal_type temp(before + after, decimalFactor);
        output = temp;
    }
  } else {
    output = decimal_type(0);
  }
  return result;
}

/// Exports decimal to string
/// Used format: {-}bbbb.aaaa where
/// {-} is optional '-' sign character
/// '.' is locale-dependent decimal point character
/// bbbb is stream of digits before decimal point
/// aaaa is stream of digits after decimal point
template <int prec>
std::string &toString(const decimal<prec> &arg, std::string &output) {
  using namespace std;

  ostringstream out;
  toStream(arg, out);
  output = out.str();
  return output;
}

/// Exports decimal to string
/// Used format: {-}bbbb.aaaa where
/// {-} is optional '-' sign character
/// '.' is locale-dependent decimal point character
/// bbbb is stream of digits before decimal point
/// aaaa is stream of digits after decimal point
template <int prec>
std::string toString(const decimal<prec> &arg) {
  std::string res;
  toString(arg, res);
  return res;
}

// input
template <class charT, class traits, class decimal_type>
  std::basic_istream<charT, traits> &
      operator>>(std::basic_istream<charT, traits> & is, decimal_type & d)
{
  if (!fromStream(is, d))
     d.setUnbiased(0);
  return is;
}

// output
template <class charT, class traits, int prec>
  std::basic_ostream<charT, traits> &
    operator<<(std::basic_ostream<charT, traits> & os, const decimal<prec> & d)
{
  toStream(d, os);
  return os;
}

/// Imports decimal from string
/// Used format: {-}bbbb.aaaa where
/// {-} is optional '-' sign character
/// '.' is locale-dependent decimal point character
/// bbbb is stream of digits before decimal point
/// aaaa is stream of digits after decimal point
template <typename T>
T fromString(const std::string &str) {
    std::istringstream is(str);
    T t;
    is >> t;
    return t;
}

template <typename T>
void fromString(const std::string &str, T &out) {
    std::istringstream is(str);
    is >> out;
}

} // namespace
#endif // _DECIMAL_H__
