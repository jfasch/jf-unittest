// -*- C++ -*-

// Copyright (C) 2008-2012 Joerg Faschingbauer

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

#include "assert-suite.h"

#include <jf/unittest/case.h>
#include <jf/unittest/direct-runner.h>

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

class MyTestResult : public jf::unittest::Result
{
public:
    MyTestResult() : num_success_(0), num_failure_(0), num_error_(0) {}

    bool ok() const { return num_success_ > 0 && num_failure_ == 0 && num_error_ == 0; }

    virtual void add_success(const jf::unittest::TestCase*) { num_success_++; }
    virtual void add_failure(const jf::unittest::TestCase*, const jf::unittest::Failure&) { num_failure_++; }
    virtual void add_error(const jf::unittest::TestCase*, const std::string& message) { num_error_++; }

    int num_success() const { return num_success_; }
    int num_failure() const { return num_failure_; }
    int num_error() const { return num_error_; }

private:
    int num_success_;
    int num_failure_;
    int num_error_;
};

}

namespace jf {
namespace unittest {
namespace tests {

class AssertThrows : public jf::unittest::TestCase
{
public:
    AssertThrows() : jf::unittest::TestCase("AssertThrows") {}
    virtual void run()
    {
        {
            ThrowTest t;
            MyTestResult result;
            DirectRunner().run_test(&t, &result);
            JFUNIT_ASSERT(result.ok());
        }
        {
            NoThrowTest t;
            MyTestResult result;
            DirectRunner().run_test(&t, &result);
            JFUNIT_ASSERT(result.num_failure() == 1);
        }
        {
            WrongThrowTest t;
            MyTestResult result;
            DirectRunner().run_test(&t, &result);
            JFUNIT_ASSERT(result.num_failure() == 1);
        }
    }
};

Assert::Assert()
: jf::unittest::TestSuite("Assert")
{
    add_test(std::auto_ptr<Test>(new AssertThrows));
}

}
}
}
