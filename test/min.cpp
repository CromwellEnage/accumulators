//  (C) Copyright Eric Niebler 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace boost;
using namespace accumulators;

///////////////////////////////////////////////////////////////////////////////
// test_stat
//
void test_stat()
{
    accumulator_set<int, stats<tag::min> > acc;

    acc(1);
    BOOST_TEST_EQ(1, (min)(acc));

    acc(0);
    BOOST_TEST_EQ(0, (min)(acc));

    acc(2);
    BOOST_TEST_EQ(0, (min)(acc));
}

///////////////////////////////////////////////////////////////////////////////
// test_persistency
//
void test_persistency()
{
    std::stringstream ss;
    {
        accumulator_set<int, stats<tag::min> > acc;
        acc(1);
        acc(0);
        acc(2);
        BOOST_TEST_EQ(0, (min)(acc));
        boost::archive::text_oarchive oa(ss);
        acc.serialize(oa, 0);
    }
    accumulator_set<int, stats<tag::min> > acc;
    boost::archive::text_iarchive ia(ss);
    acc.serialize(ia, 0);
    BOOST_TEST_EQ(0, (min)(acc));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
int main( int argc, char* argv[] )
{
    test_stat();
    test_persistency();
    return boost::report_errors();
}
