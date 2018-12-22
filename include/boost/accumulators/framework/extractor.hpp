///////////////////////////////////////////////////////////////////////////////
// extractor.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_FRAMEWORK_EXTRACTOR_HPP_EAN_28_10_2005
#define BOOST_ACCUMULATORS_FRAMEWORK_EXTRACTOR_HPP_EAN_28_10_2005

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/array/data.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/seq/to_array.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/parameter/binding.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
#include <utility>

    /// INTERNAL ONLY
    ///
#define BOOST_ACCUMULATORS_EXTRACTOR_FUN_PARAM(z, n, _)                                 \
    std::forward<BOOST_PP_CAT(A, n)>(BOOST_PP_CAT(a, n))
#endif

namespace boost { namespace accumulators
{

namespace detail
{
    template<typename AccumulatorSet, typename Feature>
    struct accumulator_set_result
    {
        typedef typename as_feature<Feature>::type feature_type;
        typedef typename mpl::apply<
            typename boost::remove_const<
                typename boost::remove_reference<AccumulatorSet>::type
            >::type
          , feature_type
        >::type::result_type type;
    };

    template<typename Args, typename Feature>
    struct argument_pack_result
      : accumulator_set_result<
            typename boost::remove_reference<
                typename parameter::binding<Args
                    typename boost::remove_const<
                        typename boost::remove_reference<Args>::type
                    >::type
                  , tag::accumulator
                >::type
            >::type
          , Feature
        >
    {
    };

    template<typename A, typename Feature>
    struct extractor_result
      : mpl::eval_if<
            detail::is_accumulator_set<A>
          , accumulator_set_result<A, Feature>
          , argument_pack_result<A, Feature>
        >
    {
    };

    template<typename Feature, typename AccumulatorSet>
    typename extractor_result<AccumulatorSet, Feature>::type
    do_extract(AccumulatorSet const &acc, mpl::true_)
    {
        typedef typename as_feature<Feature>::type feature_type;
        return extract_result<feature_type>(acc);
    }

    template<typename Feature, typename Args>
    typename extractor_result<Args, Feature>::type
    do_extract(Args const &args, mpl::false_)
    {
        typedef typename as_feature<Feature>::type feature_type;
        return find_accumulator<feature_type>(args[accumulator]).result(args);
    }

} // namespace detail


///////////////////////////////////////////////////////////////////////////////
/// Extracts the result associated with Feature from the specified accumulator_set.
template<typename Feature>
struct extractor
{
    typedef extractor<Feature> this_type;

    /// The result meta-function for determining the return type of the extractor
    template<typename F>
    struct result;

    template<typename A1>
    struct result<this_type(A1)>
      : detail::extractor_result<A1, Feature>
    {
    };

    /// Extract the result associated with Feature from the accumulator set
    /// \param acc The accumulator set object from which to extract the result
    template<typename Arg1>
    typename detail::extractor_result<Arg1, Feature>::type
    operator ()(Arg1 const &arg1) const
    {
        // Arg1 could be an accumulator_set or an argument pack containing
        // an accumulator_set. Dispatch accordingly.
        return detail::do_extract<Feature>(arg1, detail::is_accumulator_set<Arg1>());
    }

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    template<typename Arg1>
    typename detail::extractor_result<Arg1, Feature>::type
    operator ()(Arg1 &&arg1) const
    {
        // Arg1 could be an accumulator_set or an argument pack containing
        // an accumulator_set. Dispatch accordingly.
        Arg1 const& arg = arg1;
        return detail::do_extract<Feature>(arg, detail::is_accumulator_set<Arg1>());
    }
#endif

    /// \overload
    ///
    /// \param a1 Optional named parameter to be passed to the accumulator's result() function.
    template<typename AccumulatorSet, typename A1>
    typename detail::extractor_result<AccumulatorSet, Feature>::type
    operator ()(AccumulatorSet const &acc, A1 const &a1) const
    {
        BOOST_MPL_ASSERT((detail::is_accumulator_set<AccumulatorSet>));
        typedef typename as_feature<Feature>::type feature_type;
        return extract_result<feature_type>(acc, a1);
    }

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    template<typename AccumulatorSet, typename A1>
    typename detail::extractor_result<AccumulatorSet, Feature>::type
    operator ()(AccumulatorSet &&acc, A1 &&a1) const
    {
        BOOST_MPL_ASSERT((detail::is_accumulator_set<AccumulatorSet>));
        typedef typename as_feature<Feature>::type feature_type;
        AccumulatorSet const& acc_c = acc;
        return extract_result<feature_type>(acc_c, std::forward<A1>(a1));
    }
#endif

