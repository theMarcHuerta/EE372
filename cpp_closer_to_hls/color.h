/*
uses vec3's three components to represent the red, green, and blue channels of a color.
Additionally, it provides a function to output a color to an output stream, adjusting 
the color based on the number of samples per pixel and applying a gamma correction for 
more accurate color representation.
*/
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"  // Include the vec3 class for mathematical operations on 3D vectors.

#include <iostream>

using color = vec3;  // Defines color as an alias for vec3, representing RGB colors.

// Converts linear RGB component to gamma-corrected component.
inline double linear_to_gamma(double linear_component) {
    return sqrt(linear_component);  // Gamma correction with gamma=2.0.
}

// Outputs a color to an ostream, applying gamma correction and averaging over samples per pixel.
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();


    // std::cout << pixel_color * 255 << std::endl;
    // Average the color by the number of samples to reduce noise (Anti-aliasing).
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply gamma correction (simplified form for gamma=2.0).
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Convert to integer [0,255] and output.
    static const interval intensity(0.000, 0.999);  // Clamps color to prevent overflow.
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif


// 1 ac_int<W,S> exec(ac_int<W,S> data_in,int addr, bool write){
// 2 ac_int<W,S> tmp;
// 3 addr_int = addr;
// 4 if(write){
// 5 if(cnt==0)
// 6 write_data.set_slc(0,data_in);
// 7 else
// 8 write_data.set_slc(W,data_in);
// 9 }
// 10 if(cnt==0){//read on even
// 11 read_data = ram[addr_int>>1];
// 12 }
// 13 else{//write on odd
// 14 if(write){
// 15 ram[addr_int>>1] = write_data;
// 16 }
// 17 }
// 18 if(cnt==0)
// 19 tmp = read_data.template slc<W>(0);
// 20 else
// 21 tmp = read_data.template slc<W>(W);
// 22 ++cnt;
// 23 return tmp;
// 24 }