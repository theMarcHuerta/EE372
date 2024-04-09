#------------------------------------------------------------
# reciprocal_pwl() test design
#------------------------------------------------------------

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
options defaults
##  -DPLOT_RECIPROCAL_FILE_WRITE
options set Input/CompilerFlags {-DTEST_RECIPROCAL_AC_FIXED}
options set Input/CppStandard c++11
options set Clustering/prototype_rom false

# Start a new project to have the above change take effect
project new

solution options set Input/CompilerFlags {-DTEST_RECIPROCAL_AC_FIXED}
solution options set Input/CppStandard c++11
solution options set Clustering/prototype_rom false

flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true

# Configure C++ testbench options
# 1st argument: lower limit, 2nd: upper limit, 3rd: increment, 4th: tolerance in %
flow package option set /SCVerify/INVOKE_ARGS {-15 15 - 1.00}

# enable matlab/simulink interfacing
set simulinkOut ./simulink
flow package require /Matlab
flow package option set /Matlab/MATLAB_ROOT {$MATLABROOT}
flow package option set /Matlab/SIMULINK_DIR ${simulinkOut}

# Add input files
solution file add [file join $sfd ac_reciprocal_pwl_tb.cpp]
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

# Build the simulink s-function
set solutionDir [project get /CURRENT/SOLUTION_DIR]
set sfn [ file tail [ glob  ${solutionDir}/sfun_*.cpp ] ]
flow run /Matlab/launch_mex_compile $sfn

# Copy out the simulink model file for use
file copy -force ${sfd}/ac_reciprocal.mdl ${simulinkOut}

# Verify C++ design with testbench
flow run /SCVerify/launch_make ./scverify/Verify_orig_cxx_osci.mk {} SIMTOOL=osci sim
# Verify RTL
