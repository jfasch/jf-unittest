// -*- C++ -*-

// Copyright (C) 2011 Joerg Faschingbauer

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

#ifndef HAVE_JF_UNITTEST_VISITOR_H
#define HAVE_JF_UNITTEST_VISITOR_H

#include "test_suite.h"
#include "test_case.h"

namespace jf {
namespace unittest {

class Visitor
{
public:
    virtual ~Visitor() {}

    virtual void enter_suite(const TestSuite*) = 0;
    virtual void leave_suite(const TestSuite*) = 0;
    virtual void enter_test(const TestCase*) = 0;
    virtual void leave_test(const TestCase*) = 0;
};

}
}

#endif
