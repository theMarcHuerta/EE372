/*
The BVH structure optimizes the ray tracing process by allowing the algorithm to quickly ignore large sections of 
the scene that a given ray cannot possibly intersect. This is achieved by testing for intersections with bounding 
boxes first, before testing the individual objects within those boxes. The process significantly reduces the number 
of intersection tests needed, especially in complex scenes, by hierarchically organizing objects.

Building the BVH: The bvh_node constructor sorts the objects based on their positions along a randomly chosen axis 
and splits them into two groups, recursively building a binary tree structure. Each node in the tree represents a 
spatial subdivision with its own bounding box.

Intersection Tests: The hit method for a bvh_node first checks if the ray intersects the node's bounding box. If 
not, it can immediately return false, as none of the objects inside can be hit by the ray. If there is an 
intersection, it recursively checks both child nodes, effectively culling large parts of the scene from detailed 
intersection testing.
*/
#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include <algorithm>

// The bvh_node class represents a single node in the BVH tree. Each node stores two child nodes (left and right)
// and a bounding box that encloses the objects contained in both children.
class bvh_node : public hittable {
  public:
    // Constructor that builds the BVH from a hittable_list of objects.
    bvh_node(const hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}

    // Constructor that recursively builds the BVH tree from a subset of objects.
    bvh_node(const std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
        auto objects = src_objects; // Create a modifiable array of the source scene objects

        int axis = random_int(0,2); // Randomly choose an axis to sort the objects along (x, y, or z).
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare; // Comparator function for sorting.

        size_t object_span = end - start; // Number of objects in the range to be processed.

        // Base cases: if there's one object, it becomes both the left and right child.
        // If there are two objects, sort them and assign to left and right respectively.
        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start+1])) {
                left = objects[start];
                right = objects[start+1];
            } else {
                left = objects[start+1];
                right = objects[start];
            }
        } else {
            // For more than two objects, sort them based on the chosen axis and split into two halves.
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span/2;
            left = make_shared<bvh_node>(objects, start, mid); // Recursively build the left subtree.
            right = make_shared<bvh_node>(objects, mid, end);  // Recursively build the right subtree.
        }

        // Calculate the bounding box for this node by merging the bounding boxes of the children.
        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

    // Tests if a ray hits any object within this BVH node's bounding volume.
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (!bbox.hit(r, ray_t))
            return false; // Early exit if the ray doesn't intersect the bounding box of this node.

        bool hit_left = left->hit(r, ray_t, rec);
        // If left child is hit, test the right child but only up to the distance to the closest hit so far.
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        return hit_left || hit_right; // Return true if either child is hit.
    }

    aabb bounding_box() const override { return bbox; } // Returns this node's bounding box.

  private:
    shared_ptr<hittable> left, right; // Pointers to child BVH nodes or objects.
    aabb bbox; // Bounding box enclosing objects in both child nodes.

    // Helper functions for comparing objects based on their bounding box's position along an axis.
    static bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index) {
        return a->bounding_box().axis(axis_index).min < b->bounding_box().axis(axis_index).min;
    }

    static bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 2);
    }
};

#endif
