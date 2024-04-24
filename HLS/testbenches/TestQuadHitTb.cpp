#include <iostream>

#include "TestQuadHit.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    TestQuadHit inst; // DUT

    // initialize arguments
    ray<sfp_11_22> r = {{0, 0, 0}, {1, 0, 0}};  // ray pointing along +x axis
    sfp_11_22 closest_so_far = WS_MAX_X;
    quad_hittable quad0 = {{10, -0.5, -0.5}, {0, 1, 0}, {0, 0, 1}, LAMBERTIAN, 0, {1, 0, 0}, {1, 0, 0}, 10, {255, 255, 255}};   // first quad
    quad_hittable quad1 = {{5, -0.5, -0.5}, {0, 1, 0}, {0, 0, 1}, METAL_MAX, 0, {1, 0, 0}, {1, 0, 0}, 5, {255, 255, 255}};   // second quad
    quad_hittable quad2 = {{18, -0.5, -0.5}, {0, 1, 0}, {0, 0, 1}, LAMBERTIAN, 0, {1, 0, 0}, {1, 0, 0}, 18, {255, 255, 255}};   // third quad
    HitRecord<sfp_11_22> rec = {{0, 0, 0}, {0, 0, 0}, false, WS_MIN_X, 0, 0, METAL_MAX, {0, 0, 0}};

    inst.run(r, closest_so_far, quad0, rec);

    // first hit should be at (10, 0, 0)
    if (closest_so_far != 10 || rec.hit_loc.x != 10 || rec.hit_loc.y != 0 || rec.hit_loc.z != 0 || rec.mat != LAMBERTIAN) {
        cout << "Test Failed!   " << endl;
        cout << closest_so_far << endl;
        cout << rec.hit_loc.x << endl;
        cout << rec.hit_loc.y << endl;
        cout << rec.hit_loc.z << endl;
        cout << closest_so_far << endl;
        cout << rec.mat;
        CCS_RETURN(1);
    }

    // run second hit routine
    inst.run(r, closest_so_far, quad1, rec);

    // second hit should be at (5, 0, 0) and this should now be the closest
    if (closest_so_far != 5 || rec.hit_loc.x != 5 || rec.hit_loc.y != 0 || rec.hit_loc.z != 0 || rec.mat != METAL_MAX) {
        cout << "Test Failed!   " << endl;
        cout << closest_so_far << endl;
        cout << rec.hit_loc.x << endl;
        cout << rec.hit_loc.y << endl;
        cout << rec.hit_loc.z << endl;
        cout << closest_so_far << endl;
        cout << rec.mat;
        CCS_RETURN(1);
    }

    inst.run(r, closest_so_far, quad2, rec);

    // third hit is not the closest, so nothing should change
    if (closest_so_far != 5 || rec.hit_loc.x != 5 || rec.hit_loc.y != 0 || rec.hit_loc.z != 0 || rec.mat != METAL_MAX) {
        cout << "Test Failed!   " << endl;
        cout << closest_so_far << endl;
        cout << rec.hit_loc.x << endl;
        cout << rec.hit_loc.y << endl;
        cout << rec.hit_loc.z << endl;
        cout << closest_so_far << endl;
        cout << rec.mat;
        CCS_RETURN(1);
    }
    
    cout << "Test Passed!  " << endl;
    CCS_RETURN(0);
    
}