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

#include "result.h"

#include "suite.h"
#include "find.h"

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

}
}
