// -*- C++ -*-

// Copyright (C) 2008 Joerg Faschingbauer

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2.1 of
// the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA

#include"assert_suite.h"

#include <jf/unittest/test_case.h>
#include <jf/unittest/simple_test_result.h>

namespace {

class TestException {};

void throw_testexception()
{
    throw TestException();
}

void no_throw_testexception() {}

class TestExceptionPositive : public jf::unittest::TestCase
{
public:
    TestExceptionPositive() : jf::unittest::TestCase("") {}
    virtual void run()
    {
        JFUNIT_ASSERT_THROWS(TestException, throw_testexception());
    }
};

class TestExceptionNegative : public jf::unittest::TestCase
{
public:
    TestExceptionNegative() : jf::unittest::TestCase("") {}
    virtual void run()
    {
        JFUNIT_ASSERT_THROWS(TestException, no_throw_testexception());
    }
};

}

namespace jf {
namespace unittest {
namespace tests {

class AssertThrowsTest : public jf::unittest::TestCase
{
public:
    AssertThrowsTest() : jf::unittest::TestCase("jf::unittest::tests::AssertThrowsTest") {}
    virtual void run()
    {
        {
            TestExceptionPositive pos;
            SimpleTestResult result;
            pos.run_internal(&result, NULL);
            JFUNIT_ASSERT(result.ok());
        }
        {
            TestExceptionNegative neg;
            SimpleTestResult result;
            neg.run_internal(&result, NULL);
            JFUNIT_ASSERT(result.num_failure() == 1);
        }
    }
};

AssertSuite::AssertSuite()
: jf::unittest::TestSuite("jf::unittest::tests::AssertSuite")
{
    add_test(new AssertThrowsTest);
}

}
}
}
