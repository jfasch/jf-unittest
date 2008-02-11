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

#include "enter_leave.h"

#include <jf/unittest/test_case.h>
#include <jf/unittest/test_suite.h>
#include <jf/unittest/test_result.h>

#include <cassert>

namespace {

class MyTestCase : public jf::unittest::TestCase
{
public:
    enum State {
      S_INIT,
      S_RUNNING,
      S_DONE
    };

public:
    MyTestCase(TestCase* caller)
    : jf::unittest::TestCase(""),
      state_(S_INIT),
      caller_(caller) {}

    State state() const { return state_; }

    virtual void setup()
    {
        switch (state_) {
            case S_INIT:
                state_ = S_RUNNING;
                break;
            case S_RUNNING:
                JFUNIT_OBJECT_FAIL(caller_);
                break;
            case S_DONE:
                JFUNIT_OBJECT_FAIL(caller_);
                break;
        }
    }

    virtual void run() {}
    
    virtual void teardown()
    {
        switch (state_) {
            case S_INIT:
                JFUNIT_OBJECT_FAIL(caller_);
                break;
            case S_DONE:
                JFUNIT_OBJECT_FAIL(caller_);
                break;
            case S_RUNNING:
                state_ = S_DONE;
                break;
        }
    }
    
private:
    State state_;
    TestCase* caller_;
};

class MyTestResult : public jf::unittest::TestResult
{
public:
    MyTestResult(jf::unittest::TestCase* caller)
    : caller_(caller),
      num_suites_entered_(0),
      num_suites_left_(0),
      num_tests_entered_(0),
      num_tests_left_(0) {}

    int num_suites_entered() const { return num_suites_entered_; }
    int num_suites_left() const { return num_suites_left_; }
    int num_tests_entered() const { return num_tests_entered_; }
    int num_tests_left() const { return num_tests_left_; }

    virtual void enter_suite(const jf::unittest::TestSuite* s)
    {
        for (jf::unittest::TestSuite::Tests::const_iterator i = s->tests().begin();
             i != s->tests().end();
             ++i) {
            const MyTestCase* mt = dynamic_cast<const MyTestCase*>(*i);
            assert(mt);
            switch (mt->state()) {
                case MyTestCase::S_INIT:
                    break;
                case MyTestCase::S_RUNNING:
                    JFUNIT_OBJECT_FAIL(caller_);
                    break;
                case MyTestCase::S_DONE:
                    JFUNIT_OBJECT_FAIL(caller_);
                    break;
            }
        }
        num_suites_entered_++;
    }

    virtual void leave_suite(const jf::unittest::TestSuite* s)
    {
        for (jf::unittest::TestSuite::Tests::const_iterator i = s->tests().begin();
             i != s->tests().end();
             ++i) {
            const MyTestCase* mt = dynamic_cast<const MyTestCase*>(*i);
            assert(mt);
            switch (mt->state()) {
                case MyTestCase::S_INIT:
                    JFUNIT_OBJECT_FAIL(caller_);
                    break;
                case MyTestCase::S_RUNNING:
                    JFUNIT_OBJECT_FAIL(caller_);
                    break;
                case MyTestCase::S_DONE:
                    break;
            }
        }
        num_suites_left_++;
    }

    virtual void enter_test(const jf::unittest::TestCase* t)
    {
        const MyTestCase* mt = dynamic_cast<const MyTestCase*>(t);
        assert(mt);
        switch (mt->state()) {
            case MyTestCase::S_INIT:
                break;
            case MyTestCase::S_RUNNING:
                JFUNIT_OBJECT_FAIL(caller_);
                break;
            case MyTestCase::S_DONE:
                JFUNIT_OBJECT_FAIL(caller_);
                break;
        }
        num_tests_entered_++;
    }

    virtual void leave_test(const jf::unittest::TestCase* t)
    {
        const MyTestCase* mt = dynamic_cast<const MyTestCase*>(t);
        assert(mt);
        switch (mt->state()) {
            case MyTestCase::S_INIT:
                JFUNIT_OBJECT_FAIL(caller_);
                break;
            case MyTestCase::S_RUNNING:
                JFUNIT_OBJECT_FAIL(caller_);
                break;
            case MyTestCase::S_DONE:
                break;
        }
        num_tests_left_++;
    }

    virtual void add_success(const jf::unittest::TestCase*) {}
    virtual void add_failure(const jf::unittest::TestCase*, const jf::unittest::Failure&) {}
    virtual void add_error(const jf::unittest::TestCase*, const std::string&) {}
    virtual void unclean_alarm(const jf::unittest::TestCase*) {}

private:
    jf::unittest::TestCase* caller_;
    int num_suites_entered_;
    int num_suites_left_;
    int num_tests_entered_;
    int num_tests_left_;
};

}

namespace jf {
namespace unittest {
namespace tests {

class TestEnterLeave : public jf::unittest::TestCase
{
public:
    TestEnterLeave() : jf::unittest::TestCase("jf::unittest::TestEnterLeave") {}
    virtual void run()
    {
        MyTestResult result(this);
        MyTestCase test(this);
        test.run_internal(&result, NULL);
        JFUNIT_ASSERT(result.num_tests_entered() == 1);
        JFUNIT_ASSERT(result.num_tests_left() == 1);
    }
};

class SuiteEnterLeave : public jf::unittest::TestCase
{
public:
    SuiteEnterLeave() : jf::unittest::TestCase("jf::unittest::SuiteEnterLeave") {}
    virtual void run()
    {
        MyTestResult result(this);
        jf::unittest::TestSuite suite(/*name=*/"");
        suite.add_test(new MyTestCase(this));
        suite.run_internal(&result, NULL);
        JFUNIT_ASSERT(result.num_suites_entered() == 1);
        JFUNIT_ASSERT(result.num_suites_left() == 1);
        JFUNIT_ASSERT(result.num_tests_entered() == 1);
        JFUNIT_ASSERT(result.num_tests_left() == 1);
    }
};

EnterLeaveSuite::EnterLeaveSuite()
: jf::unittest::TestSuite("jf::unittest::tests::EnterLeaveSuite")
{
    add_test(new TestEnterLeave);
    add_test(new SuiteEnterLeave);
}

}
}
}
