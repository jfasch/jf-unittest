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

#include <jf/unittest/test_result.h>

#include <deque>
#include <vector>

namespace jf {
namespace unittest {

class JF_UNITTEST_API TreeTestResult : public jf::unittest::TestResult_Legacy
{
public:
    /** @brief Formats fancy tree output
        @param ostream where to write
        @param print_path Instead of only the node name, print the
                          entire path from the root of the tree -
                          suitable for commandline path arguments.
    */
    TreeTestResult(std::ostream& ostream, bool print_path);

    virtual void enter_suite(const TestSuite*);
    virtual void leave_suite(const TestSuite*);
    virtual void enter_test(const TestCase*);
    virtual void leave_test(const TestCase*);
    virtual void add_success(const TestCase*);
    virtual void add_failure(const TestCase*, const Failure&);
    virtual void add_error(const TestCase*, const std::string& message);
    virtual void add_assertion(const TestCase*);

    void print_summary() const;
    bool ok() const { return num_success_ == num_tests_run_; }

public:
    typedef std::deque<const TestSuite*> SuiteStack;

private:
    class Report {
    public:
        Report(const TestCase* c, const Failure& f)
        : testcase_(c),
          type_(T_FAILURE),
          failure_(f) {}
        Report(const TestCase* c, const std::string& m)
        : testcase_(c),
          type_(T_ERROR),
          failure_(std::string(), std::string(), 0),
          message_(m) {}
        void print(std::ostream&) const;
    private:
        enum Type {
          T_FAILURE,
          T_ERROR
        };
        const TestCase* testcase_;
        Type type_;
        Failure failure_;
        std::string message_;
    };
    typedef std::vector<Report> Reports;

private:
    std::ostream& ostream_;
    bool print_path_;

    SuiteStack suite_stack_;

    Failure cur_failure;
    const Failure* p_cur_failure;
    std::string cur_error;
    const std::string* p_cur_error;

    Reports reports_;

    int num_suites_entered_;
    int num_tests_run_;
    int num_success_;
    int num_failure_;
    int num_error_;
    int num_assertion_;

    TreeTestResult(const TreeTestResult&);
    TreeTestResult& operator=(const TreeTestResult&);
};

}
}

#endif
