 // -*- C++ -*-

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

#include "direct-runner.h"

#include <cstdio>

#include <iostream>
using namespace std;

static void add_failure_description(
    std::string& msg,
    const jf::unittest::Failure& f)
{
    msg += f.failed_condition();
    msg += " (";
    msg += f.filename();
    msg += ':';
    char tmp[16];
    std::sprintf(tmp, "%d", f.line());
    msg += tmp;
    msg += ')';
}

namespace jf {
namespace unittest {

void DirectRunner::run_test(
    TestCase* test_case,
    Result* result)
{
    bool setup_ok = false;
    
    try {
        test_case->setup();
        setup_ok = true;
    }
    catch (const FailureException& e) {
        // add both a failure (containing the entire cause and file,
        // line) and an error (to indicate a "very hard" condition).
        result->add_failure(test_case, e.failure());

        std::string msg("setup error: ");
        add_failure_description(msg, e.failure());
        result->add_error(test_case, msg);
    }
    catch (const std::exception& e) {
        std::string msg("setup error: ");
        msg += e.what();
        result->add_error(test_case, msg);
    }
    catch (...) {
        result->add_error(test_case, "setup error: \"...\" caught");
    }

    // only if setup went ok go on to execute the test code and
    // teardown
    if (setup_ok) {
        try {
            test_case->run();
            result->add_success(test_case);
        }
        catch (const FailureException& f) {
            result->add_failure(test_case, f.failure());
        }
        catch (const std::exception& e) {
            result->add_error(test_case, e.what());
        }
        catch (...) {
            result->add_error(test_case, "\"...\" caught");
        }

        try {
            test_case->teardown();
        }
        catch (const FailureException& e) {
            std::string msg("teardown: ");
            add_failure_description(msg, e.failure());
            result->add_error(test_case, msg);
        }
        catch (const std::exception& e) {
            std::string msg("teardown: ");
            msg += e.what();
            result->add_error(test_case, msg);
        }
        catch (...) {
            result->add_error(test_case, "teardown: \"...\" caught");
        }
    }
}

}
}
