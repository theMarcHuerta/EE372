#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

#include "TestQuadHit.cpp"

using namespace std;

struct output {
    vec3<double> hit_loc;
    vec3<double> normal;
    bool front_face;
    double t; // how far along the hit occured
    uint_3 mat;
    rgb_in color;

    static double thresh;

    bool operator==(const HitRecord<sfp_11_22>& o) {
        return  (std::abs(o.hit_loc.x.to_double() - hit_loc.x) < thresh &&
                std::abs(o.hit_loc.y.to_double() - hit_loc.y) < thresh &&
                std::abs(o.hit_loc.z.to_double() - hit_loc.z) < thresh &&
                std::abs(o.normal.x.to_double() - normal.x) < thresh &&
                std::abs(o.normal.y.to_double() - normal.y) < thresh &&
                std::abs(o.normal.z.to_double() - normal.z) < thresh &&
                std::abs(o.color.r.to_double() - color.r.to_double()) < thresh &&
                std::abs(o.color.g.to_double() - color.g.to_double()) < thresh &&
                std::abs(o.color.b.to_double() - color.b.to_double()) < thresh &&
                o.mat == mat);               
    }
};

// set comparison threshold
double output::thresh = 1e-1;

std::istream& operator>>(std::istream& is, sfp_11_22& val) {
    // Implementation depends on the specifics of sfp_11_22
    double temp;
    if (is >> temp) {
        val = temp; // Assuming conversion from double or similar
    }
    return is;
}

std::istream& operator>>(std::istream& is, uint_3& val) {
    // Implementation depends on the specifics of pbool
    unsigned int temp;
    if (is >> temp) {
        val = temp; // Assuming conversion from double or similar
    }
    return is;
}

std::istream& operator>>(std::istream& is, pbool& val) {
    // Implementation depends on the specifics of pbool
    int temp;
    if (is >> temp) {
        val = temp; // Assuming conversion from double or similar
    }
    return is;
}

std::istream& operator>>(std::istream& is, fp_1_22& val) {
    // Implementation depends on the specifics of sfp_11_22
    double temp;
    if (is >> temp) {
        val = temp; // Assuming conversion from double or similar
    }
    return is;
}

template<typename T>
std::vector<_quad_hittable<T>> read_hittables(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<_quad_hittable<T>> hittables;
    std::string line;

    if (!file) {
        std::cerr << "Unable to open file" << std::endl;
        return hittables;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        _quad_hittable<T> hittable;
        std::istringstream iss(line);

        // Read each component of the struct
        iss >> hittable.corner_pt.x >> hittable.corner_pt.y >> hittable.corner_pt.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> hittable.u.x >> hittable.u.y >> hittable.u.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> hittable.v.x >> hittable.v.y >> hittable.v.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> hittable.mat_type;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> hittable.is_invis;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> hittable.normal.x >> hittable.normal.y >> hittable.normal.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> hittable.w.x >> hittable.w.y >> hittable.w.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> hittable.d_plane;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> hittable.quad_color.r >> hittable.quad_color.g >> hittable.quad_color.b;

        hittables.push_back(hittable);
    }

    return hittables;
}

template<typename T>
std::vector<ray<T>> read_rays(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<ray<T>> rays;
    std::string line;

    if (!file) {
        std::cerr << "Unable to open file" << std::endl;
        return rays;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        ray<T> ray;
        std::istringstream iss(line);

        // Read each component of the struct
        iss >> ray.orig.x >> ray.orig.y >> ray.orig.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> ray.dir.x >> ray.dir.y >> ray.dir.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> ray.camera_ray;

        rays.push_back(ray);
    }

    return rays;
}

std::vector<output> read_recs(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<output> recs;
    std::string line;

    if (!file) {
        std::cerr << "Unable to open file" << std::endl;
        return recs;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        output rec;
        std::istringstream iss(line);

        // Read each component of the struct
        iss >> rec.hit_loc.x >> rec.hit_loc.y >> rec.hit_loc.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> rec.normal.x >> rec.normal.y >> rec.normal.z;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> rec.front_face;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> rec.t;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse

        iss >> rec.mat;

        std::getline(file, line);
        iss.clear();  // Clear any error flags
        iss.str(line);  // Set new string to parse
        
        iss >> rec.color.r >> rec.color.g >> rec.color.b;

        recs.push_back(rec);
    }

    return recs;
}