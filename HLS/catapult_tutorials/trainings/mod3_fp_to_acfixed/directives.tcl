puts "Catapult Online Training: Module 3"
puts "In this lab, you will simulate the FIR filter"
puts "  with bit-accurate AC Datatypes"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults

# Setup options and flows
flow package require /SCVerify

# Compile the design
solution file add ${sfd}/src/fir_filter.cpp
solution file add ${sfd}/src/csvparser.cpp -exclude true
solution file add ${sfd}/src/fir_filter_tb.cpp -exclude true
go analyze

directive set -DESIGN_HIERARCHY fir_filter
go compile

# Simulate the C++ design with the testbench
flow run /SCVerify/launch_make ./scverify/Verify_orig_cxx_osci.mk {} SIMTOOL=osci sim
