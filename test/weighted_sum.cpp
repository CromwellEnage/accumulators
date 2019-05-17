//  (C) Copyright Eric Niebler, Olivier Gygi 2006.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/weighted_sum.hpp>
#include <boost/accumulators/statistics/variates/covariate.hpp>

using namespace boost;
using namespace accumulators;

///////////////////////////////////////////////////////////////////////////////
// test_stat
//
void test_stat()
{
    accumulator_set<int, stats<tag::weighted_sum, tag::weighted_sum_of_variates<int, tag::covariate1> >, int> acc;

    acc(1, weight = 2, covariate1 = 3);
    BOOST_TEST_EQ(2, weighted_sum(acc));
    BOOST_TEST_EQ(6, weighted_sum_of_variates(acc));

    acc(2, weight = 3, covariate1 = 6);
    BOOST_TEST_EQ(8, weighted_sum(acc));
    BOOST_TEST_EQ(24, weighted_sum_of_variates(acc));

    acc(4, weight = 6, covariate1 = 9);
    BOOST_TEST_EQ(32, weighted_sum(acc));
    BOOST_TEST_EQ(78, weighted_sum_of_variates(acc));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
int main( int argc, char* argv[] )
{
    test_stat();
    return boost::report_errors();
}
