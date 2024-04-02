/*
Auhtors: Marc Huerta & Brian Kaether

Stanford University, 2024
__________________________________________________________________________________________
This moodule implements basic shading calculations for a ray-traced scene using the
Lambertian reflection model. 

This module performs basic shading calculations for ray-traced scenes, considering the 
direction and intensity of the light source and the surface properties at the intersection 
point. Additionally, it provides a function to check for shadows caused by occluding 
objects in the scene.
*/
module shader #(
    parameter integer WIDTH = 32, // Precision for fixed-point operations
    parameter integer FRACTIONAL_WIDTH = 16, // Fractional bits in fixed-point representation
    parameter integer COLOR_WIDTH = 8 // Width for color channels
)(
    input logic hit, // Flag indicating if the ray hit an object
    input logic signed [WIDTH-1:0] intersection_x, // Intersection point x-coordinate
    input logic signed [WIDTH-1:0] intersection_y, // Intersection point y-coordinate
    input logic signed [WIDTH-1:0] intersection_z, // Intersection point z-coordinate
    input logic signed [WIDTH-1:0] normal_x, // Normal at intersection x-component
    input logic signed [WIDTH-1:0] normal_y, // Normal at intersection y-component
    input logic signed [WIDTH-1:0] normal_z, // Normal at intersection z-component
    input logic signed [WIDTH-1:0] light_dir_x, // Directional light x-direction
    input logic signed [WIDTH-1:0] light_dir_y, // Directional light y-direction
    input logic signed [WIDTH-1:0] light_dir_z, // Directional light z-direction
    input logic signed [WIDTH-1:0] light_intensity, // Light source intensity
    output logic [COLOR_WIDTH-1:0] color_r, // Red channel output
    output logic [COLOR_WIDTH-1:0] color_g, // Green channel output
    output logic [COLOR_WIDTH-1:0] color_b // Blue channel output
);

    // Normalize light direction
    logic signed [WIDTH-1:0] light_dir_norm_x, light_dir_norm_y, light_dir_norm_z;
    logic signed [WIDTH-1:0] light_magnitude;
    logic signed [WIDTH-1:0] dot_product, lambertian_coefficient;

    always_comb begin
        // Normalize the light direction vector
        light_magnitude = sqrt(light_dir_x * light_dir_x + light_dir_y * light_dir_y + light_dir_z * light_dir_z);
        light_dir_norm_x = light_dir_x / light_magnitude;
        light_dir_norm_y = light_dir_y / light_magnitude;
        light_dir_norm_z = light_dir_z / light_magnitude;
        
        // Calculate dot product of light direction and surface normal
        dot_product = (light_dir_norm_x * normal_x + light_dir_norm_y * normal_y + light_dir_norm_z * normal_z);

        // Ensure dot product is not negative for the Lambertian model
        dot_product = (dot_product < 0) ? 0 : dot_product;

        // Lambertian reflection model for diffuse surfaces
        lambertian_coefficient = dot_product * light_intensity;
        
        // Simple shadow check (not implemented here) could adjust lambertian_coefficient based on shadow rays
        
        // Assign color based on Lambertian coefficient, assuming a white light source
        // Scale the intensity to the color width, assuming light_intensity is scaled from 0 to max intensity
        color_r = lambertian_coefficient[WIDTH-1 -: COLOR_WIDTH];
        color_g = color_r;
        color_b = color_r;
    end

    // Note: Implement a sqrt function for fixed-point numbers
    // This is the Newton-Raphson method
    function automatic signed [WIDTH-1:0] sqrt(input signed [WIDTH-1:0] value);
        signed [WIDTH-1:0] x;
        signed [WIDTH-1:0] x_next = value >> 1; // Initial guess
        if (value <= 0) begin
            sqrt = 0;
            return;
        end

        // Newton-Raphson Iteration
        repeat (10) begin // Fixed number of iterations for simplicity
            x = x_next;
            x_next = (x + value / x) >> 1;
        end

        sqrt = x;
    endfunction

    function automatic logic check_shadow(
        input logic signed [WIDTH-1:0] origin_x,  // Ray origin - intersection point
        input logic signed [WIDTH-1:0] origin_y,
        input logic signed [WIDTH-1:0] origin_z,
        input logic signed [WIDTH-1:0] dir_x,     // Direction towards the light
        input logic signed [WIDTH-1:0] dir_y,
        input logic signed [WIDTH-1:0] dir_z,
        input logic [NUM_TRIANGLES*3*WIDTH-1:0] triangles  // Scene geometry
    );
        logic hit;
        logic signed [WIDTH-1:0] t, t_min = 'dMAX_VALUE;  // Use a max value constant appropriate for your system
        logic [NUM_TRIANGLES-1:0] i;

        // Iterate through all triangles to check if the shadow ray intersects any of them
        for (i = 0; i < NUM_TRIANGLES; i = i + 1) begin
            // Extract triangle vertices for the i-th triangle
            logic signed [WIDTH-1:0] v0_x, v0_y, v0_z, v1_x, v1_y, v1_z, v2_x, v2_y, v2_z;
            // Assuming a function or logic to extract vertices from 'triangles' based on 'i'

            ray_triangle_intersection intersection_check (
                .ray_origin_x(origin_x), .ray_origin_y(origin_y), .ray_origin_z(origin_z),
                .ray_dir_x(dir_x), .ray_dir_y(dir_y), .ray_dir_z(dir_z),
                .v0_x(v0_x), .v0_y(v0_y), .v0_z(v0_z),
                .v1_x(v1_x), .v1_y(v1_y), .v1_z(v1_z),
                .v2_x(v2_x), .v2_y(v2_y), .v2_z(v2_z),
                .hit(hit), .t(t)
            );

            // If any triangle is hit by the shadow ray, and the distance is less than to the light source,
            // the point is in shadow
            if (hit && t < t_min) begin
                check_shadow = 1'b1;
                return;
            end
        end
        check_shadow = 1'b0;
    endfunction


endmodule
