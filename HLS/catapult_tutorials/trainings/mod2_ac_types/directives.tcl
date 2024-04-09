puts "Catapult Online Training: Module 2"
puts "In this lab, you will simulate an example that uses"
puts "  bit-accurate (AC) datatypes"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults

# Setup options and flows
flow package require /SCVerify

# Compile the design
solution file add ${sfd}/src/ac_datatype_example.cpp
go analyze 

directive set -DESIGN_HIERARCHY main
go compile

# Simulate the testbench
flow run /SCVerify/launch_make ./scverify/Verify_orig_cxx_osci.mk {} SIMTOOL=osci sim
