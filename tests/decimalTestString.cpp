//
// Created by piotr on 7/19/21.
//

#include "decimal.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(decimalString)
{
        using namespace dec;
        decimal<4> d(4.1234);
        std::string dTxt = toString(d);
        BOOST_CHECK(d == fromString<decimal<4> >(dTxt));

        // fromString converts with rounding
        BOOST_CHECK(d == decimal<4>().pack(4,1234));
        BOOST_CHECK(d == fromString<decimal<4> >("4.1234"));
        BOOST_CHECK(d == fromString<decimal<4> >("4.12341"));
        BOOST_CHECK(d != fromString<decimal<4> >("4.12345"));
        BOOST_CHECK(decimal<5>().pack(4,12341) == fromString<decimal<5> >("4.12341"));

        // test negative values
        d = d * decimal_cast<4>(-1);
        BOOST_CHECK(d < decimal_cast<4>(0));

        std::string sneg = toString(d);
        BOOST_TEST_MESSAGE("sneg: " << sneg);
        BOOST_TEST_MESSAGE("d: " << d);
        BOOST_TEST_MESSAGE("sneg-to-decimal: " << (fromString<decimal<4> >(sneg)));
        BOOST_CHECK(d == fromString<decimal<4> >(sneg));
}

BOOST_AUTO_TEST_CASE(decimalToString) {

        std::string expected1 = "3.1549999999999998";
        dec::decimal<16> d1(expected1);

        BOOST_CHECK_EQUAL(dec::toString(d1), expected1);

        std::string expected2 = "315499999999999.98";
        dec::decimal<2> d2(expected2);

        BOOST_CHECK_EQUAL(dec::toString(d2), expected2);
}

BOOST_AUTO_TEST_CASE(decimalFromString) {

        std::string value1 = "3.1549999999999998";
        dec::decimal<16> d1a (3.1549999999999998);
        dec::decimal<16> d1b(value1);

        BOOST_CHECK_EQUAL(d1a, d1b);

        std::string value2 = "31549999999999.98";
        dec::decimal<2> d2a  (31549999999999.98);
        dec::decimal<2> d2b(value2);

        BOOST_CHECK_EQUAL(d2a, d2b);
}

BOOST_AUTO_TEST_CASE(decimalToStringWithFormat) {

        dec::decimal_format format(',', '.');

        std::string value1    = "3.1549999999999998";
        std::string expected1 = "3,1549999999999998";
        dec::decimal<16> d1(value1);

        BOOST_CHECK_EQUAL(dec::toString(d1, format), expected1);

        std::string value2    = "315499999999999.98";
        std::string expected2 = "315.499.999.999.999,98";
        dec::decimal<2> d2(value2);

        BOOST_CHECK_EQUAL(dec::toString(d2, format), expected2);
}

BOOST_AUTO_TEST_CASE(decimalFromStringWithFormat) {

        dec::decimal_format format(',', '.');

        const std::string value1 = "3,1549999999999998";
        dec::decimal<16> d1a (3.1549999999999998);
        dec::decimal<16> d1b(dec::fromString<dec::decimal<16> >(value1, format));

        BOOST_CHECK_EQUAL(d1a, d1b);

        const std::string value2 = "31.549.999.999.999,98";
        dec::decimal<2> d2a  (31549999999999.98);
        dec::decimal<2> d2b(dec::fromString<dec::decimal<2> >(value2, format));

        BOOST_CHECK_EQUAL(d2a, d2b);
}

struct italiano_separators : std::numpunct<char> {
    char do_thousands_sep()   const { return '.'; }  // separate with dot
    std::string do_grouping() const { return "\3"; } // groups of 3 digits
    char do_decimal_point()   const { return ','; }  // separate with comma
};

BOOST_AUTO_TEST_CASE(decimalItalianoParsing) {
    std::locale new_locale(std::cout.getloc(), new italiano_separators);
    std::locale prior_locale = std::locale::global(new_locale);
    std::stringstream ss("1.234,56");
    dec::decimal<2> ret;
    dec::fromStream(ss, ret);
    std::locale::global(prior_locale);
    dec::DEC_INT64 beforeValue, afterValue;
    ret.unpack(beforeValue, afterValue);
    BOOST_CHECK_EQUAL(beforeValue, 1234);
    BOOST_CHECK_EQUAL(afterValue, 56);
}

