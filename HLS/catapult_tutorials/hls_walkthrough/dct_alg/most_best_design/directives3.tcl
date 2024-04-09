##
## Copyright 2003-2015 Mentor Graphics Corporation
##
## All Rights Reserved.
##
## THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
## MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
## 

#Map the mult_add functions to CCOREs
project new
# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial 
solution options defaults

# Testbench
flow package require SCVerify
set outfile [utility tmpfile ccs_jpeg_sample.bmp.jpg]
flow package option set /SCVerify/INVOKE_ARGS [list [file join $sfd sample.bmp] $outfile]
flow package option set /SCVerify/USE_CCS_BLOCK true

# Input Files
solution file add [file join $sfd dct.cpp]
solution file add [file join $sfd jpeg.cpp]      -exclude true
solution file add [file join $sfd huff.cpp]      -exclude true
solution file add {$MGC_HOME/shared/include/ccs_bmpfile.cpp} -exclude true
solution file add {$MGC_HOME/shared/include/ccs_jpgfile.cpp} -exclude true
solution file add [file join $sfd main.cpp]      -exclude true
go analyze

solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add ram_nangate-45nm-singleport_beh
directive set -PROTOTYPE_ROM false
directive set -DESIGN_HIERARCHY {dct dct_h dct_v {mult_add<ac_int<8, true>, ac_int<9, true>, ac_int<21, true>>} {mult_add<ac_int<16, true>, ac_int<17, true>, ac_int<31, true>>}}
go compile

directive set -OPT_CONST_MULTS simple_one_adder
go libraries

directive set -CLOCKS {clk {-CLOCK_PERIOD 3.3333 -CLOCK_EDGE rising -CLOCK_UNCERTAINTY 0.0 -CLOCK_HIGH_TIME 1.67 -RESET_SYNC_NAME rst -RESET_ASYNC_NAME arst_n -RESET_KIND sync -RESET_SYNC_ACTIVE high -RESET_ASYNC_ACTIVE low -ENABLE_ACTIVE high}}
directive set /dct/mult_add<ac_int<8,true>,ac_int<9,true>,ac_int<21,true>> -MAP_TO_MODULE {[CCORE]}
directive set /dct/mult_add<ac_int<8,true>,ac_int<9,true>,ac_int<21,true>> -CCORE_TYPE combinational
directive set /dct/mult_add<ac_int<16,true>,ac_int<17,true>,ac_int<31,true>> -MAP_TO_MODULE {[CCORE]}
directive set /dct/mult_add<ac_int<16,true>,ac_int<17,true>,ac_int<31,true>> -CCORE_TYPE combinational
go assembly

directive set /dct/dct_h/core/main -PIPELINE_INIT_INTERVAL 8
directive set /dct/dct_h/core/COPY_ROW0 -UNROLL yes
directive set /dct/dct_h/core/COL0 -UNROLL yes
directive set /dct/dct_v/core/main -PIPELINE_INIT_INTERVAL 8
directive set /dct/dct_v/core/COPY_ROW -UNROLL yes
directive set /dct/dct_v/core/ROW1 -UNROLL yes
go architect
go extract
