# For cachechecker, with 2-way 512KB L1 and 4-way 1MB L2
# Yangdi

import m5
from m5.objects import *
from m5.defines import buildEnv
from m5.util import addToPath
import os, optparse, sys

addToPath('../common')
addToPath('../ruby')

import Options
import Ruby
import Simulation

# Get paths we might need.  It's expected this file is in m5/configs/example.
config_path = os.path.dirname(os.path.abspath(__file__))
config_root = os.path.dirname(config_path)
m5_root = os.path.dirname(config_root)

# change root dir which contains executable file and input file
rootdir = "/home/yangdi/Documents/CacheChecker/gem5"
parser = optparse.OptionParser()
Options.addCommonOptions(parser)
#
# Add the ruby specific and protocol specific options
#
Ruby.define_options(parser)

execfile(os.path.join(config_root, "common", "Options.py"))

(options, args) = parser.parse_args()

#
# Set the default cache size and associativity to be very small to encourage
# races between requests and writebacks.
#
options.l1d_size="512kB"
options.l1i_size="512kB"
options.l2_size="1MB"
options.l1d_assoc=2
options.l1i_assoc=2
options.l2_assoc=4

if args:
     print "Error: script doesn't take any positional arguments"
     sys.exit(1)

(CPUClass, test_mem_mode, FutureClass) = Simulation.setCPUClass(options)

np = options.num_cpus
system = System(cpu = [CPUClass(cpu_id=i) for i in xrange(np)],
                mem_mode = test_mem_mode,
                mem_ranges = [AddrRange(options.mem_size)],
                cache_line_size = options.cacheline_size)

# Create a top-level voltage domain
system.voltage_domain = VoltageDomain(voltage = options.sys_voltage)

# Create a source clock for the system and set the clock period
system.clk_domain = SrcClockDomain(clock =  options.sys_clock,
                                   voltage_domain = system.voltage_domain)

# Create a CPU voltage domain
system.cpu_voltage_domain = VoltageDomain()

# Create a separate clock domain for the CPUs
system.cpu_clk_domain = SrcClockDomain(clock = options.cpu_clock,
                                       voltage_domain =
                                       system.cpu_voltage_domain)

Ruby.create_system(options, False, system)

# Create a seperate clock domain for Ruby
system.ruby.clk_domain = SrcClockDomain(clock = options.ruby_clock,
                                        voltage_domain = system.voltage_domain)

assert(options.num_cpus == len(system.ruby._cpu_ports))

class Checker(LiveProcess):
    cwd = rootdir
    executable = rootdir + '/cachechecker'
    # with n - 1 cores, as core 0 is used to drive the simulation
    cmd = ['cachechecker', str(options.num_cpus - 1)]
for i in xrange(np):
    ruby_port = system.ruby._cpu_ports[i]
    system.cpu[i].createInterruptController()
    system.cpu[i].icache_port = ruby_port.slave
    system.cpu[i].dcache_port = ruby_port.slave
    if buildEnv['TARGET_ISA'] == 'x86':
        system.cpu[i].interrupts[0].pio = ruby_port.master
        system.cpu[i].interrupts[0].int_master = ruby_port.slave
        system.cpu[i].interrupts[0].int_slave = ruby_port.master
        system.cpu[i].itb.walker.port = ruby_port.slave
        system.cpu[i].dtb.walker.port = ruby_port.slave

# -----------------------
# run simulation
# -----------------------

root = Root( full_system = False, system = system )
root.workload = Checker()
for cpu in system.cpu:
    cpu.workload = root.workload
root.system.mem_mode = 'timing'

# Not much point in this being higher than the L1 latency
m5.ticks.setGlobalFrequency('1ns')

# instantiate configuration
m5.instantiate()

# simulate until program terminates
exit_event = m5.simulate(options.abs_max_tick)

print 'Exiting @ tick', m5.curTick(), 'because', exit_event.getCause()
