/*
This class allows you to aggregate multiple objects into a single hittable entity, 
which can then be added to a scene. The hit method iterates over all contained 
objects to find the closest hit object.
*/
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "rtweekend.h"
#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;  // Container for hittable objects.

        hittable_list() {}  // Default constructor.
        hittable_list(shared_ptr<hittable> object) { add(object); }  // Construct with a single object.

        // Clears all objects from the list.
        void clear() { objects.clear(); }

        // Adds a hittable object to the list.
        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        // Overrides hit method to check all objects in the list for the closest hit.
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;  // Tracks the closest hit so far.

            // Iterate over all objects, updating `closest_so_far` with the nearest hit.
            for (const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;  // Update the closest hit distance.
                    rec = temp_rec;  // Update the record to the nearest hit.
                }
            }
            // if (hit_anything){
            // // if (rec.p.x() < 1e-16) {rec.p.e[0] = 0.0;}
            // // if (rec.p.y() < 1e-16) {rec.p.e[1] = 0.0;}
            // // if (rec.p.y() < 1e-16) {rec.p.e[2] = 0.0;}
            // // std::cout << rec.p << "\n";
            // // std::cout << rec.normal << "\n";
            // // std::cout << rec.front_face << "\n";
            // // std::cout << rec.t << "\n";
            // // std::cout << rec.u << "\n";
            // // std::cout << rec.v << "\n";
            // // std::cout << rec.mat->matnum() << "\n";
            // // std::cout << rec.mat->colorofmat() << "\n\n";
            // }
            // else {
            // std::cout << 0 << "\n";
            // std::cout << 0 << "\n";
            // std::cout << 0 << "\n";
            // std::cout << 0 << "\n";
            // std::cout << 0 << "\n";
            // std::cout << 0 << "\n";
            // std::cout << 0 << "\n";
            // std::cout << 0 << "\n\n";
            // }
            // std::cout << 1 << std::endl;

            return hit_anything;  // Returns true if any object was hit.
        }

    private:
};

#endif
