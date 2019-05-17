//  (C) Copyright Eric Niebler 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>

using namespace boost;
using namespace accumulators;

namespace my
{
    BOOST_PARAMETER_KEYWORD(tag, int_val)
}

///////////////////////////////////////////////////////////////////////////////
// test_stat
//
void test_stat()
{
    int i = 42;
    accumulator_set<double, stats<tag::value<int, my::tag::int_val> > > acc2(
        my::int_val = i);

    int val1 = value<int, my::tag::int_val>(acc2);
    int val2 = value_tag<my::tag::int_val>(acc2);

    BOOST_TEST_EQ(i, val1);
    BOOST_TEST_EQ(i, val2);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
int main( int argc, char* argv[] )
{
    test_stat();
    return boost::report_errors();
}
