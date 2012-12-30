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

#include "setup-teardown.h"

#include <jf/unittest/case.h>
#include <jf/unittest/direct-runner.h>

#include <cassert>

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

class MyResult : public jf::unittest::Result
{
public:
    MyResult() : num_success_(0),
                 num_failure_(0),
                 num_error_(0) {}

    int num_success() const { return num_success_; }
    int num_failure() const { return num_failure_; }
    int num_error() const { return num_error_; }

    virtual void add_success(const jf::unittest::TestCase*) { num_success_++; }
    virtual void add_failure(const jf::unittest::TestCase*, const jf::unittest::Failure&) { num_failure_++; }
    virtual void add_error(const jf::unittest::TestCase*, const std::string&) { num_error_++; }
    virtual void add_additional_info(const jf::unittest::TestCase*, const std::string&) { assert(false); }
    
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
    SetupTeardownSuccess() : jf::unittest::TestCase("SetupTeardownSuccess") {}
    virtual void run()
    {
        SuccessTest test;
        MyResult result;
        DirectRunner().run_test(&test, &result);

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
    SetupTeardownSetupFailure() : jf::unittest::TestCase("SetupTeardownSetupFailure") {}
    virtual void run()
    {
        SetupFailureTest test;
        MyResult result;
        DirectRunner().run_test(&test, &result);

        JFUNIT_ASSERT(test.setup_called());
        JFUNIT_ASSERT(!test.run_called());
        JFUNIT_ASSERT(!test.teardown_called());
        JFUNIT_ASSERT(result.num_success() == 0);

        // a setup failure is both a regular failure, as well as a
        // more heavyweight error.
        JFUNIT_ASSERT(result.num_failure() == 1);
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
    SetupTeardownSetupError() : jf::unittest::TestCase("SetupTeardownSetupError") {}
    virtual void run()
    {
        SetupErrorTest test;
        MyResult result;
        DirectRunner().run_test(&test, &result);

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
    SetupTeardownRunFailure() : jf::unittest::TestCase("SetupTeardownRunFailure") {}
    virtual void run()
    {
        RunFailureTest test;
        MyResult result;
        DirectRunner().run_test(&test, &result);

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
    SetupTeardownRunError() : jf::unittest::TestCase("SetupTeardownRunError") {}
    virtual void run()
    {
        RunErrorTest test;
        MyResult result;
        DirectRunner().run_test(&test, &result);
        
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
    SetupTeardownTeardownFailure() : jf::unittest::TestCase("SetupTeardownTeardownFailure") {}
    virtual void run()
    {
        TeardownFailureTest test;
        MyResult result;
        DirectRunner().run_test(&test, &result);

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
    SetupTeardownTeardownError() : jf::unittest::TestCase("SetupTeardownTeardownError") {}
    virtual void run()
    {
        TeardownErrorTest test;
        MyResult result;
        DirectRunner().run_test(&test, &result);

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

SetupTeardown::SetupTeardown()
: jf::unittest::TestSuite("SetupTeardown")
{
    add_test(std::auto_ptr<Test>(new SetupTeardownSuccess));
    add_test(std::auto_ptr<Test>(new SetupTeardownSetupFailure));
    add_test(std::auto_ptr<Test>(new SetupTeardownSetupError));
    add_test(std::auto_ptr<Test>(new SetupTeardownRunFailure));
    add_test(std::auto_ptr<Test>(new SetupTeardownRunError));
    add_test(std::auto_ptr<Test>(new SetupTeardownTeardownFailure));
    add_test(std::auto_ptr<Test>(new SetupTeardownTeardownError));
}

}
}
}