    // ... other overloads generated by Boost.Preprocessor:

    /// INTERNAL ONLY
    ///
#define BOOST_ACCUMULATORS_EXTRACTOR_FUN_OP(z, n, _)                                    \
    template<BOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>                                  \
    struct result<this_type(BOOST_PP_ENUM_PARAMS_Z(z, n, A))>                           \
      : detail::extractor_result<A1, Feature>                                           \
    {};                                                                                 \
    template<                                                                           \
        typename AccumulatorSet                                                         \
        BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, typename A)                               \
    >                                                                                   \
    typename detail::extractor_result<AccumulatorSet, Feature>::type                    \
    operator ()(                                                                        \
        AccumulatorSet const &acc                                                       \
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, n, A, const &a)                       \
    ) const                                                                             \
    {                                                                                   \
        BOOST_MPL_ASSERT((detail::is_accumulator_set<AccumulatorSet>));                 \
        typedef typename as_feature<Feature>::type feature_type;                        \
        return extract_result<feature_type>(acc BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, a));\
    }

    BOOST_PP_REPEAT_FROM_TO(
        2
      , BOOST_PP_INC(BOOST_ACCUMULATORS_MAX_ARGS)
      , BOOST_ACCUMULATORS_EXTRACTOR_FUN_OP
      , _
    )

#undef BOOST_ACCUMULATORS_EXTRACTOR_FUN_OP

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

    /// INTERNAL ONLY
    ///
#define BOOST_ACCUMULATORS_EXTRACTOR_FUN_OP(z, n, _)                                    \
    template<                                                                           \
        typename AccumulatorSet                                                         \
        BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, typename A)                               \
    >                                                                                   \
    typename detail::extractor_result<AccumulatorSet, Feature>::type                    \
    operator ()(                                                                        \
        AccumulatorSet &&acc                                                            \
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, n, A, &&a)                            \
    ) const                                                                             \
    {                                                                                   \
        BOOST_MPL_ASSERT((detail::is_accumulator_set<AccumulatorSet>));                 \
        typedef typename as_feature<Feature>::type feature_type;                        \
        AccumulatorSet const &acc_c = acc;                                              \
        return extract_result<feature_type>(acc_c                                       \
        BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(n, BOOST_ACCUMULATORS_EXTRACTOR_FUN_PARAM, _));\
    }

    BOOST_PP_REPEAT_FROM_TO(
        2
      , BOOST_PP_INC(BOOST_ACCUMULATORS_MAX_ARGS)
      , BOOST_ACCUMULATORS_EXTRACTOR_FUN_OP
      , _
    )

#undef BOOST_ACCUMULATORS_EXTRACTOR_FUN_OP

