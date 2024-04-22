#include <iostream>

#include "TestSphereHit.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    TestSphereHit inst; // DUT

    // initialize arguments
    ray<sfp_11_22> r = {{0, 0, 0}, {1, 0, 0}};  // ray pointing along +x axis
    sfp_11_22 closest_so_far = WS_MAX_X;
    sphere_hittable sphere0 = {{11, 0, 0}, 1, LAMBERTIAN, {255, 255, 255}};   // first sphere
    sphere_hittable sphere1 = {{6, 0, 0}, 1, METAL, {255, 255, 255}};   // second sphere
    sphere_hittable sphere2 = {{18, 0, 0}, 2, LAMBERTIAN, {255, 255, 255}};   // third sphere
    HitRecord<sfp_11_22> rec = {{0, 0, 0}, {0, 0, 0}, false, WS_MIN_X, 0, 0, METAL, {0, 0, 0}};

    inst.run(r, closest_so_far, sphere0, rec);

    // first hit should be at (10, 0, 0)
    if (closest_so_far != 10 || rec.hit_loc.x != 10 || rec.hit_loc.y != 0 || rec.hit_loc.z != 0 || rec.mat != LAMBERTIAN) {
        cout << "Test Failed!   " << endl;
        CCS_RETURN(1);
    }

    // run second hit routine
    inst.run(r, closest_so_far, sphere1, rec);

    // second hit should be at (5, 0, 0) and this should now be the closest
    if (closest_so_far != 5 || rec.hit_loc.x != 5 || rec.hit_loc.y != 0 || rec.hit_loc.z != 0 || rec.mat != METAL) {
        cout << "Test Failed!   " << endl;
        CCS_RETURN(1);
    }

    inst.run(r, closest_so_far, sphere2, rec);

    // third hit is not the closest, so nothing should change
    if (closest_so_far != 5 || rec.hit_loc.x != 5 || rec.hit_loc.y != 0 || rec.hit_loc.z != 0 || rec.mat != METAL) {
        cout << "Test Failed!   " << endl;
        CCS_RETURN(1);
    }
    
    cout << "Test Passed!  " << endl;
    CCS_RETURN(0);
    
}