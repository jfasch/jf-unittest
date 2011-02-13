// -*- C++ -*-

// Copyright (C) 2011 Joerg Faschingbauer

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

#include "find_suite.h"

#include <jf/unittest/test_case.h>
#include <jf/unittest/find.h>

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

class PathSearch : public TestCase
{
public:
    PathSearch() : TestCase("PathSearch") {}

    virtual void run()
    {
        // "top" -> element named "top" or NULL
        {
            TestSuite root("root");
            root.add_test(new ATest("top"));
            {
                Test* test = find(&root, "top");
                JFUNIT_ASSERT(test);
                JFUNIT_ASSERT(test->name() == "top");
            }
            {
                Test* test = find(&root, "notop");
                JFUNIT_ASSERT(!test);
            }
        }

        // "" -> root
        {
            TestSuite root("root");
            JFUNIT_ASSERT(find(&root, "") == &root);
        }

        // "node/leaf", "/node/leaf"
        {
            TestSuite root("root");
            TestSuite* node = new TestSuite("node");
            node->add_test(new ATest("leaf"));
            root.add_test(node);
            {
                Test* found = find(&root, "node/leaf");
                JFUNIT_ASSERT(found);
                JFUNIT_ASSERT(found->name() == "leaf");
            }
            {
                Test* found = find(&root, "/node/leaf");
                JFUNIT_ASSERT(found);
                JFUNIT_ASSERT(found->name() == "leaf");
            }
        }
        // "node/leaf/", "/node/leaf/" fail if leaf is not a suite
        {
            TestSuite root("root");
            TestSuite* node = new TestSuite("node");
            node->add_test(new ATest("leaf"));
            root.add_test(node);
            {
                Test* found = find(&root, "node/leaf/");
                JFUNIT_ASSERT(!found);
            }
            {
                Test* found = find(&root, "/node/leaf/");
                JFUNIT_ASSERT(!found);
            }
        }
        // "node/suite/", "/node/suite/" all well
        {
            TestSuite root("root");
            TestSuite* node = new TestSuite("node");
            node->add_test(new TestSuite("suite"));
            root.add_test(node);
            {
                Test* found = find(&root, "node/suite/");
                JFUNIT_ASSERT(found);
                JFUNIT_ASSERT(found->name() == "suite");
            }
            {
                Test* found = find(&root, "/node/suite/");
                JFUNIT_ASSERT(found);
                JFUNIT_ASSERT(found->name() == "suite");
            }
        }
        // absolute path search, starting at arbitrary node.
        {
            TestSuite root("root");
            TestCase* root_leaf = new ATest("root-leaf");
            root.add_test(root_leaf);
            TestSuite* node = new TestSuite("node");
            TestCase* deep_leaf = new ATest("deep-leaf");
            node->add_test(deep_leaf);
            root.add_test(node);

            JFUNIT_ASSERT(find(node, "/root-leaf") == root_leaf);
            JFUNIT_ASSERT(find(node, "/node/deep-leaf") == deep_leaf);
        }
    }
};

}

namespace jf {
namespace unittest {
namespace tests {

FindSuite::FindSuite()
: jf::unittest::TestSuite("jf::unittest::tests::FindSuite")
{
    add_test(new PathSearch);
}

}
}
}
