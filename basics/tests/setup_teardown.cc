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

#include "setup_teardown.h"

#include <jf/unittest/test_case.h>
#include <jf/unittest/test_result.h>

namespace {

class MyTest : public jf::unittest::TestCase
{
public:
    MyTest() : jf::unittest::TestCase(""),
               setup_called_(false),
               run_called_(false),
               teardown_called_(false) {}

    bool setup_called() const { return setup_called_; }
    bool run_called() const { return run_called_; }
    bool teardown_called() const { return teardown_called_; }

    virtual void setup() { setup_called_ = true; }
    virtual void run() { run_called_ = true; }
    virtual void teardown() { teardown_called_ = true; }

private:
    bool setup_called_;
    bool run_called_;
    bool teardown_called_;
};

class MyTestResult : public jf::unittest::TestResult
{
public:
    MyTestResult() : num_success_(0), num_failure_(0), num_error_(0) {}
    int num_success() const { return num_success_; }
    int num_failure() const { return num_failure_; }
    int num_error() const { return num_error_; }
    virtual void add_success(const jf::unittest::TestCase*) { num_success_++; }
    virtual void add_failure(const jf::unittest::TestCase*, const jf::unittest::Failure&) { num_failure_++; }
    virtual void add_error(const jf::unittest::TestCase*, const std::string&) { num_error_++; }
private:
    int num_success_;
    int num_failure_;
    int num_error_;
};

}

