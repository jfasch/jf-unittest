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

#ifndef HAVE_JF_UNITTEST_TREE_TEST_RUNNER_H
#define HAVE_JF_UNITTEST_TREE_TEST_RUNNER_H

#include <jf/unittest/api.h>
#include <jf/unittest/test_fwd.h>

namespace jf {
namespace unittest {

class JF_UNITTEST_API TreeTestRunner
{
public:
    /** @brief Formats fancy tree output
        @param print_path Instead of only the node name, print the
                          entire path from the root of the tree -
                          suitable for commandline path arguments.
    */
    TreeTestRunner(bool print_path=false);
    bool run(Test*);

private:
    bool print_path_;
};

}
}

#endif
