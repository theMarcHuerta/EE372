puts "Catapult Online Training: Module 12"
puts "In this lab, you will examine the mapping of interface"
puts "  variables to dfferent handshaking protocols"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]
dofile ${sfd}/setup.tcl

# set stall mode to Flush with II of 1
directive set /mem_intf/core/VECTOR_ADD -PIPELINE_INIT_INTERVAL 1
directive set /mem_intf/core/VECTOR_ADD -PIPELINE_STALL_MODE flush
directive set /mem_intf/core/main -PIPELINE_INIT_INTERVAL 0
directive set /mem_intf/core/main -PIPELINE_STALL_MODE flush

# Map arrays on the interface to memories
directive set /mem_intf/op1:rsc -MAP_TO_MODULE ccs_sample_mem.ccs_ram_sync_singleport
directive set /mem_intf/op2:rsc -MAP_TO_MODULE ccs_sample_mem.ccs_ram_sync_singleport
directive set /mem_intf/result:rsc -MAP_TO_MODULE ccs_sample_mem.ccs_ram_sync_singleport

# Generate the initial Netlist
go extract

# Simulate the RTL Design
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim
