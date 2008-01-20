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

#ifndef HAVE_JF_UNITTEST_TEST_CASE_H
#define HAVE_JF_UNITTEST_TEST_CASE_H

#include "test.h"

namespace jf {
namespace unittest {

class TestCase : public Test {
public:
    TestCase(const std::string& name) : Test(name) {}
    virtual ~TestCase() {}

    virtual void setup() {}
    virtual void run() = 0;
    virtual void teardown() {}

public:
    virtual void run_internal(TestResult*);

#   define JFUNIT_OBJECT_ASSERT(testcase, condition) \
        do { \
            testcase->do_cond_fail((condition), #condition, __FILE__, __LINE__); \
        } while (false)
#   define JFUNIT_OBJECT_FAIL(testcase) JFUNIT_OBJECT_ASSERT(testcase, false)
#   define JFUNIT_ASSERT(condition) JFUNIT_OBJECT_ASSERT(this, condition)
#   define JFUNIT_FAIL() JFUNIT_ASSERT(false)

    void do_cond_fail(bool condition,
                      const std::string& condition_str,
                      const std::string& filename,
                      int line);
};

}
}

#endif