BOOST_AUTO_TEST_CASE(decimalToStringWithGlobalCLocale) {
    std::locale prior_cout_locale = std::cout.getloc();
    std::locale prior_locale = std::locale::global(std::locale("C"));

    dec::decimal<7> value = dec::decimal<7>::buildWithExponent(1234567890987L, -7);
    std::string expected = "123456.7890987";

    BOOST_CHECK_EQUAL(dec::toString(value), expected);

    std::cout.imbue( prior_cout_locale );
    std::locale::global(prior_locale);
}

BOOST_AUTO_TEST_CASE(decimalToStringWithGlobalItLocale) {
    std::locale prior_cout_locale = std::cout.getloc();
    std::locale italiano_locale(std::cout.getloc(), new italiano_separators);
    std::locale prior_locale = std::locale::global(italiano_locale);

    dec::decimal<7> value = dec::decimal<7>::buildWithExponent(1234567890987L, -7);
    std::string expected = "123.456,7890987";

    BOOST_CHECK_EQUAL(dec::toString(value), expected);

    std::cout.imbue(prior_cout_locale);
    std::locale::global(prior_locale);
}

BOOST_AUTO_TEST_CASE(decimalToStringWithGlobalItLocaleAndProvidedFormat) {
    std::locale prior_cout_locale = std::cout.getloc();
    std::locale italiano_locale(std::cout.getloc(), new italiano_separators);
    std::locale prior_locale = std::locale::global(italiano_locale);

    dec::decimal_format format('.', '\0');

    dec::decimal<7> value = dec::decimal<7>::buildWithExponent(1234567890987L, -7);
    std::string expected = "123456.7890987";

    BOOST_CHECK_EQUAL(dec::toString(value, format), expected);

    std::cout.imbue(prior_cout_locale);
    std::locale::global(prior_locale);
}

BOOST_AUTO_TEST_CASE(decimalFromStringWithGlobalCLocale) {
    std::locale prior_cout_locale = std::cout.getloc();
    std::locale prior_locale = std::locale::global(std::locale("C"));

    std::string value = "123456.7890987";
    dec::decimal<7> expected = dec::decimal<7>::buildWithExponent(1234567890987L, -7);
    dec::decimal<7> d7(value);

    BOOST_CHECK_EQUAL(d7, expected);

    std::cout.imbue(prior_cout_locale);
    std::locale::global(prior_locale);
}

BOOST_AUTO_TEST_CASE(decimalFromStringWithGlobalItLocale) {
    std::locale prior_cout_locale = std::cout.getloc();
    std::locale italiano_locale(std::cout.getloc(), new italiano_separators);
    std::locale prior_locale = std::locale::global(italiano_locale);

    std::string value = "123.456,7890987";
    dec::decimal<7> expected = dec::decimal<7>::buildWithExponent(1234567890987L, -7);
    dec::decimal<7> d7(value);

    BOOST_CHECK_EQUAL(d7, expected);

    std::cout.imbue(prior_cout_locale);
    std::locale::global(prior_locale);
}

BOOST_AUTO_TEST_CASE(decimalFromStringWithGlobalItLocaleAndProvidedFormat) {
    std::locale prior_cout_locale = std::cout.getloc();
    std::locale italiano_locale(std::cout.getloc(), new italiano_separators);
    std::locale prior_locale = std::locale::global(italiano_locale);

    dec::decimal_format cformat('.', '\0');

    std::string value = "123456.7890987";
    dec::decimal<7> expected = dec::decimal<7>::buildWithExponent(1234567890987L, -7);
    dec::decimal<7> d7(value, cformat);

    BOOST_CHECK_EQUAL(d7, expected);

    std::cout.imbue(prior_cout_locale);
    std::locale::global(prior_locale);
}