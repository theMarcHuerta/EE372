##
## Copyright 2003-2015 Mentor Graphics Corporation
##
## All Rights Reserved.
##
## THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
## MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
## 

# Synthesize and Verify the Lab1 Design
project new

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
options defaults

# Testbench
flow package require SCVerify

# Input Files
solution file add [file join $sfd tb_fix.cpp] -exclude true
solution file add [file join $sfd avg2_fix.cpp]
go analyze

# Load Technology Library
solution library add mgc_sample-065nm-dw_beh_dc -- -rtlsyntool DesignCompiler -vendor Sample -technology 065nm -Designware Yes
go libraries

# Simulate C Design in OSCII simulator
flow run /SCVerify/launch_make ./scverify/Verify_orig_cxx_osci.mk {} SIMTOOL=osci sim

# Set clock period
directive set -CLOCKS {clk {-CLOCK_PERIOD 10.0 }}
go assembly

directive set /avg2/core/main -PIPELINE_INIT_INTERVAL 0
directive set /avg2/core/ACC -UNROLL no
go extract

# Simulate C vs. RTL
flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim

