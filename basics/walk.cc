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

#include "walk.h"

#include "test_case.h"
#include "test_suite.h"

#include <cassert>

namespace jf {
namespace unittest {

void walk(TestSuite* suite, Visitor* visitor, Runner* runner, Result* result)
{
    if (visitor)
        visitor->enter_suite(suite);
    for (TestSuite::Tests::const_iterator i = suite->tests().begin();
         i != suite->tests().end();
         ++i) {
        TestCase* c = dynamic_cast<TestCase*>(*i);
        if (c) {
            if (visitor)
                visitor->enter_test(c);
            if (runner)
                runner->run_test(c, result);
            if (visitor)
                visitor->leave_test(c);
            continue;
        }
        TestSuite* s = dynamic_cast<TestSuite*>(*i);
        if (s) {
            walk(s, visitor, runner, result);
            continue;
        }
        assert(!"what's this?");        
    }
    if (visitor)
        visitor->leave_suite(suite);
}

}
}
