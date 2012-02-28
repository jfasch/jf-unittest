// -*- C++ -*-

// Copyright (C) 2011-2012 Joerg Faschingbauer

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

#include "walk-suite.h"

#include <jf/unittest/case.h>
#include <jf/unittest/walk.h>

#include <vector>

namespace {

using namespace jf::unittest;

class ATest : public TestCase
{
public:
    ATest(const std::string& name)
    : TestCase(name),
      has_run_(false) {}
    bool has_run() const { return has_run_; }
    virtual void run() { has_run_ = true; }
private:
    bool has_run_;
};

class MyVisitor : public Visitor
{
public:
    class Record
    {
    public:
        enum Event {
            EV_ENTER_SUITE,
            EV_LEAVE_SUITE,
            EV_ENTER_CASE,
            EV_LEAVE_CASE
        };
        Record(Event event, const std::string& name) : event_(event), name_(name) {}
        Event event() const { return event_; }
        const std::string& name() const { return name_; }
    private:
        Event event_;
        std::string name_;
    };
    typedef std::vector<Record> Records;

public:
    const Records& records() const { return records_; }
    
private:
    virtual void enter_suite(const TestSuite* s)
    {
        records_.push_back(Record(Record::EV_ENTER_SUITE, s->name()));
    }
    virtual void leave_suite(const TestSuite* s)
    {
        records_.push_back(Record(Record::EV_LEAVE_SUITE, s->name()));
    }
    virtual void enter_test(const TestCase* c)
    {
        records_.push_back(Record(Record::EV_ENTER_CASE, c->name()));
    }
    virtual void leave_test(const TestCase* c)
    {
        records_.push_back(Record(Record::EV_LEAVE_CASE, c->name()));
    }

private:
    Records records_;
};

class WalkTest : public TestCase
{
public:
    WalkTest() : TestCase("Walk") {}

