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

#ifndef INCLUDED_CUSTOM_DSP_PLL_2ND_ORDER_ALLOUT_H
#define INCLUDED_CUSTOM_DSP_PLL_2ND_ORDER_ALLOUT_H

//#include <gnuradio/analog/api.h>
#include <custom_dsp/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/blocks/control_loop.h>

namespace gr {
//namespace analog {
  namespace custom_dsp {

    /*!
     * \brief <+description of block+>
     * \ingroup custom_dsp
     *
     */
    class CUSTOM_DSP_API pll_2nd_order_allout : virtual public gr::sync_block,
	                                        virtual public blocks::control_loop
    {
     public:
      typedef boost::shared_ptr<pll_2nd_order_allout> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of custom_dsp::pll_2nd_order_allout.
       *
       * To avoid accidental use of raw pointers, custom_dsp::pll_2nd_order_allout's
       * constructor is in a private implementation
       * class. custom_dsp::pll_2nd_order_allout::make is the public interface for
       * creating new instances.
       */
      static sptr make(float w, float max_freq, float min_freq, float damp_c, short gain_set, float alpha, float beta, float lock_thr);

      virtual bool lock_detector(void) = 0;
      virtual float set_lock_threshold(float) = 0;

      virtual void set_loop_bandwidth(float bw) = 0;
      virtual void set_damping_factor(float df) = 0;
      virtual void set_alpha(float alpha) = 0;
      virtual void set_beta(float beta) = 0;
      virtual void set_frequency(float freq) = 0;
      virtual void set_phase(float phase) = 0;
      virtual void set_min_freq(float freq) = 0;
      virtual void set_max_freq(float freq) = 0;

      virtual float get_loop_bandwidth() const = 0;
      virtual float get_damping_factor() const = 0;
      virtual float get_alpha() const = 0;
      virtual float get_beta() const = 0;
      virtual float get_frequency() const = 0;
      virtual float get_phase() const = 0;
      virtual float get_min_freq() const = 0;
      virtual float get_max_freq() const = 0;


    };

  } // namespace custom_dsp
//} // namespace analog
} // namespace gr

#endif /* INCLUDED_CUSTOM_DSP_PLL_2ND_ORDER_ALLOUT_H */

