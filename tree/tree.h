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

#ifndef HAVE_JF_UNITTEST_TREE_WALK_H
#define HAVE_JF_UNITTEST_TREE_WALK_H

#include <jf/unittest/result.h>
#include <jf/unittest/visitor.h>

#include <deque>
#include <vector>

namespace jf {
namespace unittest {

class TreeWalk : private Result, private Visitor
{
public:
    /** @brief Formats fancy tree output
        @param ostream where to write
    */
    TreeWalk(std::ostream& ostream);

    /** @brief Instead of only the node name, print the entire path
        from the root of the tree - suitable for commandline path
        arguments.
    */
    TreeWalk& print_path(bool);

    /** @brief Run each test in a separate process

        This way segfaults and the like show up as regular test
        errors, rather than crashing the entire suite.
     */
    TreeWalk& use_fork(bool);

    bool do_it(Test&);

private:
    /** Visitor implementation */
    //@{
    virtual void enter_suite(const TestSuite*);
    virtual void leave_suite(const TestSuite*);
    virtual void enter_test(const TestCase*);
    virtual void leave_test(const TestCase*);
    //@}

    /** Result implementation */
    //@{
    virtual void add_success(const TestCase*);
    virtual void add_failure(const TestCase*, const Failure&);
    virtual void add_error(const TestCase*, const std::string& message);
    //@}

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
    bool use_fork_;

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

    TreeWalk(const TreeWalk&);
    TreeWalk& operator=(const TreeWalk&);
};

}
}

#endif
