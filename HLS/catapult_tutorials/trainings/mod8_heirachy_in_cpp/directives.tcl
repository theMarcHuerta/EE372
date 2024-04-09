puts "Catapult OnDemand Training: Setup Module 7"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults
flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true

# Specify the design files
solution file add ${sfd}/src/decimator.cpp
solution file add ${sfd}/src/fir_filter.cpp
solution file add ${sfd}/src/top.cpp
solution file add ${sfd}/src/top_tb.cpp -exclude true
solution file add ${sfd}/src/csvparser.cpp -exclude true
go analyze 

# Specify 'top' as the top-level design
directive set -DESIGN_HIERARCHY {top fir_filter decimator}

# Specify Technology Library
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
go libraries

# Specify the Clock Period
directive set -CLOCKS {clk {-CLOCK_PERIOD 10}}
go assembly 

# unroll and pipeline the FIR FIlter block
directive set /top/fir_filter/core/main -PIPELINE_INIT_INTERVAL 1
directive set /top/fir_filter/core/main/MAC_LOOP -UNROLL yes
directive set /top/fir_filter/core/main -PIPELINE_STALL_MODE flush

# unroll and pipeline the Decimator block
directive set /top/decimator/core/main -PIPELINE_INIT_INTERVAL 1
directive set /top/decimator/core/main -PIPELINE_STALL_MODE flush

# Generate the RTL
go extract

# Simulate the RTL Design
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim

