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

#ifndef HAVE_JF_UNITTEST_TREE_TEST_RESULT_H
#define HAVE_JF_UNITTEST_TREE_TEST_RESULT_H

namespace jf {
namespace unittest {

class TreeTestResult
{
public:
    virtual void add_success(const TestCase*) = 0;
    virtual void add_failure(const TestCase*, const Failure&) = 0;
    virtual void add_error(const TestCase*, const std::string& message) = 0;
};

}
}

#endif
