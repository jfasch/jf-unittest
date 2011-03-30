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

#include <jf/unittest/test_case.h>

#include <jf/unittest/test_result.h>
#include <jf/unittest/failure.h>

#include <cassert>
#include <cstdio>

namespace {

// jjj

// static void add_failure_description(std::string& msg, const jf::unittest::Failure& f)
// {
//     msg += f.failed_condition();
//     msg += " (";
//     msg += f.filename();
//     msg += ':';
//     char tmp[16];
//     std::sprintf(tmp, "%d", f.line());
//     msg += tmp;
//     msg += ')';
// }

}

namespace jf {
namespace unittest {

// jjj

// void TestCase::run_internal(TestResult_Legacy* result)
// {
//     result_ = result;
//     result->enter_test(this);
    
//     bool setup_ok = false;
//     try {
//         this->setup();
//         setup_ok = true;
//     }
//     catch (const FailureException& e) {
//         std::string msg("setup: ");
//         add_failure_description(msg, e.failure());
//         result->add_error(this, msg);
//     }
//     catch (const std::exception& e) {
//         std::string msg("setup: ");
//         msg += e.what();
//         result->add_error(this, msg);
//     }
//     catch (...) {
//         result->add_error(this, "setup: \"...\" caught");
//     }

//     // only if setup went ok go on to execute the test code and
//     // teardown
//     if (setup_ok) {
//         try {
//             this->run();
//             result->add_success(this);
//         }
//         catch (const FailureException& f) {
//             result->add_failure(this, f.failure());
//         }
//         catch (const std::exception& e) {
//             result->add_error(this, e.what());
//         }
//         catch (...) {
//             result->add_error(this, "\"...\" caught");
//         }

//         try {
//             this->teardown();
//         }
//         catch (const FailureException& e) {
//             std::string msg("teardown: ");
//             add_failure_description(msg, e.failure());
//             result->add_error(this, msg);
//         }
//         catch (const std::exception& e) {
//             std::string msg("teardown: ");
//             msg += e.what();
//             result->add_error(this, msg);
//         }
//         catch (...) {
//             result->add_error(this, "teardown: \"...\" caught");
//         }
//     }

//     result->leave_test(this);

//     result_ = 0;
// }

// void TestCase::do_cond_fail(
//     bool condition,
//     const std::string& condition_str,
//     const std::string& filename,
//     int line)
// {
//     if (result_)
//         result_->add_assertion(this);

//     if (!condition)
//         throw FailureException(Failure(condition_str, filename, line));
// }

}
}

