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

#include "test_case.h"

#include "test_result.h"
#include "failure.h"

#include <cassert>

namespace {

class FailureException : public std::exception
{
public:
    FailureException(const jf::unittest::Failure& failure) : failure_(failure) {}
    const jf::unittest::Failure& failure() const { return failure_; }
    /** Inherited from std::exception. Overloaded fatally since nobody
        should get a chance to call it. */
    virtual const char* what() { assert(false); }

private:
    jf::unittest::Failure failure_;
};

static void add_failure_description(std::string& msg, const jf::unittest::Failure& f)
{
    msg += f.failed_condition();
    msg += " (";
    msg += f.filename();
    msg += ':';
    char tmp[16];
    sprintf(tmp, "%d", f.line());
    msg += tmp;
    msg += ')';
}

}

namespace jf {
namespace unittest {

void TestCase::run_internal(
    TestResult* result)
{
    result->enter_test(this);
    
    bool setup_ok = false;
    try {
        this->setup();
        setup_ok = true;
    }
    catch (const FailureException& e) {
        std::string msg("setup: ");
        add_failure_description(msg, e.failure());
        result->add_error(this, msg);
    }
    catch (const std::exception& e) {
        std::string msg("setup: ");
        msg += e.what();
        result->add_error(this, msg);
    }
    catch (...) {
        result->add_error(this, "setup: \"...\" caught");
    }

    // only if setup went ok go on to execute the test code and
    // teardown
    if (setup_ok) {
        try {
            this->run();
            result->add_success(this);
        }
        catch (const FailureException& f) {
            result->add_failure(this, f.failure());
        }
        catch (const std::exception& e) {
            result->add_error(this, e.what());
        }
        catch (...) {
            result->add_error(this, "\"...\" caught");
        }

        try {
            this->teardown();
        }
        catch (const FailureException& e) {
            std::string msg("teardown: ");
            add_failure_description(msg, e.failure());
            result->add_error(this, msg);
        }
        catch (const std::exception& e) {
            std::string msg("teardown: ");
            msg += e.what();
            result->add_error(this, msg);
        }
        catch (...) {
            result->add_error(this, "teardown: \"...\" caught");
        }
    }

    result->leave_test(this);
}

void TestCase::do_cond_fail(
    bool condition,
    const std::string& condition_str,
    const std::string& filename,
    int line)
{
    if (!condition)
        throw FailureException(Failure(condition_str, filename, line));
}

}
}

