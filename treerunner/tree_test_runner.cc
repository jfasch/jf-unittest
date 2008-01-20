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

#include "tree_test_runner.h"

#include "tree_test_result.h"

#include <jf/unittest/test.h>

#include <iostream>

namespace jf {
namespace unittest {

bool TreeTestRunner::run(Test* test)
{
    TreeTestResult result(std::cerr);
    test->run_internal(&result);
    result.print_summary();
    return result.ok();
}

}
}
