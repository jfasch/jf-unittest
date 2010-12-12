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

#include <jf/unittest/test_suite.h>

#include <jf/unittest/test_result.h>
#include <jf/unittest/cleanliness.h>

#include <cassert>

namespace jf {
namespace unittest {

TestSuite::~TestSuite()
{
    for (Tests::const_iterator i = tests_.begin(); i != tests_.end(); ++i)
        delete *i;
}

void TestSuite::add_test(Test* t)
{
    assert(t);
    tests_.push_back(t);
}

void TestSuite::run_internal(TestResult* result, const CleanlinessCheck* cleanliness_check)
{
    // note that the test's run_internal() method catches all errors,
    // so it is safe to not wrap the call into try/catch.
    result->enter_suite(this);
    for (Tests::const_iterator i = tests_.begin(); i != tests_.end(); ++i) {
        (*i)->run_internal(result, cleanliness_check);
        if (cleanliness_check && !cleanliness_check->environment_is_clean())
            break;
    }
    result->leave_suite(this);
}

}
}
