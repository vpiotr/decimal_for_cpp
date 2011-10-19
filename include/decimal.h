/////////////////////////////////////////////////////////////////////////////
// Name:        decimal.h
// Purpose:     Decimal data type support, for COBOL-like fixed-point
//              operations on currency values.
// Author:      Piotr Likus
// Modified by:
// Created:     03/01/2011
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
///   value = value / decimal<2>(333);
///   cout << "Result is: " << value.getAsDouble() << endl;

namespace dec
{

// ----------------------------------------------------------------------------
// Config section
// ----------------------------------------------------------------------------
// - define DEC_EXTERNAL_INT64 if you do not want internal definition of "int64" data type
//   in this case define "DEC_INT64" somewhere
// - define DEC_EXTERNAL_ROUND if you do not want internal "round()" function
// - define DEC_CROSS_DOUBLE if you want to use double (intead of xdouble) for cross-conversions

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
typedef int64 dec_storage_t;
typedef unsigned int uint;
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
    static const int value = 10 * DecimalFactor<Prec - 1>::value;
};

template <> struct DecimalFactor<0> {
    static const int value = 1;
};

template <> struct DecimalFactor<1> {
    static const int value = 10;
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

#endif //DEC_EXTERNAL_ROUND

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
    explicit decimal(int64 value, int precFactor) { initWithPrec(value, precFactor); }

    ~decimal() {}

    inline int getPrec() const { return DecimalFactor<Prec>::value; }

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
             round(
                 static_cast<cross_float>(result.m_value * rhs.m_value)
                 /
                 static_cast<cross_float>(DecimalFactor<Prec>::value)
             );

        return result;
    }

    decimal & operator*=(const decimal &rhs) {
      //m_value = (m_value * rhs.m_value) / DecimalFactor<Prec>::value;
      m_value =
            round(
                static_cast<cross_float>(m_value * rhs.m_value)
                /
                static_cast<cross_float>(DecimalFactor<Prec>::value)
            );

      return *this;
    }

    const decimal operator/(const decimal &rhs) const {
        decimal result = *this;
        //result.m_value = (result.m_value * DecimalFactor<Prec>::value) / rhs.m_value;
        result.m_value =
            round(
                static_cast<cross_float>(result.m_value * DecimalFactor<Prec>::value)
                /
                static_cast<cross_float>(rhs.m_value)
            );

        return result;
    }

    decimal & operator/=(const decimal &rhs) {
      //m_value = (m_value * DecimalFactor<Prec>::value) / rhs.m_value;
      m_value =
            round(
                static_cast<cross_float>(m_value * DecimalFactor<Prec>::value)
                /
                static_cast<cross_float>(rhs.m_value)
            );

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
protected:
    inline xdouble getPrecFactorXDouble() const { return static_cast<xdouble>(DecimalFactor<Prec>::value); }
    inline double getPrecFactorDouble() const { return static_cast<double>(DecimalFactor<Prec>::value); }
    inline int getPrecFactorInt() const { return DecimalFactor<Prec>::value; }
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
    void initWithPrec(int64 value, int precFactor) {
        int ownFactor = DecimalFactor<Prec>::value;

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
    return decimal<Prec>(arg.getUnbiased(), arg.getPrec());
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

} // namespace
#endif // _DECIMAL_H__
