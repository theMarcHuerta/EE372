puts "Catapult OnDemand Training: Setup Module 7"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults

# Setup options and flows
flow package require /SCVerify

# Specify the design files
solution file add ${sfd}/src/decimator.cpp
solution file add ${sfd}/src/decimator_tb.cpp -exclude true
go analyze

# Specify Technology Library
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
go libraries

# Specify the Clock Period
directive set -CLOCKS {clk {-CLOCK_PERIOD 10}}
go assembly 