    virtual void run()
    {
        class MyRunner : public Runner
        {
        public:
            typedef std::vector<std::string> stringlist;
            const stringlist& list() const { return list_; }
            virtual void run_test(TestCase* c, Result*)
            {
                list_.push_back(c->name());
            }
        private:
            stringlist list_;
        };

        TestSuite root("root");

        root.add_test(std::auto_ptr<Test>(new ATest("leaf0")));
        std::auto_ptr<TestSuite> node0(new TestSuite("node0"));
        node0->add_test(std::auto_ptr<Test>(new ATest("leaf0.0")));
        node0->add_test(std::auto_ptr<Test>(new ATest("leaf0.1")));
        std::auto_ptr<TestSuite> node0_2(new TestSuite("node0.2"));
        node0_2->add_test(std::auto_ptr<Test>(new ATest("leaf0.2.0")));
        node0->add_test(std::auto_ptr<Test>(node0_2));
        root.add_test(std::auto_ptr<Test>(node0));
        
        std::auto_ptr<TestSuite> node1(new TestSuite("node1"));
        node1->add_test(std::auto_ptr<Test>(new ATest("leaf1.0")));
        node1->add_test(std::auto_ptr<Test>(new ATest("leaf1.1")));
        std::auto_ptr<TestSuite> node1_2(new TestSuite("node1.2"));
        node1_2->add_test(std::auto_ptr<Test>(new ATest("leaf1.2.0")));
        node1->add_test(std::auto_ptr<Test>(node1_2));
        root.add_test(std::auto_ptr<Test>(node1));

        MyVisitor visitor;
        MyRunner runner;
        walk(&root, &visitor, &runner, NULL);

        {
            MyVisitor::Records::const_iterator i = visitor.records().begin();

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_SUITE);
            JFUNIT_ASSERT(i->name() == "root");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_CASE);
            JFUNIT_ASSERT(i->name() == "leaf0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_CASE);
            JFUNIT_ASSERT(i->name() == "leaf0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_SUITE);
            JFUNIT_ASSERT(i->name() == "node0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_CASE);
            JFUNIT_ASSERT(i->name() == "leaf0.0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_CASE);
            JFUNIT_ASSERT(i->name() == "leaf0.0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_CASE);
            JFUNIT_ASSERT(i->name() == "leaf0.1");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_CASE);
            JFUNIT_ASSERT(i->name() == "leaf0.1");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_SUITE);
            JFUNIT_ASSERT(i->name() == "node0.2");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_CASE);
            JFUNIT_ASSERT(i->name() == "leaf0.2.0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_CASE);
            JFUNIT_ASSERT(i->name() == "leaf0.2.0");
            ++i;
        
            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_SUITE);
            JFUNIT_ASSERT(i->name() == "node0.2");
            ++i;
        
            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_SUITE);
            JFUNIT_ASSERT(i->name() == "node0");
            ++i;
        

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_SUITE);
            JFUNIT_ASSERT(i->name() == "node1");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_CASE);
            JFUNIT_ASSERT(i->name() == "leaf1.0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_CASE);
            JFUNIT_ASSERT(i->name() == "leaf1.0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_CASE);
            JFUNIT_ASSERT(i->name() == "leaf1.1");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_CASE);
            JFUNIT_ASSERT(i->name() == "leaf1.1");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_SUITE);
            JFUNIT_ASSERT(i->name() == "node1.2");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_ENTER_CASE);
            JFUNIT_ASSERT(i->name() == "leaf1.2.0");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_CASE);
            JFUNIT_ASSERT(i->name() == "leaf1.2.0");
            ++i;
        
            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_SUITE);
            JFUNIT_ASSERT(i->name() == "node1.2");
            ++i;
        
            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_SUITE);
            JFUNIT_ASSERT(i->name() == "node1");
            ++i;

            JFUNIT_ASSERT(i != visitor.records().end());
            JFUNIT_ASSERT(i->event() == MyVisitor::Record::EV_LEAVE_SUITE);
            JFUNIT_ASSERT(i->name() == "root");
            ++i;
        

            JFUNIT_ASSERT(i==visitor.records().end());
        }

        {
            MyRunner::stringlist::const_iterator i = runner.list().begin();

            JFUNIT_ASSERT(i != runner.list().end());
            JFUNIT_ASSERT(*i == "leaf0");
            ++i;

            JFUNIT_ASSERT(i != runner.list().end());
            JFUNIT_ASSERT(*i == "leaf0.0");
            ++i;

            JFUNIT_ASSERT(i != runner.list().end());
            JFUNIT_ASSERT(*i == "leaf0.1");
            ++i;

            JFUNIT_ASSERT(i != runner.list().end());
            JFUNIT_ASSERT(*i == "leaf0.2.0");
            ++i;

            JFUNIT_ASSERT(i != runner.list().end());
            JFUNIT_ASSERT(*i == "leaf1.0");
            ++i;

            JFUNIT_ASSERT(i != runner.list().end());
            JFUNIT_ASSERT(*i == "leaf1.1");
            ++i;

            JFUNIT_ASSERT(i != runner.list().end());
            JFUNIT_ASSERT(*i == "leaf1.2.0");
            ++i;
        }
    }
};

/** Leaves the runner argument to walk() NULL -> does not crash
 */
class WalkNullRunner : public TestCase
{
public:
    WalkNullRunner() : TestCase("WalkNullRunner") {}

    virtual void run()
    {
        class MyVisitor : public Visitor
        {
        public:
            virtual void enter_suite(const TestSuite*) {}
            virtual void leave_suite(const TestSuite*) {}
            virtual void enter_test(const TestCase*) {}
            virtual void leave_test(const TestCase*) {}
        };
        
        TestSuite root("root");
        std::auto_ptr<TestSuite> node0(new TestSuite("node"));
        node0->add_test(std::auto_ptr<Test>(new ATest("leaf")));

        MyVisitor visitor;
        walk(&root, &visitor, NULL, NULL);
    }
};

/** Leaves the visitor argument to walk() NULL -> does not crash
 */
class WalkNullVisitor : public TestCase
{
public:
    WalkNullVisitor() : TestCase("WalkNullVisitor") {}

    virtual void run()
    {
        class MyRunner : public Runner
        {
        public:
            virtual void run_test(TestCase*, Result*) {}
        };
        
        TestSuite root("root");
        std::auto_ptr<TestSuite> node0(new TestSuite("node"));
        node0->add_test(std::auto_ptr<Test>(new ATest("leaf")));

        MyRunner runner;
        walk(&root, NULL, &runner, NULL);
    }
};

}

namespace jf {
namespace unittest {
namespace tests {

Walk::Walk()
: jf::unittest::TestSuite("Walk")
{
    add_test(std::auto_ptr<Test>(new WalkTest));
    add_test(std::auto_ptr<Test>(new WalkNullRunner));
    add_test(std::auto_ptr<Test>(new WalkNullVisitor));
}

}
}
}
