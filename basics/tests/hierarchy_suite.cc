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

#include "hierarchy_suite.h"

#include <jf/unittest/test_case.h>

#include <cassert>

namespace {

using namespace jf::unittest;

class ATest : public TestCase
{
public:
    ATest(const std::string& name) : TestCase(name) {}
    virtual void run() { assert(false); }
};

class Path : public TestCase
{
public:
    Path() : TestCase("Path") {}

    virtual void run()
    {
        // "/top"
        {
            TestSuite root;
            Test* test = new ATest("top");
            root.add_test(std::auto_ptr<Test>(test));
            JFUNIT_ASSERT(test->path() == "/top");
        }

        // "/" -> root
        {
            TestSuite root;
            JFUNIT_ASSERT(root.path() == "/");
        }

        // "/node/leaf"
        {
            TestSuite root;
            TestSuite* node = new TestSuite("node");
            Test* leaf = new ATest("leaf");
            node->add_test(std::auto_ptr<Test>(leaf));
            root.add_test(std::auto_ptr<Test>(node));
            JFUNIT_ASSERT(node->path() == "/node");
            JFUNIT_ASSERT(leaf->path() == "/node/leaf");
        }
        // "/node/suite"
        {
            TestSuite root;
            TestSuite* node = new TestSuite("node");
            Test* suite = new TestSuite("suite");
            node->add_test(std::auto_ptr<Test>(suite));
            root.add_test(std::auto_ptr<Test>(node));
            JFUNIT_ASSERT(node->path() == "/node");
            JFUNIT_ASSERT(suite->path() == "/node/suite");
        }
    }
};

class Find : public TestCase
{
public:
    Find() : TestCase("Find") {}

    virtual void run()
    {
        // "/top"
        {
            TestSuite root;
            Test* test = new ATest("top");
            root.add_test(std::auto_ptr<Test>(test));
            JFUNIT_ASSERT(root.find("/top") == test);
            JFUNIT_ASSERT(root.find("top") == test);
            JFUNIT_ASSERT(root.find("") == &root);
        }

        // "/" -> root
        {
            TestSuite root;
            JFUNIT_ASSERT(root.find("/") == &root);
        }

        // "/node/leaf"
        {
            TestSuite root;
            TestSuite* node = new TestSuite("node");
            Test* leaf = new ATest("leaf");
            node->add_test(std::auto_ptr<Test>(leaf));
            root.add_test(std::auto_ptr<Test>(node));
            JFUNIT_ASSERT(node->find("/node") == node);
            JFUNIT_ASSERT(root.find("/node") == node);
            JFUNIT_ASSERT(root.find("/node/leaf") == leaf);
            JFUNIT_ASSERT(root.find("node/leaf") == leaf);
            JFUNIT_ASSERT(root.find("node") == node);
        }
        // "/node/suite"
        {
            TestSuite root;
            TestSuite* node = new TestSuite("node");
            TestSuite* suite = new TestSuite("suite");
            node->add_test(std::auto_ptr<Test>(suite));
            root.add_test(std::auto_ptr<Test>(node));
            JFUNIT_ASSERT(node->find("/node") == node);
            JFUNIT_ASSERT(suite->find("/node/suite") == suite);
            JFUNIT_ASSERT(root.find("/node") == node);
            JFUNIT_ASSERT(root.find("/node/suite") == suite);

            JFUNIT_ASSERT(root.find("node") == node);
            JFUNIT_ASSERT(root.find("node/suite") == suite);
        }
    }
};

}

namespace jf {
namespace unittest {
namespace tests {

Hierarchy::Hierarchy()
: jf::unittest::TestSuite("Hierarchy")
{
    add_test(std::auto_ptr<Test>(new Path));
    add_test(std::auto_ptr<Test>(new Find));
}

}
}
}
