//  (C) Copyright Gaetano Mendola 2010, Simon West 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/sum_kahan.hpp>
#include <boost/accumulators/statistics/variates/covariate.hpp>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace boost;
using namespace accumulators;

///////////////////////////////////////////////////////////////////////////////
// test_sum_kahan
//
void test_sum_kahan()
{
    accumulator_set<float, stats<tag::sum_kahan> > acc;

    BOOST_TEST_EQ(0.0f, sum_kahan(acc));

    for (size_t i = 0; i < 1e6; ++i) {
        acc(1e-6f);
    }

    BOOST_TEST_EQ(1.0f, sum_kahan(acc));
}

///////////////////////////////////////////////////////////////////////////////
// test_sum_of_weights_kahan
//
void test_sum_of_weights_kahan()
{
    accumulator_set<float, stats<tag::sum_of_weights_kahan>, float > acc;

    BOOST_TEST_EQ(0.0f, sum_of_weights_kahan(acc));

    for (size_t i = 0; i < 1e6; ++i) {
        acc(0, weight = 1e-6f);
    }

    BOOST_TEST_EQ(1.0f, sum_of_weights_kahan(acc));
}

///////////////////////////////////////////////////////////////////////////////
// test_sum_of_variates_kahan
//
void test_sum_of_variates_kahan()
{
    accumulator_set<
        float, 
        stats<tag::sum_of_variates_kahan<float, tag::covariate1> >,
        float
    >
    acc;

    BOOST_TEST_EQ(0.0f, sum_of_variates_kahan(acc));

    for (size_t i = 0; i < 1e6; ++i) {
        acc(0, covariate1 = 1e-6f);
    }

    BOOST_TEST_EQ(1.0f, sum_of_variates_kahan(acc));
}

///////////////////////////////////////////////////////////////////////////////
// test_persistency
//
void test_persistency()
{
    std::stringstream ss;
    {
        accumulator_set<
            float, 
            stats<tag::sum_of_variates_kahan<float, tag::covariate1> >,
            float
        >
        acc;

        BOOST_TEST_EQ(0.0f, sum_of_variates_kahan(acc));

        for (size_t i = 0; i < 1e6; ++i) {
            acc(0, covariate1 = 1e-6f);
        }

        BOOST_TEST_EQ(1.0f, sum_of_variates_kahan(acc));
        boost::archive::text_oarchive oa(ss);
        acc.serialize(oa, 0);
    }
    accumulator_set<
        float, 
        stats<tag::sum_of_variates_kahan<float, tag::covariate1> >,
        float
    >
    acc;
    boost::archive::text_iarchive ia(ss);
    acc.serialize(ia, 0);
    BOOST_TEST_EQ(1.0f, sum_of_variates_kahan(acc));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
int main( int argc, char* argv[] )
{
    test_sum_kahan();
    test_sum_of_weights_kahan();
    test_sum_of_variates_kahan();
    test_persistency();
    return boost::report_errors();
}
