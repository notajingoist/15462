/**
 * @file raytacer.hpp
 * @brief Raytracer class
 *
 * Implement these functions for project 3.
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#ifndef _462_RAYTRACER_HPP_
#define _462_RAYTRACER_HPP_

#define MAX_DEPTH 5

#include "math/color.hpp"
#include "math/random462.hpp"
#include "math/vector.hpp"
#include "scene/material.hpp"
#include "math/matrix.hpp"
#include "scene/scene.hpp"
#include "scene/bbox.hpp"

namespace _462 {

//struct ColorInfo;
class Scene;
class Ray;
struct Intersection;

static Vector3 cos_weighted_hemi(const Vector3& n)
{
    real_t Xi1 = random();
    real_t Xi2 = random();

    real_t theta = std::acos(std::sqrt(1.0-Xi1));
    real_t phi = 2.0 * M_PI * Xi2; 
 
    real_t xs = sin(theta) * cos(phi);
    real_t ys = cos(theta);
    real_t zs = sin(theta) * sin(phi);
 
    Vector3 y(n.x, n.y, n.z);
    Vector3 h = y;
    if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z)) {
        h.x= 1.0;
    } else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z)) {
        h.y= 1.0;
    } else {
        h.z= 1.0;
    }
 
    Vector3 x = normalize(cross(h, y));
    Vector3 z = normalize(cross(x, y));
    
    Vector3 dir = xs * x + ys * y + zs * z;
    return normalize(dir);
}


class Raytracer
{
public:

    Raytracer();

    ~Raytracer();

    bool initialize(Scene* scene, size_t num_samples,
                    size_t width, size_t height);

    bool raytrace(unsigned char* buffer, real_t* max_time);

private:

    Color3 compute_subrays_color(IntersectInfo& intsec, ColorInfo& colinf,
        size_t curr_depth, size_t max_depth, IntersectInfo& final_intsec);
    Color3 compute_bd(IntersectInfo& intsec, ColorInfo& colinf);
    Color3 compute_lights_color(IntersectInfo& intsec, ColorInfo& colinf);
        
    Color3 recursive_raytrace(const Scene* scene, Ray r, size_t depth);

    Color3 trace_pixel(const Scene* scene,
		       size_t x,
		       size_t y,
		       size_t width,
		       size_t height);

    // the scene to trace
    Scene* scene;

    // the dimensions of the image to trace
    size_t width, height;

    // the next row to raytrace
    size_t current_row;

    unsigned int num_samples;

    Bbox* root_bbox;
};





} /* _462 */



#endif /* _462_RAYTRACER_HPP_ */
