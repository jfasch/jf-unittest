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

#include <jf/unittest/tests/stage2_suite.h>
#include <jf/unittest/direct_runner.h>
#include <jf/unittest/walk.h>
#include <jf/unittest/result.h>

namespace {

class MyTestResult : public jf::unittest::Result
{
public:
    MyTestResult() : num_success_(0), num_failure_(0), num_error_(0) {}

    bool ok() const { return num_success_ > 0 && num_failure_ == 0 && num_error_ == 0; }

    virtual void add_success(const jf::unittest::TestCase*) { num_success_++; }
    virtual void add_failure(const jf::unittest::TestCase*, const jf::unittest::Failure&) { num_failure_++; }
    virtual void add_error(const jf::unittest::TestCase*, const std::string& message) { num_error_++; }

private:
    int num_success_;
    int num_failure_;
    int num_error_;
};

}

int main()
{
    jf::unittest::tests::Stage2Suite suite;
    MyTestResult result;
    jf::unittest::DirectRunner runner;

    walk(&suite, NULL, &runner, &result);

    return result.ok()? 0: 1;
}
