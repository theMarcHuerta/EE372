puts "Catapult Online Training: Module 11"
puts "In this lab, you will examine the mapping of interface"
puts "  variables to dfferent handshaking protocols"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults

# Setup options and flows
flow package require /SCVerify

# Read design source files
solution file add ${sfd}/src/fir_filter.cpp -type C++
solution file add ${sfd}/src/csvparser.cpp -type C++ -exclude true
solution file add ${sfd}/src/fir_filter_tb.cpp -type C++ -exclude true
go analyze 

directive set -DESIGN_HIERARCHY fir_filter
go compile

# Load technology libraries
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
go libraries

# Set the clock period to 2.0ns
directive set -CLOCKS {clk {-CLOCK_PERIOD 2.0 }}
go assembly

# Pipeline the top level design with flushing
directive set /fir_filter/core/main -PIPELINE_STALL_MODE flush
directive set /fir_filter/core/main -PIPELINE_INIT_INTERVAL 1

# Unroll the loops
directive set /fir_filter/core/MAC_LOOP -UNROLL yes
directive set /fir_filter/core/main/SHIFT_LOOP -UNROLL yes

# Specify interfacing on Top Level Ports 
directive set /fir_filter/b:rsc -MAP_TO_MODULE {[DirectInput]}
directive set /fir_filter/i_sample:rsc -MAP_TO_MODULE ccs_ioport.ccs_in_wait
directive set /fir_filter/y:rsc -MAP_TO_MODULE ccs_ioport.ccs_out_wait
go architect

go extract

#Set Pipeline Mode to bubble in new solution
go assembly
directive set /fir_filter/core/main -PIPELINE_STALL_MODE bubble
go extract