namespace jf {
namespace unittest {
namespace tests {

class SetupTeardownSuccess : public jf::unittest::TestCase
{
public:
    SetupTeardownSuccess() : jf::unittest::TestCase("jf::unittest::tests::SetupTeardownSuccess") {}
    virtual void run()
    {
        SuccessTest test;
        MyTestResult result;
        test.run_internal(&result);
        JFUNIT_ASSERT(test.setup_called());
        JFUNIT_ASSERT(test.run_called());
        JFUNIT_ASSERT(test.teardown_called());
        JFUNIT_ASSERT(result.num_success() == 1);
        JFUNIT_ASSERT(result.num_failure() == 0);
        JFUNIT_ASSERT(result.num_error() == 0);
    }
private:
    class SuccessTest : public MyTest {};
};

class SetupTeardownSetupFailure : public jf::unittest::TestCase
{
public:
    SetupTeardownSetupFailure() : jf::unittest::TestCase("jf::unittest::tests::SetupTeardownSetupFailure") {}
    virtual void run()
    {
        SetupFailureTest test;
        MyTestResult result;
        test.run_internal(&result);
        JFUNIT_ASSERT(test.setup_called());
        JFUNIT_ASSERT(!test.run_called());
        JFUNIT_ASSERT(!test.teardown_called());
        JFUNIT_ASSERT(result.num_success() == 0);
        JFUNIT_ASSERT(result.num_failure() == 0);
        JFUNIT_ASSERT(result.num_error() == 1);
    }
private:
    class SetupFailureTest : public MyTest
    {
    public:
        virtual void setup()
        {
            MyTest::setup();
            JFUNIT_FAIL();
        }
    };
};

class SetupTeardownSetupError : public jf::unittest::TestCase
{
public:
    SetupTeardownSetupError() : jf::unittest::TestCase("jf::unittest::tests::SetupTeardownSetupError") {}
    virtual void run()
    {
        SetupErrorTest test;
        MyTestResult result;
        test.run_internal(&result);
        JFUNIT_ASSERT(test.setup_called());
        JFUNIT_ASSERT(!test.run_called());
        JFUNIT_ASSERT(!test.teardown_called());
        JFUNIT_ASSERT(result.num_success() == 0);
        JFUNIT_ASSERT(result.num_failure() == 0);
        JFUNIT_ASSERT(result.num_error() == 1);
    }
private:
    class SetupErrorTest : public MyTest
    {
    public:
        virtual void setup()
        {
            MyTest::setup();
            throw 1;
        }
    };
};

class SetupTeardownRunFailure : public jf::unittest::TestCase
{
public:
    SetupTeardownRunFailure() : jf::unittest::TestCase("jf::unittest::tests::SetupTeardownRunFailure") {}
    virtual void run()
    {
        RunFailureTest test;
        MyTestResult result;
        test.run_internal(&result);
        JFUNIT_ASSERT(test.setup_called());
        JFUNIT_ASSERT(test.run_called());
        JFUNIT_ASSERT(test.teardown_called());
        JFUNIT_ASSERT(result.num_success() == 0);
        JFUNIT_ASSERT(result.num_failure() == 1);
        JFUNIT_ASSERT(result.num_error() == 0);
    }
private:
    class RunFailureTest : public MyTest
    {
    public:
        virtual void run()
        {
            MyTest::run();
            JFUNIT_FAIL();
        }
    };
};

class SetupTeardownRunError : public jf::unittest::TestCase
{
public:
    SetupTeardownRunError() : jf::unittest::TestCase("jf::unittest::tests::SetupTeardownRunError") {}
    virtual void run()
    {
        RunErrorTest test;
        MyTestResult result;
        test.run_internal(&result);
        JFUNIT_ASSERT(test.setup_called());
        JFUNIT_ASSERT(test.run_called());
        JFUNIT_ASSERT(test.teardown_called());
        JFUNIT_ASSERT(result.num_success() == 0);
        JFUNIT_ASSERT(result.num_failure() == 0);
        JFUNIT_ASSERT(result.num_error() == 1);
    }
private:
    class RunErrorTest : public MyTest
    {
    public:
        virtual void run()
        {
            MyTest::run();
            throw 1;
        }
    };
};

class SetupTeardownTeardownFailure : public jf::unittest::TestCase
{
public:
    SetupTeardownTeardownFailure() : jf::unittest::TestCase("jf::unittest::tests::SetupTeardownTeardownFailure") {}
    virtual void run()
    {
        TeardownFailureTest test;
        MyTestResult result;
        test.run_internal(&result);
        JFUNIT_ASSERT(test.setup_called());
        JFUNIT_ASSERT(test.run_called());
        JFUNIT_ASSERT(test.teardown_called());
        JFUNIT_ASSERT(result.num_success() == 1);
        JFUNIT_ASSERT(result.num_failure() == 0);
        JFUNIT_ASSERT(result.num_error() == 1);
    }
private:
    class TeardownFailureTest : public MyTest
    {
    public:
        virtual void teardown()
        {
            MyTest::teardown();
            JFUNIT_FAIL();
        }
    };
};

class SetupTeardownTeardownError : public jf::unittest::TestCase
{
public:
    SetupTeardownTeardownError() : jf::unittest::TestCase("jf::unittest::tests::SetupTeardownTeardownError") {}
    virtual void run()
    {
        TeardownErrorTest test;
        MyTestResult result;
        test.run_internal(&result);
        JFUNIT_ASSERT(test.setup_called());
        JFUNIT_ASSERT(test.run_called());
        JFUNIT_ASSERT(test.teardown_called());
        JFUNIT_ASSERT(result.num_success() == 1);
        JFUNIT_ASSERT(result.num_failure() == 0);
        JFUNIT_ASSERT(result.num_error() == 1);
    }
private:
    class TeardownErrorTest : public MyTest
    {
    public:
        virtual void teardown()
        {
            MyTest::teardown();
            throw 1;
        }
    };
};

SetupTeardownSuite::SetupTeardownSuite()
: jf::unittest::TestSuite("jf::unittest::tests::SetupTeardownSuite")
{
    add_test(new SetupTeardownSuccess);
    add_test(new SetupTeardownSetupFailure);
    add_test(new SetupTeardownSetupError);
    add_test(new SetupTeardownRunFailure);
    add_test(new SetupTeardownRunError);
    add_test(new SetupTeardownTeardownFailure);
    add_test(new SetupTeardownTeardownError);
}

}
}
}
