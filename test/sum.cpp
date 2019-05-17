//  (C) Copyright Eric Niebler 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/weighted_sum.hpp>
#include <boost/accumulators/statistics/variates/covariate.hpp>
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
    accumulator_set<int, stats<tag::sum, tag::sum_of_weights, tag::sum_of_variates<int, tag::covariate1> >, int> acc;

    acc(1, weight = 2, covariate1 = 3);
    BOOST_TEST_EQ(2, sum(acc));
    BOOST_TEST_EQ(2, sum_of_weights(acc));
    BOOST_TEST_EQ(3, sum_of_variates(acc));

    acc(2, weight = 4, covariate1 = 6);
    BOOST_TEST_EQ(10, sum(acc));
    BOOST_TEST_EQ(6, sum_of_weights(acc));
    BOOST_TEST_EQ(9, sum_of_variates(acc));

    acc(3, weight = 6, covariate1 = 9);
    BOOST_TEST_EQ(28, sum(acc));
    BOOST_TEST_EQ(12, sum_of_weights(acc));
    BOOST_TEST_EQ(18, sum_of_variates(acc));
}

///////////////////////////////////////////////////////////////////////////////
// test_persistency
//
void test_persistency()
{
    std::stringstream ss;
    {
        accumulator_set<int, stats<tag::sum, tag::sum_of_weights, tag::sum_of_variates<int, tag::covariate1> >, int> acc;
        acc(1, weight = 2, covariate1 = 3);
        acc(2, weight = 4, covariate1 = 6);
        acc(3, weight = 6, covariate1 = 9);
        BOOST_TEST_EQ(28, sum(acc));
        BOOST_TEST_EQ(12, sum_of_weights(acc));
        BOOST_TEST_EQ(18, sum_of_variates(acc));

        boost::archive::text_oarchive oa(ss);
        acc.serialize(oa, 0);
    }
    accumulator_set<int, stats<tag::sum, tag::sum_of_weights, tag::sum_of_variates<int, tag::covariate1> >, int> acc;
    boost::archive::text_iarchive ia(ss);
    acc.serialize(ia, 0);
    BOOST_TEST_EQ(28, sum(acc));
    BOOST_TEST_EQ(12, sum_of_weights(acc));
    BOOST_TEST_EQ(18, sum_of_variates(acc));
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
