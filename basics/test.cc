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

#include <jf/unittest/test.h>
#include <jf/unittest/test_suite.h>

#include <cassert>

namespace jf {
namespace unittest {

Test::Test()
: parent_(NULL) {}

Test::Test(const std::string& name)
: name_(name),
  parent_(NULL) {}

const TestSuite* Test::root() const
{
    if (parent_ == NULL) {
        const TestSuite* ret = dynamic_cast<const TestSuite*>(this);
        assert(ret);
        return ret;
    }
    else
        return parent_->root();
}

void Test::set_parent_(const TestSuite* suite)
{
    assert(name_.size()!=0); // cannot have zero length path elements
    assert(parent_ == NULL);

    parent_ = suite;
}

std::string Test::path() const
{
    std::string ret;
    if (parent_) {
        ret = parent_->path();
        if (ret != "/")
            ret += '/';
        ret += name_;
    }
    else {
        assert(name_.size()==0);
        ret = '/';
    }    
    return ret;
}

}
}
