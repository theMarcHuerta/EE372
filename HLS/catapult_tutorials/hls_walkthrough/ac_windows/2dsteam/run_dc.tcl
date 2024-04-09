##
## Copyright 2003-2015 Mentor Graphics Corporation
##
## All Rights Reserved.
##
## THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
## MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
## 

#------------------------------------------------------------
# 2-d stream window Example design
#------------------------------------------------------------

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
options defaults

# Start a new project to have the above change take effect
project new

# Enable the SCVerify verification flow
flow package require /SCVerify

# Add input files
solution file add [file join $sfd sobel_2d_stream.cpp]
solution file add [file join $sfd sobel_orig.cpp] -exclude true
solution file add [file join $sfd testbench.cpp] -exclude true

# Load Nangate Library
solution library remove *
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add ram_nangate-45nm-separate_beh

# Set the clock period
directive set -CLOCKS {clk {-CLOCK_PERIOD 3.0 }} 

# Enable transaction done for verification
directive set -TRANSACTION_DONE_SIGNAL true
go compile

directive set /sobel/core/main/for:for:if:for:for -UNROLL yes
directive set /sobel/core/main/for:for:if:for -UNROLL yes
directive set /sobel/core/main -PIPELINE_INIT_INTERVAL 1
go architect

ignore_memory_precedences add /sobel/core/main/*read_mem(w* -from /sobel/core/main/*write_mem(w*
ignore_memory_precedences add /sobel/core/main/*write_mem(w* -from /sobel/core/main/*read_mem(w*
ignore_memory_precedences add /sobel/core/main/*write_mem(w* -from /sobel/core/main/*write_mem(w*
go schedule

go extract

# Save the project
project save

# Run the verification flow
#flow run /SCVerify/launch_make ./Verify_cycle_vhdl_msim.mk SIMTOOL=msim sim

