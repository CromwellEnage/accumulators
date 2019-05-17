//  (C) Copyright Eric Niebler 2008.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_sum.hpp>
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
    accumulator_set<int, stats<tag::rolling_sum> > acc(tag::rolling_window::window_size = 3);

    BOOST_TEST_EQ(0, rolling_sum(acc));

    acc(1);
    BOOST_TEST_EQ(1, rolling_sum(acc));

    acc(2);
    BOOST_TEST_EQ(3, rolling_sum(acc));

    acc(3);
    BOOST_TEST_EQ(6, rolling_sum(acc));

    acc(4);
    BOOST_TEST_EQ(9, rolling_sum(acc));

    acc(5);
    BOOST_TEST_EQ(12, rolling_sum(acc));
}

///////////////////////////////////////////////////////////////////////////////
// test_persistency
//
void test_persistency()
{
    std::stringstream ss;
    {
        accumulator_set<int, stats<tag::rolling_sum> > acc(tag::rolling_window::window_size = 3);
        acc(1);
        acc(2);
        acc(3);
        acc(4);
        acc(5);
        BOOST_TEST_EQ(12, rolling_sum(acc));
        boost::archive::text_oarchive oa(ss);
        acc.serialize(oa, 0);
    }
    accumulator_set<int, stats<tag::rolling_sum> > acc(tag::rolling_window::window_size = 3);
    boost::archive::text_iarchive ia(ss);
    acc.serialize(ia, 0);
    BOOST_TEST_EQ(12, rolling_sum(acc));
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
