// -*- C++ -*-

// Copyright (C) 2008-2011 Joerg Faschingbauer

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

#include <jf/unittest/test_case.h>
#include <jf/unittest/test_suite.h>
#include <jf/unittest/test_result.h>

#include <iostream>
#include <cstdlib>

using namespace jf::unittest;

namespace
{

class OkTest : public TestCase
{
public:
    OkTest() : TestCase("OkTest") {}
    virtual void run()
    {
    }
};

class FailureTest : public TestCase
{
public:
    FailureTest() : TestCase("FailureTest") {}
    virtual void run()
    {
        JFUNIT_FAIL();
    }
};

class ErrorTest : public TestCase
{
public:
    ErrorTest() : TestCase("ErrorTest") {}
    virtual void run()
    {
        throw 1;
    }
};

class BootstrapTestResult : public TestResult
{
public:
    BootstrapTestResult() : num_success_(0), num_failure_(0), num_error_(0) {}

    virtual void enter_suite(const TestSuite*) {}
    virtual void leave_suite(const TestSuite*) {}
    virtual void enter_test(const TestCase*) {}
    virtual void leave_test(const TestCase*) {}
    virtual void add_success(const TestCase*) { num_success_++; }
    virtual void add_failure(const TestCase*, const Failure&) { num_failure_++; }
    virtual void add_error(const TestCase*, const std::string&) { num_error_++; }
    virtual void add_assertion(const TestCase*) {}
    virtual void unclean_alarm(const TestCase*) {}

    int num_success() const { return num_success_; }
    int num_failure() const { return num_failure_; }
    int num_error() const { return num_error_; }
private:
    int num_success_;
    int num_failure_;
    int num_error_;
};

class SuiteTest : public TestCase
{
public:
    SuiteTest() : TestCase("SuiteTest") {}
    virtual void run()
    {
        TestTestSuite s;
        BootstrapTestResult r;
        s.run_internal(&r);
        JFUNIT_ASSERT(r.num_success() == 2);
        JFUNIT_ASSERT(r.num_failure() == 2);
        JFUNIT_ASSERT(r.num_error() == 3);
    }
private:
    class TestTestSuite : public TestSuite
    {
    public:
        TestTestSuite() : TestSuite("TestTestSuite")
        {
            add_test(std::auto_ptr<Test>(new OkTest));
            add_test(std::auto_ptr<Test>(new OkTest));
            add_test(std::auto_ptr<Test>(new FailureTest));
            add_test(std::auto_ptr<Test>(new FailureTest));
            add_test(std::auto_ptr<Test>(new ErrorTest));
            add_test(std::auto_ptr<Test>(new ErrorTest));
            add_test(std::auto_ptr<Test>(new ErrorTest));
        }
    };
};

}

#define BOOTSTRAP_ASSERT(condition) \
     if (!(condition)) { \
         std::cerr << "JF BOOTSTRAP FAILED: \"" << #condition << \
             "\" (" << __FILE__<< ':' << __LINE__ << ')' << std::endl; \
         std::exit(1); \
     }

int main()
{
    {
        OkTest t;
        BootstrapTestResult r;
        t.run_internal(&r);
        BOOTSTRAP_ASSERT(r.num_success() == 1);
        BOOTSTRAP_ASSERT(r.num_failure() == 0);
        BOOTSTRAP_ASSERT(r.num_error() == 0);
    }
    {
        FailureTest t;
        BootstrapTestResult r;
        t.run_internal(&r);
        BOOTSTRAP_ASSERT(r.num_success() == 0);
        BOOTSTRAP_ASSERT(r.num_failure() == 1);
        BOOTSTRAP_ASSERT(r.num_error() == 0);
    }
    {
        ErrorTest t;
        BootstrapTestResult r;
        t.run_internal(&r);
        BOOTSTRAP_ASSERT(r.num_success() == 0);
        BOOTSTRAP_ASSERT(r.num_failure() == 0);
        BOOTSTRAP_ASSERT(r.num_error() == 1);
    }
    {
        SuiteTest t;
        BootstrapTestResult r;
        t.run_internal(&r);
        BOOTSTRAP_ASSERT(r.num_success() == 1);
    }

    return 0;
}
