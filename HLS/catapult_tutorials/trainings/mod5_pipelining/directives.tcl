# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# re-produce results from previous Module
source ${sfd}/genModule4.tcl

# Unroll shift register
go assembly
directive set /fir_filter/core/main/SHIFT_LOOP -UNROLL yes
go extract

# Completely unroll MAC loop in new solution
go assembly
directive set /fir_filter/core/main/MAC_LOOP -UNROLL yes
go extract

# Pipeline main design in new solution
go assembly
directive set /fir_filter/core/main -PIPELINE_INIT_INTERVAL 1
go extract

# Partially unroll MAC 64 times in new solution
go assembly
directive set /fir_filter/core/main/MAC_LOOP -UNROLL 64
go extract

# Verify C vs. RTL
#flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim
