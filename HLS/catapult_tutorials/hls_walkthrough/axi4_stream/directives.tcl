#------------------------------------------------------------
# AXI4-Stream Example design
#------------------------------------------------------------

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
options defaults

# Start a new project to have the above change take effect
project new

flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true

# Configure C++ testbench options
flow package option set /SCVerify/INVOKE_ARGS 8

# Add input files
solution file add [file join $sfd src crc2_gen.cpp]
solution file add [file join $sfd src crc2_check.cpp]
solution file add [file join $sfd src crc2_top.cpp]
solution file add [file join $sfd src crc2_tb.cpp] -exclude true

go analyze
directive set -DESIGN_HIERARCHY {CRC_Generate CRC_Check CRC_Top}
go compile
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add amba
go libraries
directive set -CLOCKS {clk {-CLOCK_PERIOD 10.0 -CLOCK_EDGE rising -CLOCK_UNCERTAINTY 0.0 -CLOCK_HIGH_TIME 5.0 -RESET_SYNC_NAME rst -RESET_ASYNC_NAME arst_n -RESET_KIND async -RESET_SYNC_ACTIVE low -RESET_ASYNC_ACTIVE low -ENABLE_ACTIVE high}}
go assembly
directive set /CRC_Generate/data_in:rsc -MAP_TO_MODULE ccs_ioport.ccs_in_wait
directive set /CRC_Generate/data_out:rsc -MAP_TO_MODULE amba.ccs_axi4stream_out
go architect
go extract

# Run C++ testbench stand-alone
#flow run /SCVerify/launch_make ./scverify/Verify_orig_cxx_osci.mk {} SIMTOOL=osci build

# Verify CRC_Generate block
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_vhdl_msim.mk {} SIMTOOL=msim sim
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim

go analyze
directive set -DESIGN_HIERARCHY {CRC_Check CRC_Generate CRC_Top}
go compile
go libraries
go assembly
directive set /CRC_Check/data_in:rsc -MAP_TO_MODULE amba.ccs_axi4stream_in
directive set /CRC_Check/data_out:rsc -MAP_TO_MODULE amba.ccs_axi4stream_out
directive set /CRC_Check/error_out:rsc -MAP_TO_MODULE ccs_ioport.ccs_out_wait
go extract

# Verify CRC_Check block
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_vhdl_msim.mk {} SIMTOOL=msim sim
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim

go analyze
directive set -DESIGN_HIERARCHY {CRC_Top CRC_Check CRC_Generate}
go compile
solution library remove *
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add amba
solution library add {[Block] CRC_Check.v1}
solution library add {[Block] CRC_Generate.v1}
go libraries
directive set /CRC_Top/CRC_Generate -MAP_TO_MODULE {[Block] CRC_Generate.v1}
directive set /CRC_Top/CRC_Check -MAP_TO_MODULE {[Block] CRC_Check.v1}
go assembly
directive set /CRC_Top/data_in:rsc -MAP_TO_MODULE ccs_ioport.ccs_in_wait
directive set /CRC_Top/data_out:rsc -MAP_TO_MODULE amba.ccs_axi4stream_out
directive set /CRC_Top/error_out:rsc -MAP_TO_MODULE ccs_ioport.ccs_out_wait
directive set /CRC_Top/buffer:cns -MAP_TO_MODULE amba.ccs_axi4stream_pipe
directive set /CRC_Top/buffer:cns -FIFO_DEPTH 1
go extract

# Verify CRC_Top block
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_vhdl_msim.mk {} SIMTOOL=msim sim
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim

