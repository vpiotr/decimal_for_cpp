/////////////////////////////////////////////////////////////////////////////
// Name:        runner.cpp
// Purpose:     Boost test runner.
//              To compile tests just add all "*.cpp" files to one project.
// Author:      Piotr Likus
// Modified by:
// Created:     20/06/2010
// Licence:     BSD
/////////////////////////////////////////////////////////////////////////////

#include "Precomp.h"
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "C++ Unit Tests"
#include <boost/test/unit_test.hpp>
