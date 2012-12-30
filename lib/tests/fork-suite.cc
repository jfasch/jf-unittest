// -*- C++ -*-

// Copyright (C) 2012 Joerg Faschingbauer

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

#include "fork-suite.h"

#include <jf/unittest/case.h>
#include <jf/unittest/result.h>
#include <jf/unittest/fork-runner.h>

#include <cassert>
#include <unistd.h>

namespace {

using namespace jf::unittest;

class MyTestResult : public jf::unittest::Result
{
public:
    MyTestResult() : num_success_(0), num_error_(0) {}

    size_t num_success() const { return num_success_; }
    size_t num_error() const { return num_error_; }
    const std::vector<Failure>& failures() const { return failures_; }
    const std::vector<std::string>& additional_info() const { return additional_info_; }
    
    virtual void add_success(const TestCase*) { num_success_++; }
    virtual void add_failure(const TestCase*, const Failure& f) { failures_.push_back(f); }
    virtual void add_error(const TestCase*, const std::string& message) { num_error_++; }
    virtual void add_additional_info(const TestCase*, const std::string& info) { additional_info_.push_back(info); }

private:
    size_t num_success_;
    size_t num_error_;
    std::vector<Failure> failures_;
    std::vector<std::string> additional_info_;    
};

class ForkOkTest : public TestCase
{
public:
    ForkOkTest() : TestCase("ForkOk") {}

    virtual void run()
    {
        class OkTest : public TestCase
        {
        public:
            OkTest() : TestCase("") {}
            virtual void run() {}
        };
        
        OkTest test;
        ForkRunner runner;
        MyTestResult result;
        runner.run_test(&test, &result);
        
        JFUNIT_ASSERT(result.num_success() == 1);
    }
};

class ForkFailureTest : public TestCase
{
public:
    ForkFailureTest() : TestCase("ForkFailure") {}

    virtual void run()
    {
        class FailTest : public TestCase
        {
        public:
            FailTest() : TestCase("") {}
            virtual void run() { JFUNIT_FAIL(); }
        };
        
        FailTest test;
        ForkRunner runner;
        MyTestResult result;
        runner.run_test(&test, &result);
        
        JFUNIT_ASSERT(result.failures().size() == 1);
        JFUNIT_ASSERT(result.failures()[0].filename() == __FILE__);
    }
};

class ForkDontCrashTest : public TestCase
{
public:
    ForkDontCrashTest() : TestCase("ForkDontCrash") {}

    virtual void run()
    {
        class BadErrorTest : public TestCase
        {
        public:
            BadErrorTest() : TestCase("") {}
            virtual void run() { _exit(42); }
        };
        
        BadErrorTest test;
        ForkRunner runner;
        MyTestResult result;
        runner.run_test(&test, &result);
        
        JFUNIT_ASSERT(result.num_error() == 1);
    }
};

class ForkErrorTest : public TestCase
{
public:
    ForkErrorTest() : TestCase("ForkError") {}

    virtual void run()
    {
        class ErrorTest : public TestCase
        {
        public:
            ErrorTest() : TestCase("") {}
            virtual void run() { throw std::exception(); }
        };
        
        ErrorTest test;
        ForkRunner runner;
        MyTestResult result;
        runner.run_test(&test, &result);
        
        JFUNIT_ASSERT(result.num_error() == 1);
    }
};

class AdditionalInfoTest : public TestCase
{
public:
    AdditionalInfoTest() : TestCase("AdditionalInfo") {}

    virtual void run()
    {
        class OkTest : public TestCase
        {
        public:
            OkTest() : TestCase("") {}
            virtual void run() {}
        };
        
        OkTest test;
        ForkRunner runner(/*print_pid=*/true);
        MyTestResult result;
        runner.run_test(&test, &result);

        // additional_info contains one element, the PID.
        JFUNIT_ASSERT(result.additional_info().size() == 1);
    }
};

class SetupFailureTest : public TestCase
{
public:
    SetupFailureTest() : TestCase("SetupFailure") {}

    virtual void run()
    {
        class SetupFails : public TestCase
        {
        public:
            SetupFails() : TestCase("") {}
            virtual void setup() { JFUNIT_FAIL(); }
            virtual void run() {}
        };
        
        SetupFails test;
        ForkRunner runner(/*print_pid=*/true);
        MyTestResult result;
        runner.run_test(&test, &result);

        JFUNIT_ASSERT(result.num_error() == 1);
        JFUNIT_ASSERT(result.failures().size() == 1);
    }
};

}

namespace jf {
namespace unittest {
namespace tests {

Fork::Fork()
: jf::unittest::TestSuite("Fork")
{
    add_test(std::auto_ptr<Test>(new ForkOkTest));
    add_test(std::auto_ptr<Test>(new ForkDontCrashTest));
    add_test(std::auto_ptr<Test>(new ForkErrorTest));
    add_test(std::auto_ptr<Test>(new AdditionalInfoTest));
    add_test(std::auto_ptr<Test>(new SetupFailureTest));
}

}
}
}
