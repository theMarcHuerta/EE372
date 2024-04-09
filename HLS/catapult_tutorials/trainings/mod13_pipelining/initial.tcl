puts "Catapult Online Training: Module 13"
puts "In this lab, you will pipeline the design"
puts "  and examine various stalling modes"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]
source ${sfd}/setup.tcl

# unroll loops in AC moth functions 
go assembly
directive set /StallModes/core -DESIGN_GOAL Latency
directive set /StallModes/core/ac_math::ac_div<1,1,AC_TRN,AC_WRAP,20,10,AC_TRN,AC_WRAP,20,10,AC_RND,AC_SAT>:for -UNROLL yes
directive set /StallModes/core/ac_math::ac_sqrt<20,10,AC_RND,AC_SAT,10,5,AC_RND_CONV_ODD,AC_SAT>:for -UNROLL yes
go extract
