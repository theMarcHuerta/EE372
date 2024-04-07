/*
Auhtors: Marc Huerta & Brian Kaether

Stanford University, 2024
__________________________________________________________________________________________
The Ray Generation Unit is responsible for creating the initial rays that are cast from the 
camera into the scene. This involves calculating the direction and origin of each ray based 
on the camera's position, orientation, field of view (FOV), and the dimensions of the image
being rendered. The calculation ensures that each pixel in the output image corresponds to 
a ray cast into the scene.
*/

module ray_gen_unit #(
    parameter integer WIDTH = 32, // Width for fixed-point numbers
    parameter integer IMG_WIDTH = 1280, // Image width for 720p resolution
    parameter integer IMG_HEIGHT = 720 // Image height for 720p resolution
)(
    input wire [WIDTH-1:0] cam_pos_x, cam_pos_y, cam_pos_z, // Camera position
    input wire [10:0] pixel_x, pixel_y, // Current pixel coordinates
    output wire [WIDTH-1:0] ray_origin_x, ray_origin_y, ray_origin_z, // Ray origin
    output wire [WIDTH-1:0] ray_dir_x, ray_dir_y, ray_dir_z // Ray direction
);

    // Instantiate the CORDIC module for trig calculations
    wire signed [WIDTH-1:0] angle_x, angle_y; // Angles for CORDIC
    wire signed [WIDTH-1:0] sine_x, cosine_x, sine_y, cosine_y;

    // Assuming FOV is 90 degrees and aspect ratio is 16:9, calculate angle offsets
    // The screen's horizontal center corresponds to 0 degrees, edges to +/-45 degrees
    // Convert pixel positions to angles. Here we normalize pixel positions to [-0.5, 0.5]
    // then multiply by 90 degrees (or pi/2 radians in fixed-point representation) for the angle span.
    localparam signed [WIDTH-1:0] PI_OVER_TWO = 32'd...; // Fixed-point representation of PI/2
    localparam signed [WIDTH-1:0] PI = 32'd...; // Fixed-point representation of PI

    assign angle_x = ((pixel_x - IMG_WIDTH/2) * PI_OVER_TWO) / (IMG_WIDTH/2);
    assign angle_y = ((pixel_y - IMG_HEIGHT/2) * PI_OVER_TWO) / (IMG_HEIGHT/2);

    // Use CORDIC to calculate sine and cosine for the angles
    cordic cordic_x(.angle(angle_x), .sine(sine_x), .cosine(cosine_x));
    cordic cordic_y(.angle(angle_y), .sine(sine_y), .cosine(cosine_y));

    // Calculate ray direction vectors based on sine and cosine values
    // Note: This simplification assumes the camera faces forward along the -Z axis
    // and does not take into account camera rotation or tilt.
    assign ray_dir_x = cosine_x * sine_y; // Adjusted for actual direction calculation
    assign ray_dir_y = sine_x; // Adjusted for vertical angle
    assign ray_dir_z = cosine_x * cosine_y; // Adjusted for depth direction

    // The ray origin is the camera position
    assign ray_origin_x = cam_pos_x;
    assign ray_origin_y = cam_pos_y;
    assign ray_origin_z = cam_pos_z;

endmodule
