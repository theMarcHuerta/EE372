puts "Catapult Online Training: Module 11"
puts "In this lab, you will map various "
puts "  handshaking protocols to the IO "

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
directive set -DESIGN_HIERARCHY fir_filter
go compile

# Load technology libraries
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
go libraries

# Set the clock period to 2.0ns
directive set -CLOCKS {clk {-CLOCK_PERIOD 2.0 }}
go assembly
