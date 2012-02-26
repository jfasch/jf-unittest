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

#include "find.h"
#include "test_suite.h"

#include <string.h>

namespace jf {
namespace unittest {

using namespace std;

static Test* do_find(const TestSuite* start, const char* path)
{
    char* found_delim = strchr(const_cast<char*>(path), '/');
    if (found_delim == path) {
        const TestSuite* root = start->root();
        return do_find(root, path+1);
    }

    // last path element
    if (!found_delim) {
        // ""
        if (strlen(path) == 0)
            return const_cast<TestSuite*>(start);

        // "name" -> search in current children
        for (TestSuite::Tests::const_iterator i = start->tests().begin();
             i!=start->tests().end();
             ++i)
            if ((*i)->name() == path)
                return *i;
        
        // not found
        return NULL;
    }

    // search goes on. search the current path element, check if it is
    // a TestSuite, and enter recursion.

    *found_delim = 0;
    Test* found = NULL;
    
    for (TestSuite::Tests::const_iterator i = start->tests().begin();
         i!=start->tests().end();
         ++i)
        if ((*i)->name() == path) {
            found = *i;
            break;
        }

    *found_delim = '/';

    if (!found)
        return NULL;

    TestSuite* next = dynamic_cast<TestSuite*>(found);
    if (next == NULL)
        return NULL;
    
    return do_find(next, found_delim+1);
}

Test* find(const TestSuite* start, const string& path)
{
    return do_find(start, path.c_str());
}

}
}
