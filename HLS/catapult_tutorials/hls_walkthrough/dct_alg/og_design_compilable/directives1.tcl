##
## Copyright 2003-2015 Mentor Graphics Corporation
##
## All Rights Reserved.
##
## THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
## MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
## 
#Compile the orginal design
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

# Hierarchy
directive set -DESIGN_HIERARCHY { dct }

go libraries

