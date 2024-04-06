
#include "../rays/tri_mesh.h"
#include <cmath>

namespace PT {

BBox Triangle::bbox() const {

    // TODO (PathTracer): Task 2
    // compute the bounding box of the triangle

    // Beware of flat/zero-volume boxes! You may need to
    // account for that here, or later on in BBox::intersect

    // add x y and z coords  of each vertex to their own vectors for min and max calculations
    std::vector<float> xcoords = {vertex_list[v0].position.x, vertex_list[v1].position.x, vertex_list[v2].position.x};
    std::vector<float> ycoords = {vertex_list[v0].position.y, vertex_list[v1].position.y, vertex_list[v2].position.y};
    std::vector<float> zcoords = {vertex_list[v0].position.z, vertex_list[v1].position.z, vertex_list[v2].position.z};

    // calculate mins
    float xmin = *std::min_element(xcoords.begin(), xcoords.end());
    float ymin = *std::min_element(ycoords.begin(), ycoords.end());
    float zmin = *std::min_element(zcoords.begin(), zcoords.end());

    // calculate maxes
    float xmax = *std::max_element(xcoords.begin(), xcoords.end());
    float ymax = *std::max_element(ycoords.begin(), ycoords.end());
    float zmax = *std::max_element(zcoords.begin(), zcoords.end());

    BBox box(Vec3(xmin, ymin, zmin), Vec3(xmax, ymax, zmax));

    // check for 0 volume box
    if (!box.empty()) return box;
    else return BBox();
}

Trace Triangle::hit(const Ray& ray) const {

    // Vertices of triangle - has postion and surface normal
    // See rays/tri_mesh.h for a description of this struct
    
    Tri_Mesh_Vert v_0 = vertex_list[v0];
    Tri_Mesh_Vert v_1 = vertex_list[v1];
    Tri_Mesh_Vert v_2 = vertex_list[v2];
    
    // TODO (PathTracer): Task 2
    // Intersect this ray with a triangle defined by the above three points.
    // Intersection should yield a ray t-value, and a hit point (u,v) on the surface of the triangle

    // You'll need to fill in a "Trace" struct describing information about the hit (or lack of hit)
    // initialize ret with lack of hit values
    Trace ret;
    ret.origin = ray.point;
    ret.hit = false;       // was there an intersection?
    ret.distance = INFINITY;   // at what distance did the intersection occur?
    ret.position = Vec3{}; // where was the intersection?
    ret.normal = Vec3{};   // what was the surface normal at the intersection?
                           // (this should be interpolated between the three vertex normals)

    // compute variables used in cramer's rule
    Vec3 e1 = v_1.position - v_0.position;
    Vec3 e2 = v_2.position - v_0.position;
    Vec3 s = ray.point - v_0.position;

    // calculate scalar constant used in cramer's rule
    float denom = (dot(cross(e1, ray.dir), e2));

    // if denom is 0, there is no intersection
    if (denom == 0) return ret;
    
    float scalar = 1.0 / denom;

    // calculate u
    float u = dot(-cross(s, e2), ray.dir)*scalar;
    // if u is not between 0 and 1, there is no hit
    if (u < 0) return ret;

    // calculate v
    float v = dot(cross(e1, ray.dir), s)*scalar;
    // if v is not between 0 and 1, there is no hit
    if (v < 0) return ret;

    // if sum of u and v is greater than 1, we also miss the triangle
    if (u + v > 1) return ret;

    // calculate t
    float t = dot(-cross(s, e2), e1)*scalar;
    // if t is not within the ray's dist bounds, ignore hit
    if (t < ray.dist_bounds.x || t > ray.dist_bounds.y) return ret;

    // if we get here, we have a valid intersection, so update trace structure accordingly
    ret.hit = true;
    ret.distance = t;
    ret.position = ray.at(t);

    // calculate the surface normal at the intersection point
    float w = 1.0f - u - v;
    Vec3 new_normal = w*v_0.normal + u*v_1.normal + v*v_2.normal;
    new_normal.normalize();
    ret.normal = new_normal;

    // update ray dist bounds
    ray.dist_bounds.y = t;
    
    return ret;
}

Triangle::Triangle(Tri_Mesh_Vert* verts, unsigned int v0, unsigned int v1, unsigned int v2)
    : vertex_list(verts), v0(v0), v1(v1), v2(v2) {
}

// void Tri_Mesh::build(const GL::Mesh& mesh) {

//     verts.clear();
//     triangles.clear();

//     for(const auto& v : mesh.verts()) {
//         verts.push_back({v.pos, v.norm});
//     }

//     const auto& idxs = mesh.indices();

//     std::vector<Triangle> tris;
//     for(size_t i = 0; i < idxs.size(); i += 3) {
//         tris.push_back(Triangle(verts.data(), idxs[i], idxs[i + 1], idxs[i + 2]));
//     }

//     triangles.build(std::move(tris), 4);
// }

// Tri_Mesh::Tri_Mesh(const GL::Mesh& mesh) {
//     build(mesh);
// }

// Tri_Mesh Tri_Mesh::copy() const {
//     Tri_Mesh ret;
//     ret.verts = verts;
//     ret.triangles = triangles.copy();
//     return ret;
// }

// BBox Tri_Mesh::bbox() const {
//     return triangles.bbox();
// }

// Trace Tri_Mesh::hit(const Ray& ray) const {
//     Trace t = triangles.hit(ray);
//     return t;
// }

// size_t Tri_Mesh::visualize(GL::Lines& lines, GL::Lines& active, size_t level,
//                            const Mat4& trans) const {
//     return triangles.visualize(lines, active, level, trans);
// }

} // namespace PT
