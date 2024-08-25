This is the repo for the RREFLECT (Ray Rendering Engine for Full Light Effect via Computational Techniques?) the path tracing accelerator. 

The flow is built using mflowgen (https://mflowgen.readthedocs.io/en/latest/quick-start.html) which is a modular flow generator and we used HLS to generate the RTL which we synth'd with design compiler and took through PD in innovus. We use sky 130nm.

The files can all be found in "finalflow" and under each step-- you'll likely find whatever you need/are interested in.

We originally were supposed to do this project for a class at Stanford called EE372 which was cancelled this last year so we ended up taking independent research units instead.

![Screenshot 2024-06-05 155419](https://github.com/user-attachments/assets/455d3773-c339-4651-aba6-c7107fcaf499)
