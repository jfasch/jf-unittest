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

#include <jf/unittest/test_case.h>
#include <jf/unittest/test_result.h>

using namespace jf::unittest;

namespace
{

class MyOkTest : public TestCase
{
public:
    MyOkTest() : TestCase("MyOkTest") {}
    virtual void run()
    {
    }
};

class MyFailureTest : public TestCase
{
public:
    MyFailureTest() : TestCase("MyFailureTest") {}
    virtual void run()
    {
        JFUNIT_FAIL();
    }
};

class MyErrorTest : public TestCase
{
public:
    MyErrorTest() : TestCase("MyErrorTest") {}
    virtual void run()
    {
        throw 1;
    }
};

class MyTestResult : public TestResult
{
public:
    MyTestResult() : num_success_(0), num_failure_(0), num_error_(0) {}

    virtual void add_success() { num_success_++; }
    virtual void add_failure() { num_failure_++; }
    virtual void add_error() { num_error_++; }

    int num_success() const { return num_success_; }
    int num_failure() const { return num_failure_; }
    int num_error() const { return num_error_; }
private:
    int num_success_;
    int num_failure_;
    int num_error_;
};

class MySuiteTest : public TestCase
{
public:
    MySuiteTest() : TestCase("MySuiteTest") {}
    virtual void run()
    {
        
    }
private:
    class MyTestSuite : public TestSuite
    {
    public:
        MyTestSuite() : TestSuite("MyTestSuite")
        {
            add_test(new MyOkTest);
        }
    };
};

}

int main()
{
    {
        MyOkTest t;
        MyTestResult r;
        t.run_internal(&r);
        if (r.num_success() != 1)
            return 1;
        if (r.num_failure() != 0)
            return 1;
        if (r.num_error() != 0)
            return 1;
    }
    {
        MyFailureTest t;
        MyTestResult r;
        t.run_internal(&r);
        if (r.num_success() != 0)
            return 1;
        if (r.num_failure() != 1)
            return 1;
        if (r.num_error() != 0)
            return 1;
    }
    {
        MyErrorTest t;
        MyTestResult r;
        t.run_internal(&r);
        if (r.num_success() != 0)
            return 1;
        if (r.num_failure() != 0)
            return 1;
        if (r.num_error() != 1)
            return 1;
    }
    {
        MySuiteTest t;
        MyTestResult r;
        t.run_internal(&r);
        if (r.num_success() != 1)
            return 1;
        if (r.num_failure() != 0)
            return 1;
        if (r.num_error() != 0)
            return 1;
    }
    return 0;
}
