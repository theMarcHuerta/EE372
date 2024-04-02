/*
Auhtors: Marc Huerta & Brian Kaether

Stanford University, 2024
__________________________________________________________________________________________
This moodule implements the Möller-Trumbore algorithm to determine whether a given ray 
intersects with a triangle in 3D space and calculates the distance to the intersection 
point if it exists.
*/

module ray_triangle_intersection #(
    parameter integer WIDTH = 32,  // Width for fixed-point numbers
    parameter integer FRACTIONAL_WIDTH = 16  // Number of fractional bits in fixed-point representation
)(
    input logic signed [WIDTH-1:0] ray_origin_x, ray_origin_y, ray_origin_z,
    input logic signed [WIDTH-1:0] ray_dir_x, ray_dir_y, ray_dir_z,
    input logic signed [WIDTH-1:0] v0_x, v0_y, v0_z,  // Triangle vertex 0
    input logic signed [WIDTH-1:0] v1_x, v1_y, v1_z,  // Triangle vertex 1
    input logic signed [WIDTH-1:0] v2_x, v2_y, v2_z,  // Triangle vertex 2
    output logic hit,  // Intersection flag
    output logic signed [WIDTH-1:0] t  // Distance to intersection
);

    // Constants for calculation
    localparam signed [WIDTH-1:0] EPSILON = {1'b0, {(WIDTH-1){1'b0}}} + 1;  // Smallest positive value

    // Intermediate variables for the Möller-Trumbore algorithm
    logic signed [WIDTH-1:0] edge1_x, edge1_y, edge1_z;
    logic signed [WIDTH-1:0] edge2_x, edge2_y, edge2_z;
    logic signed [WIDTH-1:0] h_x, h_y, h_z;
    logic signed [WIDTH-1:0] s_x, s_y, s_z;
    logic signed [WIDTH-1:0] q_x, q_y, q_z;
    logic signed [WIDTH-1:0] a, f, u, v;

    always_comb begin
        // Calculate edges of the triangle
        edge1_x = v1_x - v0_x;
        edge1_y = v1_y - v0_y;
        edge1_z = v1_z - v0_z;

        edge2_x = v2_x - v0_x;
        edge2_y = v2_y - v0_y;
        edge2_z = v2_z - v0_z;

        // Begin Möller-Trumbore intersection calculation
        h_x = ray_dir_y * edge2_z - ray_dir_z * edge2_y;
        h_y = ray_dir_z * edge2_x - ray_dir_x * edge2_z;
        h_z = ray_dir_x * edge2_y - ray_dir_y * edge2_x;

        a = edge1_x * h_x + edge1_y * h_y + edge1_z * h_z;

        // Check if the ray is parallel to the triangle
        if (a > -EPSILON && a < EPSILON) begin
            hit = 1'b0;
            t = 0;
        end else begin
            f = 'd1 / a;
            s_x = ray_origin_x - v0_x;
            s_y = ray_origin_y - v0_y;
            s_z = ray_origin_z - v0_z;

            u = f * (s_x * h_x + s_y * h_y + s_z * h_z);

            if (u < 0 || u > 1) begin
                hit = 1'b0;
                t = 0;
            end else begin
                q_x = s_y * edge1_z - s_z * edge1_y;
                q_y = s_z * edge1_x - s_x * edge1_z;
                q_z = s_x * edge1_y - s_y * edge1_x;

                v = f * (ray_dir_x * q_x + ray_dir_y * q_y + ray_dir_z * q_z);

                if (v < 0 || u + v > 1) begin
                    hit = 1'b0;
                    t = 0;
                end else begin
                    t = f * (edge2_x * q_x + edge2_y * q_y + edge2_z * q_z);
                    hit = 1'b1;
                end
            end
        end
    end
endmodule
