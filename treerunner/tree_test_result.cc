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

#include <jf/unittest/tree_test_result.h>

#include <jf/unittest/test_case.h>
#include <jf/unittest/test_suite.h>

#include <iostream>
#include <cassert>

namespace {

class indent
{
public:
    indent(int num) : num_(num) {}
    int num() const { return num_; }
private:
    int num_;
};
static std::ostream& operator<<(std::ostream& o, const indent& i)
{
    int num = i.num();
    while (num--)
        o << "  ";
    return o;
}

}

namespace jf {
namespace unittest {

void TreeTestResult::Report::print(std::ostream& o) const
{
    switch (type_) {
        case T_FAILURE: {
            o << "  Failure: " << testcase_->name() << ": "
              << failure_.failed_condition()
              << " (" << failure_.filename() << ':' << failure_.line() << ')';
            break;
        }
        case T_ERROR: {
            o << "  Error:   " << testcase_->name() << ": " << message_;
            break;
        }
    }
    o << '\n';
}

TreeTestResult::TreeTestResult(std::ostream& ostream)
: ostream_(ostream),
  cur_failure(std::string(), std::string(), 0),
  p_cur_failure(NULL),
  p_cur_error(NULL),
  num_suites_entered_(0),
  num_tests_run_(0),
  num_success_(0),
  num_failure_(0),
  num_error_(0),
  num_assertion_(0) {}

void TreeTestResult::enter_suite(const TestSuite* s)
{
    num_suites_entered_++;
    ostream_ << indent(suite_stack_.size()) << "+ " << s->name() << '\n';
    suite_stack_.push(s);
}

void TreeTestResult::leave_suite(const TestSuite* /*s*/)
{
    assert(suite_stack_.size() != 0);
    suite_stack_.pop();
}

void TreeTestResult::enter_test(const TestCase* c)
{
    num_tests_run_++;
    ostream_ << indent(suite_stack_.size()) << "- " << c->name() << "...";
}

void TreeTestResult::leave_test(const TestCase* c)
{
    if (p_cur_error) {
        num_error_++;
        reports_.push_back(Report(c, *p_cur_error));
        ostream_ << "error";
    }
    else if (p_cur_failure) {
        num_failure_++;
        reports_.push_back(Report(c, *p_cur_failure));
        ostream_ << "failed";
    }
    else {
        num_success_++;
        ostream_ << "ok";
    }

    ostream_ << '\n';
    p_cur_failure = NULL;
    p_cur_error = NULL;
}

void TreeTestResult::add_success(const TestCase*) {}

void TreeTestResult::add_failure(const TestCase*, const Failure& f)
{
    cur_failure = f;
    p_cur_failure = &cur_failure;
}

void TreeTestResult::add_error(const TestCase*, const std::string& message)
{
    cur_error = message;
    p_cur_error = &cur_error;
}

void TreeTestResult::print_summary() const
{
    ostream_ << "------------------------\n";
    
    ostream_ << "#Success:          " << num_success_ << '\n';
    ostream_ << "#Failures:         " << num_failure_ << '\n';
    ostream_ << "#Errors:           " << num_error_ << '\n';
    ostream_ << "#Tests run:        " << num_tests_run_ << '\n';
    ostream_ << "#Suites entered:   " << num_suites_entered_ << '\n';
    ostream_ << "#Asserts entered:  " << num_assertion_ << '\n';
    ostream_ << "------------------------\n";

    if (!ok()) {
        ostream_ << "Details follow ...\n";
        ostream_ << "------------------------\n";
        for (Reports::const_iterator i = reports_.begin();
             i != reports_.end();
             ++i)
            i->print(ostream_);
    }
}

void TreeTestResult::add_assertion( const TestCase* )
{
    num_assertion_++;
}

}
}
