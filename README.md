This is the repo for the RREFLECT (Ray Rendering Engine for Full Light Effect via Computational Techniques?) the path tracing accelerator. 

The flow is built using mflowgen (https://mflowgen.readthedocs.io/en/latest/quick-start.html) which is a modular flow generator and we used HLS to generate the RTL which we synth'd with design compiler and took through PD in innovus. We use sky 130nm.

The files can all be found in "finalflow" and under each step-- you'll likely find whatever you need/are interested in.

We originally were supposed to do this project for a class at Stanford called EE372 (hence the name of the repo) which was cancelled this last year so we ended up taking independent research units instead.

In best case scenario pre-PD, the accerator processess an esimtated 10 million primatives per second on a 6ns clock. In actuality, when running through PD and closing timing + getting DRC error free, we had to reduce clock to 10ns thanks to the limited time of the quarter. 

The chip was made with the intention of getting multiple chips and created some sort of custom board/soc that consisted of multiple chiplet cores and perhaps running like 8 at a time and splitting up an image into mutliple vertical chunks each core could process.

To generate a scene with 18-ish primatices such as the one shown below, in HD (720p) it would take about 10s according to my estimates. Definitely much work to make up but super cool to have even got it that far!

![Screenshot 2024-06-05 155419](https://github.com/user-attachments/assets/455d3773-c339-4651-aba6-c7107fcaf499)

![IMG_5623](https://github.com/user-attachments/assets/97bb9f28-58ec-4393-9fe5-eb2744415b50)

The archictecure and block diagram of the chip started off something like this as well.

![highres__top](https://github.com/user-attachments/assets/9433d253-55d5-4f3f-aa50-c80724ba1f48)

