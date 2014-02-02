/////////////////////////////////////////////////////////////////////////////
// Name:        decimal.h
// Purpose:     Decimal data type support, for COBOL-like fixed-point
//              operations on currency values.
// Author:      Piotr Likus
// Created:     03/01/2011
// Last change: 01/02/2014
// Version:     1.5
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

#include <iosfwd>
#include <iomanip>
#include <sstream>
#include <locale>

#ifndef DEC_EXTERNAL_LIMITS

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X) || (__cplusplus >= 201103L)
#include <cstdint>
#else
#include <stdint.h>
#endif

#endif

namespace dec
{

// ----------------------------------------------------------------------------
// Config section
// ----------------------------------------------------------------------------
// - define DEC_EXTERNAL_INT64 if you do not want internal definition of "int64" data type
//   in this case define "DEC_INT64" somewhere
// - define DEC_EXTERNAL_ROUND if you do not want internal "round()" function
// - define DEC_CROSS_DOUBLE if you want to use double (intead of xdouble) for cross-conversions
// - define DEC_EXTERNAL_LIMITS to define by yourself INT32_MAX

// ----------------------------------------------------------------------------
// Simple type definitions
// ----------------------------------------------------------------------------
#ifndef DEC_EXTERNAL_INT64
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed __int64 DEC_INT64;
#else
typedef signed long long DEC_INT64;
#endif
#endif

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

// round value - convert to int64
inline int64 round(double value) {
  double val1;

  if (value < 0.0)
    val1 = value - 0.5;
  else
    val1 = value + 0.5;

  int64 intPart = int64(val1);
  return intPart;
}

inline int64 round(xdouble value) {
  xdouble val1;

  if (value < 0.0)
    val1 = value - 0.5;
  else
    val1 = value + 0.5;

  int64 intPart = int64(val1);
  return intPart;
}

#endif // DEC_EXTERNAL_ROUND

template <int Prec>
class decimal {
public:
    typedef dec_storage_t raw_data_t;

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

    ~decimal() {}

    inline int64 getPrecFactor() const { return DecimalFactor<Prec>::value; }
    inline int getDecimalPoints() const { return Prec; }

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
        m_value = round(static_cast<double>(DecimalFactor<Prec>::value) * rhs);
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

    const decimal operator-(const decimal &rhs) const {
        decimal result = *this;
        result.m_value -= rhs.m_value;
        return result;
    }

    decimal & operator-=(const decimal &rhs) {
      m_value -= rhs.m_value;
      return *this;
    }

    const decimal operator*(const decimal &rhs) const {
        decimal result = *this;
        //result.m_value = (result.m_value * rhs.m_value) / DecimalFactor<Prec>::value;
        result.m_value =
             multDiv(result.m_value, rhs.m_value, DecimalFactor<Prec>::value);

        return result;
    }

    decimal & operator*=(const decimal &rhs) {
      //m_value = (m_value * rhs.m_value) / DecimalFactor<Prec>::value;
      m_value =
            multDiv(m_value, rhs.m_value, DecimalFactor<Prec>::value);

      return *this;
    }

    const decimal operator/(const decimal &rhs) const {
        decimal result = *this;
        //result.m_value = (result.m_value * DecimalFactor<Prec>::value) / rhs.m_value;
        result.m_value =
            multDiv(result.m_value, DecimalFactor<Prec>::value, rhs.m_value);

        return result;
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
       m_value = round(nval);
    }

    xdouble getAsXDouble() const { return static_cast<xdouble>(m_value) / getPrecFactorXDouble(); }

