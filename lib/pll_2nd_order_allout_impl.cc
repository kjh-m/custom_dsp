/* -*- c++ -*- */
/*
 * Copyright 2020 KH.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//#include <iostream>

#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <gnuradio/sincos.h>
#include "pll_2nd_order_allout_impl.h"

//#include <cmath>

namespace gr {
//namespace analog {
  namespace custom_dsp {

    pll_2nd_order_allout::sptr pll_2nd_order_allout::make(float w, float max_freq, float min_freq, float damp_c, short gain_set, float alpha, float beta, float lock_thr)
    {
      return gnuradio::get_initial_sptr
        (new pll_2nd_order_allout_impl(w, max_freq, min_freq, damp_c, gain_set, alpha, beta, lock_thr));
    }


    /*
     * The private constructor
     */

    static int ios[] = { sizeof(gr_complex), sizeof(gr_complex), sizeof(float), sizeof(short) };
    static std::vector<int> iosig(ios, ios + sizeof(ios) / sizeof(int));

    pll_2nd_order_allout_impl::pll_2nd_order_allout_impl(float w, float max_freq, float min_freq, float damp_c, short gain_set, float alpha, float beta, float lock_thr)
      : gr::sync_block("pll_2nd_order_allout",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::makev(1, 4, iosig)),
	      blocks::control_loop(w, max_freq, min_freq),
	      d_locksig(0),
	      d_lock_threshold(lock_thr)
    {}

    /*
     * Our virtual destructor.
     */
    pll_2nd_order_allout_impl::~pll_2nd_order_allout_impl()
    {
    }

//    int
//    pll_2nd_order_allout_impl::work(int noutput_items,
//        gr_vector_const_void_star &input_items,
//        gr_vector_void_star &output_items)
//    {
//      const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
//     <+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
//      return noutput_items;
//    }


float pll_2nd_order_allout_impl::mod_2pi(float in)
{
    if (in > GR_M_PI){
        //add_item_tag(0, // Port number
        //            nitems_written(0), // Offset
        //            pmt::mp("wrap over"), // Key
        //            pmt::from_double(in) // Value
        //);
        return in - GR_M_TWOPI;}
    else if (in < -GR_M_PI){
        //add_item_tag(0, // Port number
        //            nitems_written(0), // Offset
        //            pmt::mp("wrap under"), // Key
        //            pmt::from_double(in) // Value
        //);
        return in + GR_M_TWOPI;}
    else
        return in;
}

float pll_2nd_order_allout_impl::phase_detector(gr_complex sample, float ref_phase)
{
    float sample_phase;
    sample_phase = gr::fast_atan2f(sample.imag(), sample.real());
    return mod_2pi(sample_phase - ref_phase);
}

int pll_2nd_order_allout_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{
    const gr_complex* iptr = (gr_complex*)input_items[0];
    //gr_complex* optr = (gr_complex*)output_items[0];
    gr_complex* refout_optr = (gr_complex*)output_items[0];
    gr_complex* carrier_optr = output_items.size() >= 2 ? (gr_complex*)output_items[1] : NULL;
    float* error_optr = output_items.size() >= 3 ? (float*)output_items[2] : NULL;
    float* lock_optr = output_items.size() >= 4 ? (float*)output_items[3] : NULL;


    float error;
    float t_imag, t_real;
    gr_complex iq_sample;
    int size = noutput_items;
    
    
    pmt::pmt_t P = pmt::from_long(23);
    
    
    //std::cout<<"test\n";
    //std::cout<<"Get loop bandwidth: "<<get_loop_bandwidth()<<"\n";

    while (size-- > 0) {
    
        //if ( size == 0 ) {
        //    add_item_tag(0, // Port number
        //         nitems_written(0), // Offset
        //         pmt::mp("1st"), // Key
        //         pmt::from_double(size) // Value
        //   );
        //}
    
    
        gr::sincosf(d_phase, &t_imag, &t_real);
        *refout_optr++ = gr_complex(t_real, t_imag);

	    //fast_cc_multiply(*carrier_optr, *iptr, gr_complex(t_real, -t_imag));
        *carrier_optr++ = *iptr * gr_complex(t_real, -t_imag);

        error = phase_detector(*iptr++, d_phase);
	    *error_optr++ = error;
	
    	iq_sample = *iptr;
    	
	    d_locksig = d_locksig * (1.0 - d_alpha) + d_alpha * (iq_sample.real() * t_real + iq_sample.imag() * t_imag);

        //ADD: Test for lock_thr
        *lock_optr++ = short(0);

        advance_loop(error);
        phase_wrap();
        frequency_limit();
    }
    return noutput_items;
}

bool pll_2nd_order_allout_impl::lock_detector(void)
{
    return 0;
}

void pll_2nd_order_allout_impl::set_loop_bandwidth(float bw)
{
    blocks::control_loop::set_loop_bandwidth(bw);
}

void pll_2nd_order_allout_impl::set_damping_factor(float df)
{
    blocks::control_loop::set_damping_factor(df);
}

void pll_2nd_order_allout_impl::set_alpha(float alpha)
{
    blocks::control_loop::set_alpha(alpha);
}

void pll_2nd_order_allout_impl::set_beta(float beta) { blocks::control_loop::set_beta(beta); }

void pll_2nd_order_allout_impl::set_frequency(float freq)
{
    blocks::control_loop::set_frequency(freq);
}

void pll_2nd_order_allout_impl::set_phase(float phase)
{
    blocks::control_loop::set_phase(phase);
}

void pll_2nd_order_allout_impl::set_min_freq(float freq)
{
    blocks::control_loop::set_min_freq(freq);
}

void pll_2nd_order_allout_impl::set_max_freq(float freq)
{
    blocks::control_loop::set_max_freq(freq);
}


float pll_2nd_order_allout_impl::get_loop_bandwidth() const
{
    return blocks::control_loop::get_loop_bandwidth();
}

float pll_2nd_order_allout_impl::get_damping_factor() const
{
    return blocks::control_loop::get_damping_factor();
}

float pll_2nd_order_allout_impl::get_alpha() const { return blocks::control_loop::get_alpha(); }

float pll_2nd_order_allout_impl::get_beta() const { return blocks::control_loop::get_beta(); }

float pll_2nd_order_allout_impl::get_frequency() const
{
    return blocks::control_loop::get_frequency();
}

float pll_2nd_order_allout_impl::get_phase() const { return blocks::control_loop::get_phase(); }

float pll_2nd_order_allout_impl::get_min_freq() const
{
    return blocks::control_loop::get_min_freq();
}

float pll_2nd_order_allout_impl::get_max_freq() const
{
    return blocks::control_loop::get_max_freq();
}

float pll_2nd_order_allout_impl::set_lock_threshold(float threshold)
{
    return d_lock_threshold = threshold;
}

  } /* namespace custom_dsp */
//} // namespace analog
} /* namespace gr */

