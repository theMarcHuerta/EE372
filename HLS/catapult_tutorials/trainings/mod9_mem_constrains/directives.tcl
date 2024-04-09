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
#solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
#solution library add ram_nangate-45nm-separate_beh
solution library add mgc_Xilinx-VIRTEX-u-2_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family VIRTEX-u -speed -2 -part xcvu065-ffvc1517-2-e
solution library add Xilinx_RAMS
go libraries

# Specify the Clock Period
directive set -CLOCKS {clk {-CLOCK_PERIOD 10}}
go assembly 

#Set the Architectural Constraints
directive set /memory/core/main -PIPELINE_INIT_INTERVAL 3
directive set /memory/core/main -PIPELINE_STALL_MODE flush
directive set /memory/core/mem:rsc -MAP_TO_MODULE Xilinx_RAMS.BLOCK_SPRAM_RBW
go architect

# Generate the RTL Netlist
go extract

#Specify Memory Block Size of 512 in new solution
go assembly
directive set /memory/core/main -PIPELINE_INIT_INTERVAL 1
directive set /memory/core/mem:rsc -BLOCK_SIZE 512
go extract

#Set Interleave of 4 in new solution
go assembly
directive set /memory/core/mem:rsc -INTERLEAVE 4
directive set /memory/core/mem:rsc -BLOCK_SIZE 128
directive set /memory/core/EXE_LOOP -UNROLL 4
go extract

#Use a Dualport Memory Resource in new solution
go assembly
directive set /memory/core/mem:rsc -MAP_TO_MODULE Xilinx_RAMS.BLOCK_DPRAM_RBW_DUAL
directive set /memory/core/EXE_LOOP -UNROLL 8
go extract

flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim
