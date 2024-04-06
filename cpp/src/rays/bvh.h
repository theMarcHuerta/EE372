
#pragma once

#include "../lib/mathlib.h"
// #include "../platform/gl.h"

#include "trace.h"

namespace PT {

template<typename Primitive> class BVH {
public:
    BVH() = default;
    BVH(std::vector<Primitive>&& primitives, size_t max_leaf_size = 1);
    // I modified the signature of this function to add parameters
    void build(std::vector<Primitive>&& primitives, size_t max_leaf_size = 1, size_t start_size = 0, size_t end_size = SIZE_MAX, 
               size_t parent_node_addr = SIZE_MAX, bool left = true);
    void build_bvh(size_t max_leaf_size = 1, size_t start_size = 0, size_t end_size = SIZE_MAX, size_t parent_node_addr = SIZE_MAX, bool left = true, int sort_axis=0);

    BVH(BVH&& src) = default;
    BVH& operator=(BVH&& src) = default;

    BVH(const BVH& src) = delete;
    BVH& operator=(const BVH& src) = delete;

    BBox bbox() const;
    Trace hit(const Ray& ray) const;
    void hit(const Ray& ray, size_t node_idx, Trace& closest) const;

    BVH copy() const;

    std::vector<Primitive> destructure();
    void clear();

private:
    class Node {
        BBox bbox;
        size_t start, size, l, r;

        bool is_leaf() const;
        friend class BVH<Primitive>;
    };
    // custom struct bin that I added
    struct bin {
        BBox bbox = BBox();
        int prim_count = 0;
    };
    size_t new_node(BBox box = {}, size_t start = 0, size_t size = 0, size_t l = 0, size_t r = 0);

    std::vector<Node> nodes;
    std::vector<Primitive> primitives;
    size_t root_idx = 0;
};

} // namespace PT

#ifdef CARDINAL3D_BUILD_REF
#include "../reference/bvh.inl"
#else
#include "../student/bvh.inl"
#endif
