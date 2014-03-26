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
#define SLOP 0.001

#include "math/color.hpp"
#include "math/random462.hpp"
#include "math/vector.hpp"
#include "scene/material.hpp"
namespace _462 {

struct IntersectInfo {
    Vector3 e;
    Vector3 d;

    size_t geom_index;
    size_t tri_index;
    
    real_t t_hit;
    real_t t_leave;
    Vector3 n_hit;
    Vector3 n_leave;
    
    bool intersection_found;
    bool model_tri;
    bool tri;
    bool sphere;
    bool sphere_two;
    bool sphere_one;
};

class Scene;
class Ray;
struct Intersection;
class Raytracer
{
public:

    Raytracer();

    ~Raytracer();

    bool initialize(Scene* scene, size_t num_samples,
                    size_t width, size_t height);

    bool raytrace(unsigned char* buffer, real_t* max_time);

    static void initialize_intsec_info(IntersectInfo& intsec);

private:

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
};

} /* _462 */

#endif /* _462_RAYTRACER_HPP_ */
