set blockname [file rootname [file tail [info script] ]]

source scripts/common_no_mod.tcl

directive set -DESIGN_HIERARCHY " 
    {WorldHit} 
"

go compile

source scripts/set_libraries.tcl

go libraries
directive set -CLOCKS $clocks

go assembly
# directive set -EFFORT_LEVEL high

# -------------------------------
# Set the correct word widths and the stage replication
# Your code starts here
# -------------------------------
#return -code error "Remove this once implemented."
# directive set /InputDoubleBuffer<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/InputDoubleBufferReader<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/din -WORD_WIDTH [expr ${ARRAY_DIMENSION} * 8]
# directive set /InputDoubleBuffer<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/InputDoubleBufferWriter<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/dout -WORD_WIDTH [expr ${ARRAY_DIMENSION} * 8]
# directive set /InputDoubleBuffer<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/mem:cns -STAGE_REPLICATION 2
# directive set /InputDoubleBuffer<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/mem -WORD_WIDTH [expr ${ARRAY_DIMENSION} * 8]
# directive set /InputDoubleBuffer<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/InputDoubleBufferReader<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/run/tmp.data.value -WORD_WIDTH [expr ${ARRAY_DIMENSION} * 8]
# directive set /InputDoubleBuffer<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/InputDoubleBufferWriter<4096,${ARRAY_DIMENSION},${ARRAY_DIMENSION}>/run/tmp.data.value -WORD_WIDTH [expr ${ARRAY_DIMENSION} * 8]
# -------------------------------
# Your code ends here
# -------------------------------

go architect

go allocate
go extract