    void setAsXDouble(xdouble value)
    {
       xdouble nval = value * getPrecFactorXDouble();
       m_value = round(nval);
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

    int64 getAsInteger() const {
        return round(getAsXDouble());
    }

    /// returns two parts: before and after decimal point
    void unpack(int64 &beforeValue, int64 &afterValue) const {
      afterValue = m_value % DecimalFactor<Prec>::value;
      beforeValue = (m_value - afterValue) / DecimalFactor<Prec>::value;
    }

    /// Combines two parts (before and after decimal point) into decimal value
    /// \param[in] before value before decimal point
    /// \param[in] after value after decimal point multiplied by 10^prec
    /// \result Returns *this
    decimal &pack(int64 beforeValue, int64 afterValue) {
      if (Prec > 0) {
        m_value = beforeValue * DecimalFactor<Prec>::value;
        m_value += (afterValue % DecimalFactor<Prec>::value);
       } else
        m_value = beforeValue * DecimalFactor<Prec>::value;
      return *this;
    }

protected:
    inline xdouble getPrecFactorXDouble() const { return static_cast<xdouble>(DecimalFactor<Prec>::value); }
    inline double getPrecFactorDouble() const { return static_cast<double>(DecimalFactor<Prec>::value); }

    // result = (value1 * value2) / divider
    inline static int64 multDiv(int64 value1, int64 value2, int64 divider)
    {
      if ((abs(value1) <= INT32_MAX) || (abs(value2) <= INT32_MAX))
      {
        // no-overflow version
        return
             round(
                 static_cast<cross_float>(value1 * value2)
                 /
                 static_cast<cross_float>(divider)
             );
      } else {
        // overflow can occur - use less precise version
        return
               round(
                   static_cast<cross_float>(value1)
                   *
                   static_cast<cross_float>(value2)
                   /
                   static_cast<cross_float>(divider)
               );
      }
    }

    void init(const decimal &src) { m_value = src.m_value; }
    void init(uint value) { m_value = DecimalFactor<Prec>::value * static_cast<int>(value); }
    void init(int value) { m_value = DecimalFactor<Prec>::value * value; }
    void init(int64 value) { m_value = DecimalFactor<Prec>::value * value; }
    void init(xdouble value) {
      m_value =
         round(
             static_cast<xdouble>(DecimalFactor<Prec>::value) *
             value
         );
    }
    void init(double value) {
      m_value =
         round(
             static_cast<double>(DecimalFactor<Prec>::value) *
             value
         );
    }

    void init(float value) {
      m_value =
         round(
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
             round(
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
    return decimal<Prec>(static_cast<double>(arg));
}

template < int Prec >
decimal<Prec> decimal_cast(int arg)
{
    return decimal<Prec>(static_cast<double>(arg));
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
  output << before << dec_point;
  output << setw(arg.getDecimalPoints()) << setfill('0') << right << after;
}

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
/// \param[in] input input stream
/// \param[out] output decimal value, 0 on error
/// \result Returns true if conversion succeeded
template <int prec, typename StreamType>
bool fromStream(StreamType &input, decimal<prec> &output) {
    using namespace std;
    int64 before, after;
    int sign = 1;

    enum StateEnum { IN_SIGN, IN_BEFORE_FIRST_DIG, IN_BEFORE_DEC, IN_AFTER_DEC, IN_END} state = IN_SIGN;
    const char dec_point = use_facet<numpunct<char> >(input.getloc()).decimal_point();
    enum ErrorCodes { ERR_WRONG_CHAR = -1, ERR_NO_DIGITS = -2, ERR_WRONG_STATE = -3, ERR_STREAM_GET_ERROR = -4 };

    before = after = 0;
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
            } else {
                state = IN_END;
            }
            break;
         case IN_AFTER_DEC:
            if ((c >= '0') && (c <= '9')) {
                after = 10 * after + static_cast<int>(c - '0');
                afterDigitCount++;
                if (afterDigitCount >= prec)
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

  if (error >= 0)
  {
    if (afterDigitCount > 0)
    {
       // convert 4 to 4000 for decimal<4>
       int leftPow = prec - afterDigitCount;
       while (leftPow > 0)
       {
           after *= 10;
           leftPow--;
       }
    }

    if (sign < 0) {
        before = -before;
        after = -after;
    }

    output.pack(before, after);
  } else {
    output.pack(0,0);
  }

  return (error >= 0);
} // function

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
template <class charT, class traits, int prec>
  std::basic_istream<charT, traits> &
    operator>>(std::basic_istream<charT, traits> & is, decimal<prec> & d)
{
  if (!fromStream<prec>(is, d))
    d.setUnbiased(0);
  return is;
}

// output
template <class charT, class traits, int prec>
  std::basic_ostream<charT, traits> &
    operator<<(std::basic_ostream<charT, traits> & os, const decimal<prec> & d)
{
  std::string helper;
  os << toString(d, helper);
  return os;
}

/// Imports decimal from string
/// Used format: {-}bbbb.aaaa where
/// {-} is optional '-' sign character
/// '.' is locale-dependent decimal point character
/// bbbb is stream of digits before decimal point
/// aaaa is stream of digits after decimal point
template <typename T>
T fromString (const std::string &str) {
    std::istringstream is(str);
    T t;
    is >> t;
    return t;
}

} // namespace
#endif // _DECIMAL_H__
