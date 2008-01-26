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

#include"cleanliness_check.h"

#include <jf/unittest/cleanliness.h>
#include <jf/unittest/test_case.h>
#include <jf/unittest/simple_test_result.h>

namespace {

static bool env_is_clean = true;

class MyCleanTest : public jf::unittest::TestCase
{
public:
    MyCleanTest() : jf::unittest::TestCase("") {}
    virtual void run() {}
};

class MyUncleanTest : public jf::unittest::TestCase
{
public:
    MyUncleanTest() : jf::unittest::TestCase("") {}
    virtual void run() { env_is_clean = false; }
};

class MyCleanlinessCheck : public jf::unittest::CleanlinessCheck
{
public:
    virtual bool environment_is_clean() const { return env_is_clean; }
};

}

namespace jf {
namespace unittest {
namespace tests {

class CleanlinessTest : public jf::unittest::TestCase
{
public:
    CleanlinessTest() : jf::unittest::TestCase("jf::unittest::tests::CleanlinessTest") {}
    virtual void run()
    {
        env_is_clean = true;

        MyCleanTest clean_test;
        MyCleanlinessCheck cleanliness_check;
        SimpleTestResult result;
        clean_test.run_internal(&result, &cleanliness_check);
        JFUNIT_ASSERT(result.ok());
    }
};

class UncleanlinessTest : public jf::unittest::TestCase
{
public:
    UncleanlinessTest() : jf::unittest::TestCase("jf::unittest::tests::UncleanlinessTest") {}
    virtual void run()
    {
        env_is_clean = true;

        MyUncleanTest clean_test;
        MyCleanlinessCheck cleanliness_check;
        SimpleTestResult result;
        clean_test.run_internal(&result, &cleanliness_check);
        JFUNIT_ASSERT(!result.ok());
        JFUNIT_ASSERT(result.unclean());
    }
};

class MixedCleanlinessTest : public jf::unittest::TestCase
{
public:
    MixedCleanlinessTest() : jf::unittest::TestCase("jf::unittest::tests::MixedCleanlinessTest") {}
    virtual void run()
    {
        env_is_clean = true;

        jf::unittest::TestSuite suite("don't care about the name");

        suite.add_test(new MyCleanTest);
        suite.add_test(new MyUncleanTest);
        suite.add_test(new MyCleanTest);
        
        MyCleanlinessCheck cleanliness_check;
        SimpleTestResult result;

        suite.run_internal(&result, &cleanliness_check);

        // of the three tests, the first (clean) and the second
        // (unclean) tests were run. the rest of the suite was
        // abandoned.
        JFUNIT_ASSERT(result.num_tests_run() == 2);
        JFUNIT_ASSERT(result.ok() == false);
        JFUNIT_ASSERT(result.unclean() == true);
    }
};

class RecursiveMixedCleanlinessTest  : public jf::unittest::TestCase
{
public:
    RecursiveMixedCleanlinessTest() : jf::unittest::TestCase("jf::unittest::tests::RecursiveMixedCleanlinessTest") {}
    virtual void run()
    {
        env_is_clean = true;

        jf::unittest::TestSuite top_suite("don't care about the name");

        top_suite.add_test(new MyCleanTest);

        jf::unittest::TestSuite* unclean_suite = new TestSuite("don't care about that name as well");
        unclean_suite->add_test(new MyCleanTest);
        unclean_suite->add_test(new MyUncleanTest);
        unclean_suite->add_test(new MyCleanTest);

        top_suite.add_test(unclean_suite);
        top_suite.add_test(new MyCleanTest);

        
        MyCleanlinessCheck cleanliness_check;
        SimpleTestResult result;

        top_suite.run_internal(&result, &cleanliness_check);

        // first (clean) test in top_suite, first (clean) in
        // unclean_suite, second (unclean) in unclean_suite.
        JFUNIT_ASSERT(result.num_tests_run() == 3);
        JFUNIT_ASSERT(result.ok() == false);
        JFUNIT_ASSERT(result.unclean() == true);
    }
};


CleanlinessCheckSuite::CleanlinessCheckSuite()
: jf::unittest::TestSuite("jf::unittest::tests::CleanlinessCheckSuite")
{
    add_test(new CleanlinessTest);
    add_test(new UncleanlinessTest);
    add_test(new MixedCleanlinessTest);
    add_test(new RecursiveMixedCleanlinessTest);
}

}
}
}
