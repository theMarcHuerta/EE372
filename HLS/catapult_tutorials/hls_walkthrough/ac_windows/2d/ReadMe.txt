============================================================================
  Project Name: ac_window_2d
============================================================================

  Description:
    Beginner Level Toolkit

    This example shows how to used the 2-d window class using boolean
    flags on the interface. The original design is run as a reference
    against the windowed version and the outputs are verified for
    correctness.

  Scripts:
    directives_3x3.tcl       -- Run with Sobel 3x3 Kernel w/Mirror
    directives_9x5.tcl       -- Run with non-square Sobel 9x5 Kernel w/Mirror
    directives_9x10.tcl      -- Run with even size Sobel 9x10 Kernel w/Mirror
    directives_3x3_clip.tcl  -- Run with Sobel 3x3 Kernel w/Clipping
    directives_9x5_clip.tcl  -- Run with non-square Sobel 9x5 Kernel w/Clipping
    directives_9x10_clip.tcl -- Run with even size Sobel 9x10 Kernel w/Clipping

  Files:
    sobel.h
    sobel_orig.h
    sobel_hls.h
    testbench_3x3.cpp
    testbench_9x5.cpp
    testbench_9x10.cpp
    testbench_3x3_clip.cpp
    testbench_9x5_clip.cpp
    testbench_9x10_clip.cpp

============================================================================
