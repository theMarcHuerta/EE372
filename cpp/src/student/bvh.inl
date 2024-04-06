
#include "../rays/bvh.h"
#include <stack>

namespace PT {

template<typename Primitive>
void BVH<Primitive>::build(std::vector<Primitive>&& prims, size_t max_leaf_size, size_t start_size, size_t end_size, size_t parent_node_addr, bool left) {
    primitives = std::move(prims);
    build_bvh(max_leaf_size, start_size, end_size, parent_node_addr, left, 0);
}

// construct BVH hierarchy given a vector of prims
template<typename Primitive>
void BVH<Primitive>::build_bvh(size_t max_leaf_size, size_t start_size, size_t end_size, size_t parent_node_addr, bool left, int sort_axis) {

    // NOTE (PathTracer):
    // This BVH is parameterized on the type of the primitive it contains. This allows
    // us to build a BVH over any type that defines a certain interface. Specifically,
    // we use this to both build a BVH over triangles within each Tri_Mesh, and over
    // a variety of Objects (which might be Tri_Meshes, Spheres, etc.) in Pathtracer.
    //
    // The Primitive interface must implement these two functions:
    //      BBox bbox() const;
    //      Trace hit(const Ray& ray) const;
    // Hence, you may call bbox() and hit() on any value of type Primitive.

    // TODO (PathTracer): Task 3
    // Modify the code ahead to construct a BVH from the given vector of primitives and maximum leaf
    // size configuration.
    //
    // Please use the SAH as described in class.  We recomment the binned build from lecture.
    // In general, here is a rough sketch:
    //
    //  For each axis X,Y,Z:
    //     Try possible splits along axis, evaluate SAH for each
    //  Take minimum cost across all axes.
    //  Partition primitives into a left and right child group
    //  Compute left and right child bboxes
    //  Make the left and right child nodes.
    //
    //
    // While a BVH is conceptually a tree structure, the BVH class uses a single vector (nodes)
    // to store all the nodes. Therefore, BVH nodes don't contain pointers to child nodes,
    // but rather the indices of the
    // child nodes in this array. Hence, to get the child of a node, you have to
    // look up the child index in this vector (e.g. nodes[node.l]). Similarly,
    // to create a new node, don't allocate one yourself - use BVH::new_node, which
    // returns the index of a newly added node.
    //
    // As an example of how to make nodes, the starter code below builds a BVH with a
    // root node that encloses all the primitives and its two descendants at Level 2.
    // For now, the split is hardcoded such that the first primitive is put in the left
    // child of the root, and all the other primitives are in the right child.
    // There are no further descendants.
    
    auto start = primitives.begin();
    auto end = primitives.end();
    size_t start_idx, end_idx;

    bool at_root = (parent_node_addr == SIZE_MAX);

    // if we are at root node, clear nodes and initialize primitives
    if (at_root) {
        nodes.clear();
        start_idx = 0;
        end_idx = primitives.size(); 
    } else {
        start_idx = start_size;
        end_idx = end_size;
        start = primitives.begin() + start_idx;
        end = primitives.begin() + end_idx;
    }

    // edge case
    if(primitives.empty()) {
        return;
    }

    // sort the primitive array
    std::sort(start, end, [sort_axis](const Primitive& a, const Primitive& b) {
        return a.bbox().center()[sort_axis] < b.bbox().center()[sort_axis];
    });

    // number of bins to divide primitives into per axis
    int num_bins = 12;

    // compute bounding box for all primitives in the given array
    // also assign the primitives to bins
    BBox bb;
    for(size_t i = start_idx; i < end_idx; i++) {
        bb.enclose(primitives[i].bbox());
    }

    // calculate bin width for every axis based on number of bins
    Vec3 bin_width = (bb.max - bb.min) / (float)num_bins;

    // set up node for the current bbox
    size_t curr_node_addr = new_node();
    Node& node = nodes[curr_node_addr];
    node.bbox = bb;
    node.start = start_idx;
    node.size = end_idx - start_idx;

    // if we are not at the root, assign parent l or r node
    if (!at_root) {
        if (left) {
            nodes[parent_node_addr].l = curr_node_addr;
        } else {
            nodes[parent_node_addr].r = curr_node_addr;
        }
    }

    // calculate surface area of this box for SAH calculations
    float parent_SA = bb.surface_area();

    // initialize cost value
    float min_cost = INFINITY;
    int min_cost_division = -1;
    BBox best_leftbox;
    BBox best_rightbox;
    int best_left_prim_count = 0;
    int best_right_prim_count = 0;
    int best_axis = -1;

    // loop over each axis
    for (int axis = 0; axis < 3; axis++) {

        // create bins
        std::vector<bin> bins(num_bins);
        
        // iterate over primitives and organize them into bins
        for (size_t i = start_idx; i < end_idx; i++) {
            int bin_idx;
            // make sure we don't divide by 0
            if (bin_width[axis] == 0) {
                bin_idx = num_bins-1;
            } else {
                bin_idx = (int)std::floor((primitives[i].bbox().center()[axis] - bb.min[axis]) / bin_width[axis]);
            }

            if (bin_idx >= num_bins) bin_idx--;
            bins.at(bin_idx).bbox.enclose(primitives[i].bbox());
            bins.at(bin_idx).prim_count++;
        }

        // now that we have primitives divided into bins, evaluate all bin divisions by SAH.
        // in every iteration, split the bins such that i is the starting index of bins in the
        // right node, and bins with index less than i are in the left node
        for (int i = 1; i < bins.size(); i++) {
            // build left box
            BBox split_leftBox;
            int left_prim_count = 0;
            for (int j = 0; j < i; j++) {
                if (!bins.at(j).bbox.empty()) {
                    split_leftBox.enclose(bins.at(j).bbox);
                    left_prim_count += bins.at(j).prim_count;
                }
            }
            float left_SA = split_leftBox.surface_area();

            // build right box
            BBox split_rightBox;
            int right_prim_count = 0;
            for (int j = i; j < bins.size(); j++) {
                if (!bins.at(j).bbox.empty()) {
                    split_rightBox.enclose(bins.at(j).bbox);
                    right_prim_count += bins.at(j).prim_count;
                }
            }
            float right_SA = split_rightBox.surface_area();

            // evaluate SAH for current configuration and update best cost info if necessary
            float curr_cost = ((left_SA / parent_SA)*left_prim_count) + ((right_SA / parent_SA)*right_prim_count);
            if (curr_cost < min_cost) {
                min_cost = curr_cost;
                min_cost_division = i;
                best_leftbox = split_leftBox;
                best_rightbox = split_rightBox;
                best_left_prim_count = left_prim_count;
                best_right_prim_count = right_prim_count;
                best_axis = axis;
            }
        }
    }

    // calculate physical coordinate plane dividers based on bin idx
    Vec3 divide_plane = (bin_width * (float)min_cost_division) + bb.min;

    auto partitionPoint = std::partition(start, end, [best_axis, divide_plane](const Primitive& prim) { return prim.bbox().center()[best_axis] < divide_plane[best_axis];});
    // size_t partitionPoint_index = std::distance(primitives.begin(), partitionPoint);
    size_t partitionPoint_index = start_idx + best_left_prim_count;

    // if our algorithm wants to put all the primitives into the same child node, return
    // also return if the node we just created is within the maximum leaf size
    if ((best_left_prim_count == 0) || (best_right_prim_count == 0) || (node.size <= max_leaf_size)) {
        return;
    }

    // recurse to create child nodes if we get here
    // left node
    build_bvh(max_leaf_size, start_idx, partitionPoint_index, curr_node_addr, true, best_axis); 

    // right node
    build_bvh(max_leaf_size, partitionPoint_index, end_idx, curr_node_addr, false, best_axis);

}

template<typename Primitive>
Trace BVH<Primitive>::hit(const Ray& ray) const {
    Trace closest;
    closest.distance = INFINITY;

    // check for root hit
    Vec2 root_hit(-INFINITY, INFINITY);
    if (nodes[0].bbox.hit(ray, root_hit)) {
        hit(ray, 0, closest);
    }
    return closest;
}

template<typename Primitive>
void BVH<Primitive>::hit(const Ray& ray, size_t node_idx, Trace& closest) const {

    // TODO (PathTracer): Task 3
    // Implement ray - BVH intersection test. A ray intersects
    // with a BVH aggregate if and only if it intersects a primitive in
    // the BVH that is not an aggregate.

    // The starter code simply iterates through all the primitives.
    // Again, remember you can use hit() on any Primitive value.

    // get current node
    Node curr_node = nodes[node_idx];
    
    // Vec2 hits(-INFINITY, INFINITY);
    // bool hit_bbox = curr_node.bbox.hit(ray, hits);

    // // if ray misses bounding box, return
    // if (!hit_bbox) return;

    // // if first bounding box intersection is further than the closest hit distance, return
    // if (hits.x > closest.distance) return;

    // if node is a leaf, iterate over all primitives
    if (curr_node.is_leaf()) {
        for(const Primitive& prim : primitives) {
            Trace hit = prim.hit(ray);
            closest = Trace::min(closest, hit);
        }
    // if node is not a leaf
    } else {
        // get left and right nodes
        Node l_node = nodes[curr_node.l];
        Node r_node = nodes[curr_node.r];

        // declare hit time Vecs for left and right nodes
        Vec2 left_hit_times(-INFINITY, INFINITY);
        Vec2 right_hit_times(-INFINITY, INFINITY);

        // hit times for first and second children
        Vec2 first_hit_times(-INFINITY, INFINITY);
        Vec2 second_hit_times(-INFINITY, INFINITY);

        size_t first_idx, second_idx;
        bool first_hit, second_hit;

        // does the ray hit the child nodes?
        bool left_hit = l_node.bbox.hit(ray, left_hit_times);
        bool right_hit = r_node.bbox.hit(ray, right_hit_times);

        // set first and second values to left or right according to which is closer
        if (left_hit_times[0] <= right_hit_times[0]) {
            first_idx = curr_node.l;
            second_idx = curr_node.r;
            first_hit = left_hit;
            second_hit = right_hit;
            first_hit_times = left_hit_times;
            second_hit_times = right_hit_times;
        } else {
            first_idx = curr_node.r;
            second_idx = curr_node.l;
            first_hit = right_hit;
            second_hit = left_hit;
            first_hit_times = right_hit_times;
            second_hit_times = left_hit_times;
        }

        // only recurse into first box if it is a hit
        if (first_hit) {
            hit(ray, first_idx, closest);
        }

        // if second child's t is a closer hit than closest t
        if (second_hit && (second_hit_times[0] < closest.distance)) {
            hit(ray, second_idx, closest);
        }
    }    
}

template<typename Primitive>
BVH<Primitive>::BVH(std::vector<Primitive>&& prims, size_t max_leaf_size) {
    build(std::move(prims), max_leaf_size);
}

template<typename Primitive>
BVH<Primitive> BVH<Primitive>::copy() const {
    BVH<Primitive> ret;
    ret.nodes = nodes;
    ret.primitives = primitives;
    ret.root_idx = root_idx;
    return ret;
}

template<typename Primitive>
bool BVH<Primitive>::Node::is_leaf() const {
    return l == r;
}

template<typename Primitive>
size_t BVH<Primitive>::new_node(BBox box, size_t start, size_t size, size_t l, size_t r) {
    Node n;
    n.bbox = box;
    n.start = start;
    n.size = size;
    n.l = l;
    n.r = r;
    nodes.push_back(n);
    return nodes.size() - 1;
}

template<typename Primitive>
BBox BVH<Primitive>::bbox() const {
    return nodes[root_idx].bbox;
}

template<typename Primitive>
std::vector<Primitive> BVH<Primitive>::destructure() {
    nodes.clear();
    return std::move(primitives);
}

template<typename Primitive>
void BVH<Primitive>::clear() {
    nodes.clear();
    primitives.clear();
}

} // namespace PT
