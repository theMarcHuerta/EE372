##
## Copyright 2003-2015 Mentor Graphics Corporation
##
## All Rights Reserved.
##
## THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
## MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
## 

set pipetype pipe
foreach inst [lsort [find instances -recursive /scverify_top/rtl/*cns_$pipetype]] {
  #echo "DBG: $inst"
  set inst_path [string trim [lindex [split $inst "("] 0]]
  set leaf [lindex [split $inst /] end]
  set pattern "(.*)_cns_${pipetype}"
  regexp $pattern $leaf all chan_name

  ## Creating virtual AND of handshake signals (built in the pipe instance scope)
  set cmd "virtual signal -install $inst_path {$inst_path/ldin AND "
  append cmd "NOT $inst_path/vdin} read_stall"
  eval $cmd
  add wave $inst_path/read_stall
  set cmd "virtual signal -install $inst_path {$inst_path/ldout AND "
  append cmd "NOT $inst_path/vdout} write_stall"
  eval $cmd
  add wave $inst_path/write_stall

  #### This was meant to add the new virtual signal to the channel already in the wave, but it's different for newer versions so I'll have figure out how to do it. In the flow scripts it will be easier.
  #set cmd "quietly virtual signal -install $inst_path {(concat_noflatten)"
  #append cmd "(context $inst_path) "
  #append cmd "($siglist)} "
  #append cmd "$chan_name"
  #eval $cmd
  #echo "add wave $inst_path/$chan_name"
  #add wave $inst_path/$chan_name
}

