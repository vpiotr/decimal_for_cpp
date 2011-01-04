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

#endif DEC_EXTERNAL_ROUND

template <int Prec>
class decimal {
public:
    typedef dec_storage_t raw_data_t;

    decimal() { init(0); }
    decimal(const decimal &src) { init(src); }
    explicit decimal(uint value) { init(value); }
    explicit decimal(int value) { init(value); }
    explicit decimal(int64 value) { init(value); }
    explicit decimal(double value) { init(value); }
    explicit decimal(float value) { init(value); }

    ~decimal() {}

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
        result.m_value = (result.m_value * rhs.m_value) / DecimalFactor<Prec>::value;
        return result;              
    }

    decimal & operator*=(const decimal &rhs) {
      m_value = (m_value * rhs.m_value) / DecimalFactor<Prec>::value;
      return *this;
    }

    const decimal operator/(const decimal &rhs) const {
        decimal result = *this;     
        result.m_value = (result.m_value * DecimalFactor<Prec>::value) / rhs.m_value;            
        return result;              
    }

    decimal & operator/=(const decimal &rhs) {
      m_value = (m_value * DecimalFactor<Prec>::value) / rhs.m_value;
      return *this;
    }

    double getAsDouble() const { return static_cast<double>(m_value) / getPrecFactorDouble(); }

    void setAsDouble(double value)
    { 
       double nval = value * getPrecFactorDouble(); 
       m_value = round(nval); 
    }

    // returns integer value = real_value * (10 ^ precision)
    int64 getUnbiased() const { return m_value; }
    void setUnbiased(int64 value) { m_value = value; }

    decimal<Prec> abs() const { 
        if (m_value >= 0)
            return *this;
        else
            return (decimal<Prec>(0) - *this);
    }

protected:
    inline double getPrecFactorDouble() const { return static_cast<double>(DecimalFactor<Prec>::value); }
    inline int getPrecFactorInt() const { return DecimalFactor<Prec>::value; }
    void init(const decimal &src) { m_value = src.m_value; }
    void init(uint value) { m_value = DecimalFactor<Prec>::value * static_cast<int>(value); }
    void init(int value) { m_value = DecimalFactor<Prec>::value * value; }
    void init(int64 value) { m_value = DecimalFactor<Prec>::value * value; }
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

protected:
    dec_storage_t m_value;
};

// ----------------------------------------------------------------------------
// Pre-defined types
// ----------------------------------------------------------------------------
typedef decimal<2> decimal2;
typedef decimal<4> decimal4;
typedef decimal<6> decimal6;

} // namespace
#endif // _DECIMAL_H__