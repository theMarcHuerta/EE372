puts "Catapult OnDemand Training: Setup Module 14"

set sfd [file dirname [info script]]
#source ${sfd}/setup.tcl

# Reset the options to the factory defaults
solution new -state initial
solution options defaults
flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true

# Specify the design files
solution file add ${sfd}/src/vector_mult_tb.cpp -type C++ -exclude true
solution file add ${sfd}/src/vector_mult.cpp -type C++
go analyze 

# Specify Base Technology and RAM Library
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add ccs_sample_mem
go libraries

# Specify the Clock Period
directive set -CLOCKS {clk {-CLOCK_PERIOD 1}}
go assembly 

#########################################
# Enable merging for all four loops 
#########################################
#go assembly
directive set /vector_mult/core -DESIGN_GOAL latency 
directive set /vector_mult/core/VECTOR_MULT -MERGEABLE true 
directive set /vector_mult/core/UNLOAD_RESULT -MERGEABLE true 
directive set /vector_mult/core/VECTOR_MULT -MERGEABLE true 
directive set /vector_mult/core/UNLOAD_RESULT -MERGEABLE true 
go extract 

# Simulate the RTL Design
flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim
