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

# Take all ports and split into halves

set all_ports       [dbGet top.terms.name -v *clk*]

set num_ports       [llength $all_ports]
set half_ports_idx  [expr $num_ports / 2]

set pins_left_half  [lrange $all_ports 0               [expr $half_ports_idx - 1]]
set pins_right_half [lrange $all_ports $half_ports_idx [expr $num_ports - 1]     ]

# Take all clock ports and place them center-left

set clock_ports     [dbGet top.terms.name *clk*]
set half_left_idx   [expr [llength $pins_left_half] / 2]

if { $clock_ports != 0 } {
  for {set i 0} {$i < [llength $clock_ports]} {incr i} {
    set pins_left_half \
      [linsert $pins_left_half $half_left_idx [lindex $clock_ports $i]]
  }
}

# Spread the pins evenly across the left and right sides of the block

set ports_layer M4

editPin -layer $ports_layer -pin $pins_left_half  -side LEFT  -spreadType SIDE
editPin -layer $ports_layer -pin $pins_right_half -side RIGHT -spreadType SIDE



# #=========================================================================
# # pin-assignments.tcl
# #=========================================================================
# # The ports of this design become physical pins along the perimeter of the
# # design. The commands below will spread the pins along the left and right
# # perimeters of the core area. This will work for most designs, but a
# # detail-oriented project should customize or replace this section.
# #
# # Author : Christopher Torng
# # Date   : March 26, 2018

# #-------------------------------------------------------------------------
# # Pin Assignments
# #-------------------------------------------------------------------------
# set pins_top {\
#   {inputChannel_rsc_rdy} {inputChannel_rsc_vld} {inputChannel_rsc_dat[11]} {inputChannel_rsc_dat[10]} {inputChannel_rsc_dat[9]}  {inputChannel_rsc_dat[8]} \
#   {inputChannel_rsc_dat[7]}  {inputChannel_rsc_dat[6]}  {inputChannel_rsc_dat[5]}  {inputChannel_rsc_dat[4]}  {inputChannel_rsc_dat[3]}  {inputChannel_rsc_dat[2]} \
#   {inputChannel_rsc_dat[1]}  {inputChannel_rsc_dat[0]}   csb1\
# }

# # Pins on the right side. In this example we are not placing pins on the right
# # side, since we haven't routed out the pins on the right side of the SRAM. In
# # your design, you can use the right side as well.

# set pins_right []

# # Pins on the bottom side from right (dout0[0]) to left (din0[31]). I list pins
# # out explicitly here because the dout0 and din0 pins on the SRAM macro are
# # interleaved somewhat randomly, but if in your case the pins of the same bus
# # are to be kept together then you can generate this pin list using a tcl for
# # loop.

# set pins_bottom {\
#   {output_pxl_serial_rsc_dat[0]}  {output_pxl_serial_rsc_dat[1]}  {output_pxl_serial_rsc_dat[2]}\
#   {output_pxl_serial_rsc_dat[3]}  {output_pxl_serial_rsc_dat[4]}  {output_pxl_serial_rsc_dat[5]}\
#   {output_pxl_serial_rsc_dat[6]}  {output_pxl_serial_rsc_dat[7]}  {output_pxl_serial_rsc_dat[8]}\
#   {output_pxl_serial_rsc_dat[9]}  {output_pxl_serial_rsc_dat[10]}  {output_pxl_serial_rsc_dat[11]}\
#   {output_pxl_serial_rsc_dat[12]}  {output_pxl_serial_rsc_dat[13]}  {output_pxl_serial_rsc_dat[14]}\
#   {output_pxl_serial_rsc_dat[15]}  {output_pxl_serial_rsc_dat[16]}  {output_pxl_serial_rsc_dat[17]}\
#   {output_pxl_serial_rsc_dat[18]}  {output_pxl_serial_rsc_dat[19]}  {output_pxl_serial_rsc_dat[20]}\
#   {output_pxl_serial_rsc_dat[21]}  {output_pxl_serial_rsc_dat[22]}  {output_pxl_serial_rsc_dat[23]}\
#   {output_pxl_serial_rsc_vld}  {output_pxl_serial_rsc_rdy}\

# }

# # Pins on the left side from bottom (rst_n) to top (addr0[0]).

# set pins_left {\
#    clk         arst_n
# }

# # Spread the pins evenly along the sides of the block

# #editPin -layer met3 -pin $pins_right  -side RIGHT  -spreadType SIDE
# editPin -layer met3 -pin $pins_left   -side LEFT   -spreadType SIDE
# editPin -layer met2 -pin $pins_bottom -side BOTTOM -spreadType SIDE
# editPin -layer met2 -pin $pins_top    -side TOP    -spreadType SIDE


# # # Take all ports and split into halves

# # set all_ports       [dbGet top.terms.name -v *clk*]

# # set num_ports       [llength $all_ports]
# # set half_ports_idx  [expr $num_ports / 2]

# # set pins_left_half  [lrange $all_ports 0               [expr $half_ports_idx - 1]]
# # set pins_right_half [lrange $all_ports $half_ports_idx [expr $num_ports - 1]     ]

# # # Take all clock ports and place them center-left

# # set clock_ports     [dbGet top.terms.name *clk*]
# # set half_left_idx   [expr [llength $pins_left_half] / 2]

# # if { $clock_ports != 0 } {
# #   for {set i 0} {$i < [llength $clock_ports]} {incr i} {
# #     set pins_left_half \
# #       [linsert $pins_left_half $half_left_idx [lindex $clock_ports $i]]
# #   }
# # }

# # # Spread the pins evenly across the left and right sides of the block

# # set ports_layer M4

# # editPin -layer $ports_layer -pin $pins_left_half  -side LEFT  -spreadType SIDE
# # editPin -layer $ports_layer -pin $pins_right_half -side RIGHT -spreadType SIDE
