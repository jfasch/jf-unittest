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

#ifndef HAVE_JF_UNITTEST_CASE_H
#define HAVE_JF_UNITTEST_CASE_H

#include "test.h"
#include "failure.h"

namespace jf {
namespace unittest {

class TestCase : public Test
{
public:
    TestCase(const std::string& name) : Test(name) {}
    virtual ~TestCase() {}

protected:
    friend class DirectRunner;
    
    /** These are **only** called via DirectRunner */
    //@{
    virtual void setup() {}
    virtual void run() = 0;
    virtual void teardown() {}
    //@}

public:
#   define JFUNIT_ASSERT(condition)                                     \
    do {                                                                \
        if (!(condition))                                               \
            throw jf::unittest::FailureException(jf::unittest::Failure(#condition, __FILE__, __LINE__)); \
    } while (false)
#   define JFUNIT_FAIL() JFUNIT_ASSERT(false)
#   define JFUNIT_ASSERT_THROWS(Exception, expr) \
    do { \
        try { \
            expr; \
            JFUNIT_FAIL(); \
        } \
        catch (const Exception&) {} \
        catch (...) { \
            JFUNIT_FAIL(); \
        } \
    } while (false)
};

}
}

#endif
