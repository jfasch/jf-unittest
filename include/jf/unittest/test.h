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

#ifndef HAVE_JF_UNITTEST_TEST_H
#define HAVE_JF_UNITTEST_TEST_H

#include "test_suite_fwd.h"
#include "test_result_fwd.h"
#include "api.h"

#include <string>

namespace jf {
namespace unittest {

class JF_UNITTEST_API Test
{
public:
    Test();
    Test(const std::string& name);
    virtual ~Test() {}

    const std::string& name() const { return name_; }
    const TestSuite* parent() const { return parent_; }
    const TestSuite* root() const;

    /** @brief Calculate path string towards root */
    std::string path() const;

    virtual void run_internal(TestResult*) = 0;
    
private:
    std::string name_;
    const TestSuite* parent_;

    friend class TestSuite;
    void set_parent_(const TestSuite*);

private:
    Test(const Test&);
    Test& operator=(const Test&);
};

}
}

#endif