#endif  // BOOST_NO_CXX11_RVALUE_REFERENCES

    #ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
    /// \overload
    ///
    template<typename AccumulatorSet, typename A1, typename A2, ...>
    typename detail::extractor_result<AccumulatorSet, Feature>::type
    operator ()(AccumulatorSet const &acc, A1 const &a1, A2 const &a2, ...);

    /// \overload
    ///
    template<typename AccumulatorSet, typename A1, typename A2, ...>
    typename detail::extractor_result<AccumulatorSet, Feature>::type
    operator ()(AccumulatorSet &&acc, A1 &&a1, A2 &&a2, ...);
    #endif
};

}} // namespace boost::accumulators

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_ARRAY_REM(Array)                                                         \
    BOOST_PP_TUPLE_REM_CTOR(BOOST_PP_ARRAY_SIZE(Array), BOOST_PP_ARRAY_DATA(Array))

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_SEQ_REM(Seq)                                                             \
    BOOST_ACCUMULATORS_ARRAY_REM(BOOST_PP_SEQ_TO_ARRAY(Seq))

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_ARGS_OP(s, data, elem)                                                   \
    T ## s

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_PARAMS_OP(s, data, elem)                                                 \
    elem T ## s

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_MAKE_FEATURE(Tag, Feature, ParamsSeq)                                    \
    Tag::Feature<                                                                                   \
        BOOST_ACCUMULATORS_SEQ_REM(                                                                 \
            BOOST_PP_SEQ_TRANSFORM(BOOST_ACCUMULATORS_ARGS_OP, ~, ParamsSeq)                        \
        )                                                                                           \
    >

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN_IMPL(z, n, Tag, Feature, ParamsSeq)                 \
    template<                                                                                       \
        BOOST_ACCUMULATORS_SEQ_REM(                                                                 \
            BOOST_PP_SEQ_TRANSFORM(BOOST_ACCUMULATORS_PARAMS_OP, ~, ParamsSeq)                      \
        )                                                                                           \
      , typename Arg1                                                                               \
        BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, typename A)                                           \
    >                                                                                               \
    typename boost::accumulators::detail::extractor_result<                                         \
        Arg1                                                                                        \
      , BOOST_ACCUMULATORS_MAKE_FEATURE(Tag, Feature, ParamsSeq)                                    \
    >::type                                                                                         \
    Feature(Arg1 const &arg1 BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, n, A, const &a) )            \
    {                                                                                               \
        typedef BOOST_ACCUMULATORS_MAKE_FEATURE(Tag, Feature, ParamsSeq) feature_type;              \
        return boost::accumulators::extractor<feature_type>()(                                      \
            arg1 BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, a));                                         \
    }

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN(z, n, _)                                            \
    BOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN_IMPL(                                                   \
        z                                                                                           \
      , n                                                                                           \
      , BOOST_PP_ARRAY_ELEM(0, _)                                                                   \
      , BOOST_PP_ARRAY_ELEM(1, _)                                                                   \
      , BOOST_PP_ARRAY_ELEM(2, _)                                                                   \
    )

#else

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN_AUX(z, n, Tag, Feature, ParamsSeq)                  \
    template<                                                                                       \
        BOOST_ACCUMULATORS_SEQ_REM(                                                                 \
            BOOST_PP_SEQ_TRANSFORM(BOOST_ACCUMULATORS_PARAMS_OP, ~, ParamsSeq)                      \
        )                                                                                           \
      , typename Arg1                                                                               \
        BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, typename A)                                           \
    >                                                                                               \
    typename boost::accumulators::detail::extractor_result<                                         \
        Arg1                                                                                        \
      , BOOST_ACCUMULATORS_MAKE_FEATURE(Tag, Feature, ParamsSeq)                                    \
    >::type                                                                                         \
    Feature(Arg1 &&arg1 BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, n, A, &&a) )                      \
    {                                                                                               \
        typedef BOOST_ACCUMULATORS_MAKE_FEATURE(Tag, Feature, ParamsSeq) feature_type;              \
        Arg1 const &arg1_c = arg1;                                                                  \
        return boost::accumulators::extractor<feature_type>()(arg1_c                                \
        BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(n, BOOST_ACCUMULATORS_EXTRACTOR_FUN_PARAM, _));    \
    }

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN(z, n, _)                                            \
    BOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN_IMPL(                                                   \
        z                                                                                           \
      , n                                                                                           \
      , BOOST_PP_ARRAY_ELEM(0, _)                                                                   \
      , BOOST_PP_ARRAY_ELEM(1, _)                                                                   \
      , BOOST_PP_ARRAY_ELEM(2, _)                                                                   \
    )                                                                                               \
    BOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN_AUX(                                                    \
        z                                                                                           \
      , n                                                                                           \
      , BOOST_PP_ARRAY_ELEM(0, _)                                                                   \
      , BOOST_PP_ARRAY_ELEM(1, _)                                                                   \
      , BOOST_PP_ARRAY_ELEM(2, _)                                                                   \
    )

#endif  // BOOST_NO_CXX11_RVALUE_REFERENCES

#define BOOST_ACCUMULATORS_DEFINE_EXTRACTOR(Tag, Feature, ParamSeq)                                 \
    BOOST_PP_REPEAT(                                                                                \
        BOOST_PP_INC(BOOST_ACCUMULATORS_MAX_ARGS)                                                   \
      , BOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN                                                     \
      , (3, (Tag, Feature, ParamSeq))                                                               \
    )

#endif
