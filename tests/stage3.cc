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

// CONFIX:REQUIRE_H('jf/unittest/tests/stage2_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/unittest/tree_test_runner.h', REQUIRED)

#include <jf/unittest/tests/stage2_suite.h>
#include <jf/unittest/tests/stage3_suite.h>
#include <jf/unittest/tree_test_runner.h>

int main()
{
    jf::unittest::TestSuite suite("Stage3");
    suite.add_test(new jf::unittest::tests::Stage2Suite);
    suite.add_test(new jf::unittest::tests::Stage3Suite);

    jf::unittest::TreeTestRunner runner;
    return runner.run(&suite)? 0: 1;
}
