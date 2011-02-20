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

#include <jf/unittest/tests/stage2_suite.h>

#include "setup_teardown.h"
#include "enter_leave.h"
#include "assert_suite.h"
#include "hierarchy_suite.h"

namespace jf {
namespace unittest {
namespace tests {

Stage2Suite::Stage2Suite()
: TestSuite("jf::unittest::tests::Stage2Suite")
{
    add_test(std::auto_ptr<Test>(new SetupTeardownSuite));
    add_test(std::auto_ptr<Test>(new EnterLeaveSuite));
    add_test(std::auto_ptr<Test>(new AssertSuite));
    add_test(std::auto_ptr<Test>(new HierarchySuite));
}

}
}
}
