/*
Auhtors: Marc Huerta & Brian Kaether

Stanford University, 2024
__________________________________________________________________________________________
This moodule implements the Coordinate Rotation Digital Computer (CORDIC) algorithm to 
compute the sine and cosine of an input angle in radians.
*/
module cordic #(
    parameter integer WIDTH = 32, // Total width for fixed-point numbers
    parameter integer ITERATIONS = 16 // Number of iterations for approximation
)(
    input logic signed [WIDTH-1:0] angle, // Input angle in radians, fixed-point format
    output logic signed [WIDTH-1:0] sine, // Output sine value
    output logic signed [WIDTH-1:0] cosine // Output cosine value
);
    // Fixed-point representation of arctan(2^-i) for i = 0 to ITERATIONS-1
    // These should be precalculated and scaled to the fixed-point format used
    localparam signed [WIDTH-1:0] ANGLES[ITERATIONS-1:0] = '{
        32'd..., // Populate with actual fixed-point values
    };

    // Iterative variables for CORDIC algorithm
    logic signed [WIDTH-1:0] x, y, z;
    logic signed [WIDTH-1:0] x_next, y_next, z_next;

    integer i;

    always_comb begin
        // Initialize vector in the direction (1,0) and input angle
        x = 1 << (WIDTH / 2); // Represents '1' in fixed-point, adjust based on format
        y = 0;
        z = angle;

        // CORDIC rotation loop
        for (i = 0; i < ITERATIONS; i = i + 1) begin
            if (z < 0) begin
                x_next = x + (y >>> i);
                y_next = y - (x >>> i);
                z_next = z + ANGLES[i];
            end else begin
                x_next = x - (y >>> i);
                y_next = y + (x >>> i);
                z_next = z - ANGLES[i];
            end

            x = x_next;
            y = y_next;
            z = z_next;
        end
    end

    // Output the x and y components as cosine and sine of the input angle
    // Note: CORDIC produces a gain that should be compensated for accurate results
    assign cosine = x; // Apply gain correction as needed
    assign sine = y; // Apply gain correction as needed

endmodule
