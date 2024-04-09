#------------------------------------------------------------
# Eyeriss PE Array Example
#------------------------------------------------------------


# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
options defaults

go new

flow package require /CDesignChecker
flow run /CDesignChecker
flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true
flow package option set /SCVerify/INVOKE_ARGS {0 2 bridge.bmp}

solution file add [file join $sfd eyeriss.h]
solution file add [file join $sfd testbench.cpp]
solution file add [file join [application get /SYSTEM/ENV_MGC_HOME] shared include bmpUtil bmp_io.cpp] -exclude true
solution file add [file join $sfd sw_conv.h] -exclude true
directive set MEM_MAP_THRESHOLD 512
directive set GATE_REGISTERS false

# Block: linebuffer
go analyze
directive set -DESIGN_HIERARCHY {{lineBuffer<pxlType, ac_int<32, false>, 1024, 3>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {cnnPE<pxlType, 3, 1, 4, 16, 4>} {cnnPE<pxlType, 3, 1, 4, 16, 4>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {cnnPE<pxlType, 3, 1, 4, 16, 4>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {cnnPE<pxlType, 3, 1, 4, 16, 4>}}
go compile
solution library add mgc_Xilinx-ZYNQ-uplus-2_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family ZYNQ-uplus -speed -2 -part xczu9eg-ffvb1156-2-e
solution library add Xilinx_RAMS
go libraries
directive set -CLOCKS {clk {-CLOCK_PERIOD 3.33 -CLOCK_EDGE rising -CLOCK_HIGH_TIME 1.667 -CLOCK_OFFSET 0.000000 -CLOCK_UNCERTAINTY 0.0 -RESET_KIND async -RESET_SYNC_NAME rst -RESET_SYNC_ACTIVE high -RESET_ASYNC_NAME arst_n -RESET_ASYNC_ACTIVE low -ENABLE_NAME {} -ENABLE_ACTIVE high}}
go assembly
directive set /lineBuffer<pxlType,ac_int<32,false>,1024,3>/run/line_buff:rsc -BLOCK_SIZE 1024
go architect
ignore_memory_precedences -from *:write_mem(*)* -to *:read_mem(*)*
go allocate
go extract

# Block: cnnPE
go analyze
directive set -DESIGN_HIERARCHY {{cnnPE<pxlType, 3, 1, 4, 16, 4>} {lineBuffer<pxlType, ac_int<32, false>, 1024, 3>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>}}
go libraries
go assembly
directive set /cnnPE<pxlType,3,1,4,16,4> -EFFORT_LEVEL high
directive set /cnnPE<pxlType,3,1,4,16,4>/run/OutputShiftReg:rsc -MAP_TO_MODULE Xilinx_RAMS.BLOCK_1R1W_RBW
directive set /cnnPE<pxlType,3,1,4,16,4>/run/OutputShiftReg:rsc -GEN_EXTERNAL_ENABLE true
go architect
ignore_memory_precedences -from *write_mem(*)* -to *read_mem(*)*
directive set /cnnPE<pxlType,3,1,4,16,4>/run/MAC-1:mul -MODULE mgc_Xilinx-ZYNQ-uplus-2_beh.mgc_mul_pipe(16,1,16,1,23,1,1,0,1,2,3)
directive set /cnnPE<pxlType,3,1,4,16,4>/run/MAC-2:mul -MODULE mgc_Xilinx-ZYNQ-uplus-2_beh.mgc_mul_pipe(16,1,16,1,23,1,1,0,1,2,3)
directive set /cnnPE<pxlType,3,1,4,16,4>/run/else:mul -MODULE mgc_Xilinx-ZYNQ-uplus-2_beh.mgc_mul_pipe(16,1,16,1,23,1,1,0,1,2,3)
go allocate
go extract

# Block: vld_CNN
go analyze
directive set -DESIGN_HIERARCHY {{vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {cnnPE<pxlType, 3, 1, 4, 16, 4>} {lineBuffer<pxlType, ac_int<32, false>, 1024, 3>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>}}
go compile
go libraries
go assembly
go allocate
go extract

# Block: peArray
go analyze
directive set -DESIGN_HIERARCHY {{peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {cnnPE<pxlType, 3, 1, 4, 16, 4>} {lineBuffer<pxlType, ac_int<32, false>, 1024, 3>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>}}
go compile
solution library remove *
solution library add mgc_Xilinx-ZYNQ-uplus-2_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family ZYNQ-uplus -speed -2 -part xczu9eg-ffvb1156-2-e
solution library add Xilinx_RAMS
solution library add {[CCORE] cnnPE<pxlType,3,1,4,16,4>.v1}
solution library add {[CCORE] lineBuffer<pxlType,ac_int<32,false>,1024,3>.v1}
solution library add {[CCORE] vld_CNN<ac_int<32,false>,3,1,1,4,16>.v1}
go libraries
directive set /peArray<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1>/vld_CNN<ac_int<32,false>,3,1,1,4,16> -MAP_TO_MODULE {[CCORE] vld_CNN<ac_int<32,false>,3,1,1,4,16>.v1}
directive set /peArray<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1>/lineBuffer<pxlType,ac_int<32,false>,1024,3> -MAP_TO_MODULE {[CCORE] lineBuffer<pxlType,ac_int<32,false>,1024,3>.v1}
directive set /peArray<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1>/cnnPE<pxlType,3,1,4,16,4> -MAP_TO_MODULE {[CCORE] cnnPE<pxlType,3,1,4,16,4>.v1}
go assembly
directive set /peArray<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1> -EFFORT_LEVEL high
go architect
go allocate
go extract

# Top Block: imageProcessor
go analyze
directive set -DESIGN_HIERARCHY {{imageProcessor<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1, 0>} {peArray<pxlType, pxlvldType, ac_int<32, false>, 1024, 1024, 3, 1, 1>} {vld_CNN<ac_int<32, false>, 3, 1, 1, 4, 16>} {cnnPE<pxlType, 3, 1, 4, 16, 4>} {lineBuffer<pxlType, ac_int<32, false>, 1024, 3>}}
go compile
solution library remove *
solution library add mgc_Xilinx-ZYNQ-uplus-2_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family ZYNQ-uplus -speed -2 -part xczu9eg-ffvb1156-2-e
solution library add Xilinx_RAMS
solution library add Xilinx_ROMS
solution library add amba
solution library add {[CCORE] peArray<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1>.v1}
go libraries
directive set /imageProcessor<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1,0>/peArray<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1> -MAP_TO_MODULE {[CCORE] peArray<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1>.v1}
go assembly
#directive set /imageProcessor<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1,0>/inCNN:rsc -MAP_TO_MODULE amba.ccs_axi4stream_in
#directive set /imageProcessor<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1,0>/configIn:rsc -MAP_TO_MODULE amba.ccs_axi4stream_in
#directive set /imageProcessor<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1,0>/outCNN:rsc -MAP_TO_MODULE amba.ccs_axi4stream_out
directive set /imageProcessor<pxlType,pxlvldType,ac_int<32,false>,1024,1024,3,1,1,0>/weightIn.rom:rsc -MAP_TO_MODULE {[Register]}
go architect
go allocate
go extract

flow run /SCVerify/launch_make ./scverify/Verify_rtl_v_msim.mk {} SIMTOOL=msim sim
