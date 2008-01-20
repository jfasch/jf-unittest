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

#ifndef HAVE_JF_UNITTEST_SIMPLE_TEST_RESULT_H
#define HAVE_JF_UNITTEST_SIMPLE_TEST_RESULT_H

#include "test_result.h"

#include <iostream>

namespace jf {
namespace unittest {

class SimpleTestResult : public TestResult
{
public:
    SimpleTestResult(std::ostream* ostream = NULL)
    : ostream_(ostream),
      num_success_(0),
      num_failure_(0),
      num_error_(0) {}

    bool ok() const { return num_failure_ + num_error_ == 0; }
    int num_success() const { return num_success_; }
    int num_failure() const { return num_failure_; }
    int num_error() const { return num_error_; }

    virtual void start_suite(const TestSuite*) {}
    virtual void stop_suite(const TestSuite*) {}
    virtual void start_test(const TestCase*) {}
    virtual void stop_test(const TestCase*) {}
    virtual void add_success(const TestCase*);
    virtual void add_failure(const TestCase*, const Failure&);
    virtual void add_error(const TestCase*, const std::string&);
    
private:
    std::ostream* ostream_;
    int num_success_;
    int num_failure_;
    int num_error_;
};

}
}

#endif
