============================================================================
  Project Name: Quantization, Pipelining, and Failed Schedule Analysis
============================================================================

  Description:
    Beginner Level Toolkit

    This design takes the channelized design and adds bit-accurate data
    types to reduce area. Following this, loop pipelining constraints are
    added to meet initial performance goals. Higher performance goals
    cause scheduling failures and analysis is used to pinpoint the
    performance bottleneck.

  Documentation:
    Quantization_and_Pipelining.pdf -- Quantization_and_Pipelining.pdf

  Scripts:
    directives1.tcl -- Synthesize the design unconstrained
    directives2.tcl -- Pipeline inner2 with II=1
    directives3.tcl -- Pipeline COL0 and COL1 with II=1
    directives4.tcl -- Unroll inner1 and inner2
    directives5.tcl -- Disable pipelining and analyze Gantt chart

  Files:
    dct.cpp
    jpeg.cpp
    main.cpp
    huff.cpp
    huff.h
    fractal_800x600.bmp
    dct.h
    default.bmp
    sample.bmp
    jpeg.h

============================================================================
