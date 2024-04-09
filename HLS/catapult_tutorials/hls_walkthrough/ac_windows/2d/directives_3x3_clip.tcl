#------------------------------------------------------------
# 2-d array window Example design
#------------------------------------------------------------

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
options defaults

# Turn off some compiler warnings
options set Flows/OSCI/COMP_FLAGS {-Wall -Wno-unused-local-typedefs -Wno-unknown-pragmas -Wno-unused-label}

# Start a new project to have the above change take effect
project new

# Enable the SCVerify verification flow
flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true

# Add input files
solution file add [file join $sfd testbench_3x3_clip.cpp]

# Load Nangate Library
solution library remove *
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add ram_nangate-45nm-separate_beh

# Set the clock period
directive set -CLOCKS {clk {-CLOCK_PERIOD 3.0 }} 

# Enable transaction done for verification
directive set -TRANSACTION_DONE_SIGNAL true

go compile
directive set /.../core/main/if:for:for -match glob -UNROLL yes
directive set /.../core/main/if:for -match glob -UNROLL yes
directive set /.../core/main -match glob -PIPELINE_INIT_INTERVAL 1

go architect
# ignore_memory_precedences add *write_mem(w* -from *read_mem(w*

go schedule

go extract

# Save the project
project save

# Run the verification flow
#flow run /SCVerify/launch_make ./Verify_cycle_vhdl_msim.mk SIMTOOL=msim sim

flow run /SCVerify/launch_make ./scverify/Verify_orig_cxx_osci.mk {} SIMTOOL=osci sim
flow run /SCVerify/launch_make ./scverify/Verify_rtl_vhdl_msim.mk {} SIMTOOL=msim sim
