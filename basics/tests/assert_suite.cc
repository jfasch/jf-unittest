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
class UnknownException {};

void throw_testexception()
{
    throw TestException();
}

void no_throw_testexception() {}

// throw the exception we require.
class ThrowTest : public jf::unittest::TestCase
{
public:
    ThrowTest() : jf::unittest::TestCase("") {}
    virtual void run()
    {
        JFUNIT_ASSERT_THROWS(TestException, throw_testexception());
    }
};

// don't throw an exception though one is required.
class NoThrowTest : public jf::unittest::TestCase
{
public:
    NoThrowTest() : jf::unittest::TestCase("") {}
    virtual void run()
    {
        JFUNIT_ASSERT_THROWS(TestException, no_throw_testexception());
    }
};

// throw a different exception than the one that we require.
class WrongThrowTest : public jf::unittest::TestCase
{
public:
    WrongThrowTest() : jf::unittest::TestCase("") {}
    virtual void run()
    {
        JFUNIT_ASSERT_THROWS(UnknownException, throw_testexception());
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
            ThrowTest t;
            SimpleTestResult result;
            t.run_internal(&result, NULL);
            JFUNIT_ASSERT(result.ok());
        }
        {
            NoThrowTest t;
            SimpleTestResult result;
            t.run_internal(&result, NULL);
            JFUNIT_ASSERT(result.num_failure() == 1);
        }
        {
            WrongThrowTest t;
            SimpleTestResult result;
            t.run_internal(&result, NULL);
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
