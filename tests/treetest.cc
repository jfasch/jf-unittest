// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2011-2012 Joerg Faschingbauer

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

#include <jf/unittest/suite.h>
#include <jf/unittest/case.h>
#include <jf/unittest/main.h>

using namespace jf::unittest;
using namespace std;

namespace {

class MyTest : public TestCase
{
public:
    MyTest(const std::string& name) : TestCase(name) {}
    virtual void run() {}
};

}

int main(int argc, char** argv)
{
    TestSuite* suite = new TestSuite("suite");
    suite->add_test(std::auto_ptr<Test>(new MyTest("test")));
    return jf::unittest::main(argc, argv, std::auto_ptr<Test>(suite));
}
