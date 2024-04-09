##
## Copyright 2003-2015 Mentor Graphics Corporation
##
## All Rights Reserved.
##
## THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
## MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
## 

# Synthesize the Lab2 design
project new

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
options defaults

# Testbench
flow package require SCVerify

# Input Files
solution file add [file join $sfd transpose_filter.cpp] -type C++
solution file add [file join $sfd tb.cpp] -type C++ -exclude true
go compile 

# Load Technology Libraries
solution library add mgc_sample-065nm-dw_beh_dc -- -rtlsyntool DesignCompiler -vendor Sample -technology 065nm -Designware Yes
solution library add ram_sample-065nm-separate_beh_dc
directive set /transpose_and_filter/clip_window -CCORE_TYPE combinational
go libraries

# Simulate C design in OSCI simulator
flow run /SCVerify/launch_make ./scverify/Verify_orig_cxx_osci.mk {} SIMTOOL=osci sim

directive set -CLOCKS {clk {-CLOCK_PERIOD 10.0 }}
go assembly

directive set /transpose_and_filter/filter/core/SHIFT -UNROLL yes
directive set /transpose_and_filter/transpose/core/ROW -PIPELINE_INIT_INTERVAL 1
directive set /transpose_and_filter/transpose/core/main -PIPELINE_INIT_INTERVAL 0
directive set /transpose_and_filter/filter/core/ROW -PIPELINE_INIT_INTERVAL 1
go extract

# Simulate C vs RTL
flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim
