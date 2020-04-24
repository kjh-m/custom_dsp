#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 KH.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#


import numpy as np
from gnuradio import gr
import pmt

class freq_counter_v1(gr.decim_block):
    """
    docstring for block freq_counter_v1
    """
    def __init__(self, vec_len, samp_rate, tune_freq):

        self.vec_len = vec_len
        self.samp_rate = samp_rate
        self.tune_freq = tune_freq

        gr.sync_block.__init__(self,
            name="freq_counter_v1",
            in_sig=[(np.complex64,self.vec_len)],
            out_sig=[np.float32])

        self.first_frame = True

        self.t_fit = np.linspace(0, self.vec_len-1, self.vec_len) / self.samp_rate

        self.rx_freq = 0.0


    def work(self, input_items, output_items):
        in0 = input_items[0]
        out = output_items[0]
        # <+signal processing here+>

        if(self.first_frame):
            self.first_frame = False
            tags = self.get_tags_in_range(0,0,len(in0))
            for tag in tags:
                #print(tag.key, tag.value)
                key = pmt.to_python(tag.key)
                value = pmt.to_python(tag.value)
                if(key == 'rx_freq'):
                    self.rx_freq = value
                    print('rx freq: {:.20f}'.format(self.rx_freq))

        data_in = np.complex128(in0[0])

        phi_uw = np.unwrap( np.angle( data_in ) )

        print('fit data')
        print(phi_uw)
        print('fit data length')
        print(phi_uw.size)

        print('time axis')
        print(self.t_fit)
        print('time axis length')
        print(self.t_fit.size)


        p_fit = np.polynomial.polynomial.Polynomial.fit(self.t_fit, phi_uw, 1)
        p_c = p_fit.convert().coef

        f_phase = p_c[1] / (2.*np.pi)

        f = f_phase + self.tune_freq + self.rx_freq

        out[:] = f_phase
        print('Frequency from phase: {:.8f}'.format(f_phase))
        print('RX freq: {:.18f}'.format(self.rx_freq))
        print('Freq corr: {:.18f}'.format(self.tune_freq))
        print('Actual frequency: {:.18f}'.format(f))
        #print("n. of output items: ", len(output_items))
        return len(output_items[0])

