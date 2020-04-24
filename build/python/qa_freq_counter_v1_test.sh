#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir="/home/mikes/gnuradio/gr-custom_dsp/python"
export GR_CONF_CONTROLPORT_ON=False
export PATH="/home/mikes/gnuradio/gr-custom_dsp/build/python":$PATH
export LD_LIBRARY_PATH="":$LD_LIBRARY_PATH
export PYTHONPATH=/home/mikes/gnuradio/gr-custom_dsp/build/swig:$PYTHONPATH
/usr/bin/python3 /home/mikes/gnuradio/gr-custom_dsp/python/qa_freq_counter_v1.py 
