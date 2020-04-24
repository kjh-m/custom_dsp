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

#ifndef INCLUDED_CUSTOM_DSP_SQUARE_FF_IMPL_H
#define INCLUDED_CUSTOM_DSP_SQUARE_FF_IMPL_H

#include <custom_dsp/square_ff.h>

namespace gr {
  namespace custom_dsp {

    class square_ff_impl : public square_ff
    {
     private:
      float d_scale;

     public:
      square_ff_impl(float scale);
      ~square_ff_impl();

      void set_scale(float scale);
      float get_scale();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace custom_dsp
} // namespace gr

#endif /* INCLUDED_CUSTOM_DSP_SQUARE_FF_IMPL_H */

