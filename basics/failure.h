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

#ifndef HAVE_JF_UNITTEST_FAILURE_H
#define HAVE_JF_UNITTEST_FAILURE_H

#include <string>

namespace jf {
namespace unittest {

class Failure
{
public:
    Failure(const std::string& failed_condition, const std::string& filename, int line)
    : failed_condition_(failed_condition),
      filename_(filename),
      line_(line) {}

    const std::string& failed_condition() const { return failed_condition_; }
    const std::string& filename() const { return filename_; }
    int line() const { return line_; }

private:
    std::string failed_condition_;
    std::string filename_;
    int line_;
};

/** Thrown by tests on failure */
class FailureException : public std::exception
{
public:
    FailureException(const jf::unittest::Failure& failure) : failure_(failure) {}
    const jf::unittest::Failure& failure() const { return failure_; }
    /** Inherited from std::exception. Overloaded fatally since nobody
        should get a chance to call it. */
    virtual const char* what();

private:
    jf::unittest::Failure failure_;
};

}
}

#endif
