puts "Catapult OnDemand Training: Setup Module 14"

set sfd [file dirname [info script]]
source ${sfd}/setup.tcl

#########################################
# Enable merging of the two loading loops
#########################################
go assembly
directive set /vector_mult/core -DESIGN_GOAL latency 
directive set /vector_mult/core/LOAD_OP1 -MERGEABLE true
directive set /vector_mult/core/LOAD_OP2 -MERGEABLE true
directive set /vector_mult/core/VECTOR_MULT -MERGEABLE false 
directive set /vector_mult/core/UNLOAD_RESULT -MERGEABLE false 
go extract 

# Simulate the RTL Design
flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim
