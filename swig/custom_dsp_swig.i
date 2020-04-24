/* -*- c++ -*- */

#define CUSTOM_DSP_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "custom_dsp_swig_doc.i"

%{
#include "custom_dsp/pll_2nd_order_allout.h"
#include "custom_dsp/square_ff.h"
%}


%include "custom_dsp/pll_2nd_order_allout.h"
GR_SWIG_BLOCK_MAGIC2(custom_dsp, pll_2nd_order_allout);
%include "custom_dsp/square_ff.h"
GR_SWIG_BLOCK_MAGIC2(custom_dsp, square_ff);

