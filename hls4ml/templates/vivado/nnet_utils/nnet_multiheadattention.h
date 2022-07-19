#ifndef NNET_MHT_H_
#define NNET_MHT_H_

#include "nnet_common.h"
#include "nnet_mult.h"
#include "nnet_dense.h"
#include "hls_stream.h"
#include <math.h>

namespace nnet {

struct multiheadattention_config
{
    // Internal data type definitions
    typedef float bias_t;
    typedef float weight_t;
    typedef float accum_t;  // where this type will be used

    // Layer Sizes
    // static const unsigned n_in = 10;
    // static const unsigned n_out = 10;
    static const unsigned num_heads = 10;
    static const unsigned head_dim_key = 10;
    static const unsigned head_dim_value = 10;
    static const unsigned feature_dim = 10;

    // Resource reuse info  // not sure how to write this part
    static const unsigned io_type = io_parallel;
    static const unsigned strategy = latency; 
    static const unsigned reuse_factor = 1;
    static const bool store_weights_in_bram = false;
    static const unsigned n_zeros = 0;  // where is defined? meaning?
    static const bool use_static = true; // where is defined? meaning?
    
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

template<class data_T, class res_T, typename CONFIG_T>
void multiheadattention(
    data_T    data_q[CONFIG_T::n_in],
    data_T    data_vk[CONFIG_T::n_in],
    res_T     res[CONFIG_T::n_out],
    typename CONFIG_T::weight_t  attention_output_weight[CONFIG_T::xxx*CONFIG_T::xxx],
    typename CONFIG_T::bias_t    attention_output_bias[CONFIG_T::xxx],
    typename CONFIG_T::weight_t  key_weight[CONFIG_T::xxx*CONFIG_T::xxx],
    typename CONFIG_T::bias_t    key_bias[CONFIG_T::xxx],
    typename CONFIG_T::weight_t  query_weight[CONFIG_T::xxx*CONFIG_T::xxx],
    typename CONFIG_T::bias_t    query_bias[CONFIG_T::xxx],
    typename CONFIG_T::weight_t  value_weight[CONFIG_T::xxx*CONFIG_T::xxx],
    typename CONFIG_T::bias_t    value_bias[CONFIG_T::xxx])
{
    #pragma HLS inline
    if (CONFIG_T::strategy == nnet::latency) {
        dense_latency<data_T, res_T, CONFIG_T>(data, res, weights, biases);
    } else {
        dense_resource<data_T, res_T, CONFIG_T>(data, res, weights, biases);
    }
}

}

#endif