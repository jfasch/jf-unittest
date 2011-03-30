// -*- C++ -*-

// Copyright (C) 2008-2011 Joerg Faschingbauer

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
#include <jf/unittest/find.h>

#include <cassert>

namespace jf {
namespace unittest {

TestSuite::~TestSuite()
{
    for (Tests::const_iterator i = tests_.begin(); i != tests_.end(); ++i)
        delete *i;
}

void TestSuite::add_test(std::auto_ptr<Test> t)
{
    assert(t.get());
    Test* tp = t.release();
    tests_.push_back(tp);
    tp->set_parent_(this);
}

Test* TestSuite::find(const std::string& path)
{
    if (path.size() == 0)
        return this;
    return jf::unittest::find(this, path);
}

// jjj

// void TestSuite::run_internal(TestResult_Legacy* result)
// {
//     // note that the test's run_internal() method catches all errors,
//     // so it is safe to not wrap the call into try/catch.
//     result->enter_suite(this);
//     for (Tests::const_iterator i = tests_.begin(); i != tests_.end(); ++i)
//         (*i)->run_internal(result);
//     result->leave_suite(this);
// }

}
}
