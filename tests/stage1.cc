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

#include <jf/unittest/case.h>
#include <jf/unittest/suite.h>
#include <jf/unittest/result.h>
#include <jf/unittest/visitor.h>
#include <jf/unittest/direct-runner.h>
#include <jf/unittest/walk.h>

#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace jf::unittest;

namespace
{

class OkTest : public TestCase
{
public:
    OkTest() : TestCase("OkTest") {}
    virtual void run()
    {
        JFUNIT_ASSERT(true);
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

class MyResult : public jf::unittest::Result
{
public:
    MyResult() : num_success_(0), num_failure_(0), num_error_(0) {}

    size_t num_success() const { return num_success_; }
    size_t num_failure() const { return num_failure_; }
    size_t num_error() const { return num_error_; }

    virtual void add_success(const jf::unittest::TestCase*) { num_success_++; }
    virtual void add_failure(const jf::unittest::TestCase*, const jf::unittest::Failure&) { num_failure_++; }
    virtual void add_error(const jf::unittest::TestCase*, const std::string& message) { num_error_++; }

private:
    int num_success_;
    int num_failure_;
    int num_error_;
};

class SuiteTest_Walk : public TestCase
{
public:
    SuiteTest_Walk() : TestCase("SuiteTest_Walk") {}
    virtual void run()
    {
        class MyVisitor : public Visitor
        {
        public:
            MyVisitor() : suites_entered_(0),
                          suites_left_(0),
                          tests_entered_(0),
                          tests_left_(0) {}
            size_t suites_entered() const { return suites_entered_; }
            size_t suites_left() const { return suites_left_; }
            size_t tests_entered() const { return tests_entered_; }
            size_t tests_left() const { return tests_left_; }
        private:
            virtual void enter_suite(const TestSuite*) { suites_entered_++; }
            virtual void leave_suite(const TestSuite*) { suites_left_++; }
            virtual void enter_test(const TestCase*) { tests_entered_++; }
            virtual void leave_test(const TestCase*) { tests_left_++; }
        private:
            size_t suites_entered_;
            size_t suites_left_;
            size_t tests_entered_;
            size_t tests_left_;
        };

        class MyResult : public Result
        {
        public:
            MyResult() : num_success_(0), num_failure_(0), num_error_(0) {}
            size_t num_success() const { return num_success_; }
            size_t num_failure() const { return num_failure_; }
            size_t num_error() const { return num_error_; }
        private:
            virtual void add_success(const TestCase*){ num_success_++; }
            virtual void add_failure(const TestCase*, const Failure&) { num_failure_++; }
            virtual void add_error(const TestCase*, const std::string& message) { num_error_++; }
        private:
            size_t num_success_;
            size_t num_failure_;
            size_t num_error_;            
        };


        TestTestSuite suite;
        MyVisitor visitor;
        MyResult result;        
        DirectRunner runner;

        walk(&suite, &visitor, &runner, &result);
        
        JFUNIT_ASSERT(result.num_success() == 2);
        JFUNIT_ASSERT(result.num_failure() == 2);
        JFUNIT_ASSERT(result.num_error() == 3);
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
        MyResult r;
        DirectRunner().run_test(&t, &r);
        BOOTSTRAP_ASSERT(r.num_success() == 1);
        BOOTSTRAP_ASSERT(r.num_failure() == 0);
        BOOTSTRAP_ASSERT(r.num_error() == 0);
    }
    {
        FailureTest t;
        MyResult r;
        DirectRunner().run_test(&t, &r);
        BOOTSTRAP_ASSERT(r.num_success() == 0);
        BOOTSTRAP_ASSERT(r.num_failure() == 1);
        BOOTSTRAP_ASSERT(r.num_error() == 0);
    }
    {
        ErrorTest t;
        MyResult r;
        DirectRunner().run_test(&t, &r);
        BOOTSTRAP_ASSERT(r.num_success() == 0);
        BOOTSTRAP_ASSERT(r.num_failure() == 0);
        BOOTSTRAP_ASSERT(r.num_error() == 1);
    }
    {
        SuiteTest_Walk t;
        MyResult r;
        DirectRunner().run_test(&t, &r);
        BOOTSTRAP_ASSERT(r.num_success() == 1);
    }

    return 0;
}
