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

#include "test_suite.h"

namespace jf {
namespace unittest {

TestSuite::~TestSuite()
{
    for (int i=0; i<tests_.size(); i++)
        delete tests_[i];
}

void TestSuite::add_test(Test* t)
{
    assert(t);
    tests_.push_back(t);
}

void TestSuite::run_internal(TestResult* result)
{
    for (int i=0; i<tests_.size(); i++)
        tests_[i]->run_internal(result);
}

}
}
