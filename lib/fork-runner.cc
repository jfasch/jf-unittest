 // -*- C++ -*-

// Copyright (C) 2012 Joerg Faschingbauer

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

#include "fork-runner.h"

#include "direct-runner.h"

#include <cstdio>
#include <cassert>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <alloca.h>
#include <sys/wait.h>

namespace {

using namespace jf::unittest;

class OneTestResult : public jf::unittest::Result
{
public:
    OneTestResult()
    : success_(false),
      has_failure_(false),
      failure_("", "", 0),
      has_error_(false) {}
    bool success() const { return success_; }
    bool has_failure() const { return has_failure_; }
    const Failure& failure() const { return failure_; }
    bool has_error() const { return has_error_; }
    const std::string& error_message() const { return error_message_; }
    
    virtual void add_success(
        const TestCase*)
    { 
        assert(!success_);
        success_ = true; 
    }
    virtual void add_failure(
        const TestCase*,
        const Failure& f)
    {
        assert(!has_failure_);
        has_failure_ = true;
        failure_ = f;
    }
    virtual void add_error(
        const TestCase*,
        const std::string& message)
    {
        assert(!has_error_);
        has_error_ = true;
        error_message_ = message;
    }

private:
    bool success_;

    bool has_failure_;
    Failure failure_;

    bool has_error_;
    std::string error_message_;
};

static void write_bytes(int fd, const char* bytes, size_t num)
{
    size_t nwritten = 0;
    while (nwritten != num) {
        int cur = ::write(fd, bytes+nwritten, num-nwritten);
        if (cur < 0) {
            // this is the child end of the pipe. a hard exit is
            // perfectly ok - the parent will notice.
            ::perror("write");
            _exit(1);
        }
        nwritten += cur;
    }
}

static void read_bytes(int fd, char* bytes, size_t num)
{
    size_t nread = 0;
    while (nread != num) {
        int cur = ::read(fd, bytes+nread, num-nread);
        if (cur < 0)
            throw errno;
        if (cur == 0)
            throw true;
        nread += cur;
    }
}

static void write_bool(int fd, bool value)
{
    write_bytes(fd, (const char*)&value, sizeof value);
}

static void read_bool(int fd, bool& b)
{
    read_bytes(fd, (char*)&b, sizeof b);
}

static void write_size(int fd, size_t size)
{
    write_bytes(fd, (const char*)&size, sizeof size);
}

static void read_size(int fd, size_t& size) 
{
    read_bytes(fd, (char*)&size, sizeof size);
}

static void write_int(int fd, int i)
{
    write_bytes(fd, (const char*)&i, sizeof i);
}

static void read_int(int fd, int& i)
{
    read_bytes(fd, (char*)&i, sizeof(int));
}

static void write_string(int fd, const std::string& s)
{
    write_size(fd, s.size());
    write_bytes(fd, s.c_str(), s.size());
}

static void read_string(int fd, std::string& s)
{
    size_t size;
    read_size(fd, size);
    char* tmp = (char*)alloca(size+1);
    read_bytes(fd, tmp, size);
    ((char*)tmp)[size] = 0;
    s = (char*)tmp;
}

static std::string make_strerror(int err)
{
    char tmp[64];
    char* errstr = strerror_r(err, tmp, sizeof tmp);
    return std::string(errstr);
}

}

namespace jf {
namespace unittest {

void ForkRunner::run_test(
    TestCase* test_case,
    Result* result)
{
    int channel[2];
    int rv = ::pipe(channel);
    if (rv != 0) {
        result->add_error(test_case, make_strerror(errno));
        return;
    }
    
    pid_t pid = fork();
    if (pid == 0) { // child
        ::close(channel[0]);
        OneTestResult local_result;
        DirectRunner().run_test(test_case, &local_result);

        write_bool(channel[1], local_result.success());
        write_bool(channel[1], local_result.has_failure());
        if (local_result.has_failure()) {
            write_string(channel[1], local_result.failure().failed_condition());
            write_string(channel[1], local_result.failure().filename());
            write_int(channel[1], local_result.failure().line());
        }
        write_bool(channel[1], local_result.has_error());
        if (local_result.has_error())
            write_string(channel[1], local_result.error_message());
        ::close(channel[1]);
        _exit(0);
    }
    else { // parent
        ::close(channel[1]);

        bool b;
        try {
            read_bool(channel[0], b);
            if (b)
                result->add_success(test_case);
            read_bool(channel[0], b);
            if (b) {
                std::string cond, filename;
                int line;
                read_string(channel[0], cond);
                read_string(channel[0], filename);
                read_int(channel[0], line);
                result->add_failure(test_case, Failure(cond, filename, line));
            }
            read_bool(channel[0], b);
            if (b) {
                std::string msg;
                read_string(channel[0], msg);
            }
            ::close(channel[0]);
        }
        catch (const int& err) {
            result->add_error(test_case, std::string("internal pipe: read error: ")+make_strerror(err));
        }
        catch (const bool&) {
            // ... which is EOF
            result->add_error(test_case, std::string("internal pipe: eof during read"));
        }

        int status;
        pid_t died = ::waitpid(pid, &status, 0);
        assert(died == pid);
    }
}

}
}
