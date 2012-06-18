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

#include "main.h"

#include "suite.h"
#include "find.h"
#include "tree.h"

#include <cassert>
#include <iostream>
#include <getopt.h>

namespace jf {
namespace unittest {

static void usage(const char* argv0)
{
    std::cerr << "Usage: " << argv0 << " [OPTION...] [path]"
              << '\n'
              << "Run all tests in the suite, or just one if path is given"
              << '\n'
              << "Options:\n"
              << "  -p, --print-path\n"
              << "       Print the path to each test as it runs.\n"
              << "       The printed paths are suitable for passing as\n"
              << "       commandline argument for subsequent test runs.\n"
              << "  -f, --dont-use-fork\n"
              << "       Do not use fork to encapsulate each test's\n"
              << "       environment. Rather, run all tests in the same\n"
              << "       process. A crash of one test will then terminate\n"
              << "       the entire test run, making the results unavailable.\n"
              << "  -i, --print-pid\n"
              << "       Print the process ID of each test. This option has\n"
              << "       no effect if fork is not used.\n"
              << "  -h, --help\n"
              << "       This message.\n"
        ;
}

int main(int argc, char** argv, std::auto_ptr<Test> test)
{
    struct option long_options[] = {
        { "help", no_argument, NULL, 'h' },
        { "print-path", no_argument, NULL, 'p' },
        { "dont-use-fork", no_argument, NULL, 'f' },
        { "print-pid", no_argument, NULL, 'i' },

        { 0, 0, 0, 0 }
    };

    bool print_path = false;
    bool use_fork = true;
    bool print_pid = false;
    char opt;
    while ((opt = getopt_long(argc, argv, "pfih", long_options, 0)) != -1) {
        switch (opt) {
            case 'p':
                print_path = true;
                break;
            case 'f':
                use_fork = false;
                break;
            case 'i':
                print_pid = true;
                break;
            case 'h':
            default:
                usage(argv[0]);
                return 1;
        }
    }

    const char* path = (optind < argc)? argv[optind]: NULL;

    Test* run_test = test.get();

    TestSuite root;
    root.add_test(test);

    if (path != NULL) {
        run_test = find(&root, path);
        if (run_test == NULL) {
            std::cerr << "Path not found: " << path << std::endl;
            return 1;
        }
    }
    jf::unittest::TreeWalk tree_walk(std::cout);
    tree_walk.print_path(print_path).print_pid(print_pid).use_fork(use_fork);
    if (tree_walk.do_it(*run_test))
        return 0;
    else
        return 1;
}

}
}
