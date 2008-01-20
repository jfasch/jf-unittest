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

#include "start_stop.h"

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
      num_suites_started_(0),
      num_suites_stopped_(0),
      num_tests_started_(0),
      num_tests_stopped_(0) {}

    int num_suites_started() const { return num_suites_started_; }
    int num_suites_stopped() const { return num_suites_stopped_; }
    int num_tests_started() const { return num_tests_started_; }
    int num_tests_stopped() const { return num_tests_stopped_; }

    virtual void start_suite(const jf::unittest::TestSuite* s)
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
        num_suites_started_++;
    }

    virtual void stop_suite(const jf::unittest::TestSuite* s)
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
        num_suites_stopped_++;
    }

    virtual void start_test(const jf::unittest::TestCase* t)
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
        num_tests_started_++;
    }

    virtual void stop_test(const jf::unittest::TestCase* t)
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
        num_tests_stopped_++;
    }

    virtual void add_success(const jf::unittest::TestCase*) {}
    virtual void add_failure(const jf::unittest::TestCase*, const jf::unittest::Failure&) {}
    virtual void add_error(const jf::unittest::TestCase*, const std::string& message) {}

private:
    jf::unittest::TestCase* caller_;
    int num_suites_started_;
    int num_suites_stopped_;
    int num_tests_started_;
    int num_tests_stopped_;
};

}

namespace jf {
namespace unittest {
namespace tests {

class TestStartStop : public jf::unittest::TestCase
{
public:
    TestStartStop() : jf::unittest::TestCase("jf::unittest::TestStartStop") {}
    virtual void run()
    {
        MyTestResult result(this);
        MyTestCase test(this);
        test.run_internal(&result);
        JFUNIT_ASSERT(result.num_tests_started() == 1);
        JFUNIT_ASSERT(result.num_tests_stopped() == 1);
    }
};

class SuiteStartStop : public jf::unittest::TestCase
{
public:
    SuiteStartStop() : jf::unittest::TestCase("jf::unittest::SuiteStartStop") {}
    virtual void run()
    {
        MyTestResult result(this);
        jf::unittest::TestSuite suite(/*name=*/"");
        suite.add_test(new MyTestCase(this));
        suite.run_internal(&result);
        JFUNIT_ASSERT(result.num_suites_started() == 1);
        JFUNIT_ASSERT(result.num_suites_stopped() == 1);
        JFUNIT_ASSERT(result.num_tests_started() == 1);
        JFUNIT_ASSERT(result.num_tests_stopped() == 1);
    }
};

StartStopSuite::StartStopSuite()
: jf::unittest::TestSuite("jf::unittest::tests::StartStopSuite")
{
    add_test(new TestStartStop);
    add_test(new SuiteStartStop);
}

}
}
}
