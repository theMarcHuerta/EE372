#------------------------------------------------------------
# sqrt_pwl() test design
#------------------------------------------------------------

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
options defaults

options set Input/CompilerFlags -DTEST_SQRT_AC_COMPLEX_AC_FIXED
options set Input/CppStandard c++11

# Start a new project to have the above change take effect
project new

solution options set Input/CompilerFlags -DTEST_SQRT_AC_COMPLEX_AC_FIXED
solution options set Input/CppStandard c++11

flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true
# Configure C++ testbench options
flow package option set /SCVerify/INVOKE_ARGS {0 1256 4 2.0 1.0}

# Add input files
solution file add [file join $sfd ac_sqrt_pwl_tb.cpp]
go analyze
go compile
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
go libraries
directive set -CLOCKS {clk {-CLOCK_PERIOD 2 -CLOCK_EDGE rising -CLOCK_HIGH_TIME 1 -CLOCK_OFFSET 0.000000 -CLOCK_UNCERTAINTY 0.0 -RESET_KIND sync -RESET_SYNC_NAME rst -RESET_SYNC_ACTIVE high -RESET_ASYNC_NAME arst_n -RESET_ASYNC_ACTIVE low -ENABLE_NAME {} -ENABLE_ACTIVE high}}
go assembly
directive set /project/core/main -PIPELINE_INIT_INTERVAL 1
go allocate
go schedule
go extract
# Verify C++ design with testbench
flow run /SCVerify/launch_make ./scverify/Verify_orig_cxx_osci.mk {} SIMTOOL=osci sim
# Verify RTL