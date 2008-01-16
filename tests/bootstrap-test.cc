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
        JFUNIT_FAIL(false);
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
    MyTestResult() : num_ok_(0), num_failures_(0), num_errors_(0) {}
    int num_ok() const { return num_ok_; }
    int num_failures() const { return num_failures_; }
    int num_errors() const { return num_errors_; }
private:
    int num_ok_;
    int num_failures_;
    int num_errors_;
};

}

int main()
{
    {
        MyOkTest t;
        MyTestResult r;
        t.run_internal(&r);
        if (r.num_ok() != 1)
            return 1;
        if (r.num_failures() != 0)
            return 1;
        if (r.num_errors() != 0)
            return 1;
    }
    {
        MyFailureTest t;
        MyTestResult r;
        t.run_internal();
        if (r.num_ok() != 0)
            return 1;
        if (r.num_failures() != 1)
            return 1;
        if (r.num_errors() != 0)
            return 1;
    }
    {
        MyErrorTest t;
        MyTestResult r;
        t.run_internal();
        if (r.num_ok() != 0)
            return 1;
        if (r.num_failures() != 0)
            return 1;
        if (r.num_errors() != 1)
            return 1;
    }
    return 0;
}
