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

namespace {

class Failure
{
public:
    Failure(const std::string& condition_str, const std::string& filename, int line)
    : condition_str_(condition_str),
      filename_(filename),
      line_(line) {}
private:
    std::string condition_str_;
    std::string filename_;
    int line_;
};

}

namespace jf {
namespace unittest {

void TestCase::do_cond_fail(
    bool condition,
    const std::string& condition_str,
    const std::string& filename,
    int line)
{
    if (!condition)
        throw Failure(condition_str, filename, line);
}

void TestCase::run_internal(
    TestResult* result)
{
    try {
        run();
        result->add_success();
    }
    catch (const Failure& f) {
        result->add_failure();
    }
    catch (const std::exception&) {
        result->add_error();
    }
    catch (...) {
        result->add_error();
    }
}

}
}

