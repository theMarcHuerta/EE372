puts "Catapult OnDemand Training: Setup Module 10"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults

# Setup Flows and Options 
flow package require /SCVerify

# Specify the design files
solution file add ${sfd}/src/fifo_reorder_first.cpp
solution file add ${sfd}/src/fifo_reorder_second.cpp
solution file add ${sfd}/src/fifo_reorder.cpp
solution file add ${sfd}/src/packet_reorder_tb.cpp -exclude true
go compile

# Specify Technology Library
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add ram_nangate-45nm_pipe_beh
go libraries

# Specify the Clock Period
directive set -CLOCKS {clk {-CLOCK_PERIOD 2}}
go assembly 

directive set /packet_reorder/fifo_reorder_first/core/main -PIPELINE_INIT_INTERVAL 1
directive set /packet_reorder/fifo_reorder_first/core/main -PIPELINE_STALL_MODE flush

directive set /packet_reorder/fifo_reorder_second/core/main -PIPELINE_INIT_INTERVAL 1
directive set /packet_reorder/fifo_reorder_second/core/main -PIPELINE_STALL_MODE flush

directive set /packet_reorder/first_half_chan:cns -MAP_TO_MODULE ram_nangate-45nm_pipe_beh.mgc_pipe_mem
directive set /packet_reorder/first_half_chan:cns -FIFO_DEPTH 512

go extract
