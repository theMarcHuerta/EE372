#=========================================================================
# pin-assignments.tcl
#=========================================================================
# The ports of this design become physical pins along the perimeter of the
# design. The commands below will spread the pins along the left and right
# perimeters of the core area. This will work for most designs, but a
# detail-oriented project should customize or replace this section.
#
# Author : Christopher Torng
# Date   : March 26, 2018

#-------------------------------------------------------------------------
# Pin Assignments
#-------------------------------------------------------------------------

# Here pin assignments are done keeping in mind the location of the SRAM pins
# If you update pin assignments below you should rerun the pin-placement step 
# before re-running init step

# We are assigning pins clockwise here, starting from the top side we go left
# to right, then on the right side we go top to bottom, then on the bottom
# side, we go right to left, then on the left side we go bottom to top.

# Pins on the top side. The first pin in this list (here dout1[31]) is on the
# top left and the last pin is on the top right.

set pins_top {\
  {inputChannel_rsc_rdy} {inputChannel_rsc_vld} {inputChannel_rsc_dat[11]} {inputChannel_rsc_dat[10]} {inputChannel_rsc_dat[9]}  {inputChannel_rsc_dat[8]} \
  {inputChannel_rsc_dat[7]}  {inputChannel_rsc_dat[6]}  {inputChannel_rsc_dat[5]}  {inputChannel_rsc_dat[4]}  {inputChannel_rsc_dat[3]}  {inputChannel_rsc_dat[2]} \
  {inputChannel_rsc_dat[1]}  {inputChannel_rsc_dat[0]}   csb1\
}

# Pins on the right side. In this example we are not placing pins on the right
# side, since we haven't routed out the pins on the right side of the SRAM. In
# your design, you can use the right side as well.

set pins_right []

# Pins on the bottom side from right (dout0[0]) to left (din0[31]). I list pins
# out explicitly here because the dout0 and din0 pins on the SRAM macro are
# interleaved somewhat randomly, but if in your case the pins of the same bus
# are to be kept together then you can generate this pin list using a tcl for
# loop.

set pins_bottom {\
  {output_pxl_serial_rsc_dat[0]}  {output_pxl_serial_rsc_dat[1]}  {output_pxl_serial_rsc_dat[2]}\
  {output_pxl_serial_rsc_dat[3]}  {output_pxl_serial_rsc_dat[4]}  {output_pxl_serial_rsc_dat[5]}\
  {output_pxl_serial_rsc_dat[6]}  {output_pxl_serial_rsc_dat[7]}  {output_pxl_serial_rsc_dat[8]}\
  {output_pxl_serial_rsc_dat[9]}  {output_pxl_serial_rsc_dat[10]}  {output_pxl_serial_rsc_dat[11]}\
  {output_pxl_serial_rsc_dat[12]}  {output_pxl_serial_rsc_dat[13]}  {output_pxl_serial_rsc_dat[14]}\
  {output_pxl_serial_rsc_dat[15]}  {output_pxl_serial_rsc_dat[16]}  {output_pxl_serial_rsc_dat[17]}\
  {output_pxl_serial_rsc_dat[18]}  {output_pxl_serial_rsc_dat[19]}  {output_pxl_serial_rsc_dat[20]}\
  {output_pxl_serial_rsc_dat[21]}  {output_pxl_serial_rsc_dat[22]}  {output_pxl_serial_rsc_dat[23]}\
  {output_pxl_serial_rsc_vld}  {output_pxl_serial_rsc_rdy}\

}

# Pins on the left side from bottom (rst_n) to top (addr0[0]).

set pins_left {\
   clk         arst_n
}

# Spread the pins evenly along the sides of the block

#editPin -layer met3 -pin $pins_right  -side RIGHT  -spreadType SIDE
editPin -layer met3 -pin $pins_left   -side LEFT   -spreadType SIDE
editPin -layer met2 -pin $pins_bottom -side BOTTOM -spreadType SIDE
editPin -layer met2 -pin $pins_top    -side TOP    -spreadType SIDE

