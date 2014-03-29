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
#include "math/matrix.hpp"
namespace _462 {

struct IntersectInfo {
    Vector3 e;
    Vector3 d;

    size_t geom_index;
    size_t tri_index;
    
    real_t t_hit;
    Vector3 n_hit;
    
    bool intersection_found;
    
    real_t alpha;
    real_t beta;
    real_t gamma;
};
struct ColorInfo;
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

    static void intersects_tri_vertices(Ray r, IntersectInfo&, 
        size_t geom_index, Vector3 vtx_a_pos, Vector3 vtx_b_pos, Vector3 vtx_c_pos, 
            Vector3 vtx_a_n, Vector3 vtx_b_n, Vector3 vtx_c_n, Matrix4 invMat, 
            Matrix3 normMat);
private:

    
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
};

struct ColorInfo {
    //Geometry* const* geometries;
    //Color3 ambient_light;
    //real_t refractive_index;
    //const SphereLight* sphere_light;
    const Scene* scene;
    Vector3 p;
    Color3 tp;
    Color3 kd;
    size_t num_samples;
};



} /* _462 */



#endif /* _462_RAYTRACER_HPP_ */
