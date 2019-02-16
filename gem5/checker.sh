#!/bin/sh


build/X86/gem5.opt --debug-flags=ExecAll,ProtocolTrace --debug-file=trace configs/cachechecker/checkerruby.py --cpu-type=timing --num-cpus=9 --ruby --caches --mem-size=4GB --cacheline_size=16

#build/ARM/gem5.opt --debug-flags=ExecAll,ProtocolTrace --debug-file=trace.gz configs/cachechecker/checkerruby.py --num-cpus=5 --ruby --caches --mem-size=4GB
