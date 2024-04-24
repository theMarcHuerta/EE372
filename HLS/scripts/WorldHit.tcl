set blockname [file rootname [file tail [info script] ]]

source scripts/common_no_mod.tcl

set type1 "ac_fixed<33, 11, true>"
set type2 "ac_fixed<23, 1, false>"

directive set -DESIGN_HIERARCHY " 
    {WorldHit<${type1}, ${type2}>} 
"

go compile

source scripts/set_libraries.tcl

go libraries
directive set -CLOCKS $clocks

go assembly

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