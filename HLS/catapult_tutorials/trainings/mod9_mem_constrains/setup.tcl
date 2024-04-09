puts "Catapult OnDemand Training: Setup Module 9"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults

# Setup Flows and Options 
flow package require /SCVerify

# Specify the design files
solution file add ${sfd}/src/memory.cpp
solution file add ${sfd}/src/memory_tb.cpp -exclude true
go analyze 

# Specify 'memory' block as the top-level design
directive set -DESIGN_HIERARCHY {memory}

# Specify Technology Library
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add ram_nangate-45nm-separate_beh
go libraries

# Specify the Clock Period
directive set -CLOCKS {clk {-CLOCK_PERIOD 3}}
go assembly 

