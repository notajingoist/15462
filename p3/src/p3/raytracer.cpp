/**
 * @file raytacer.cpp
 * @brief Raytracer class
 *
 * Implement these functions for project 4.
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "raytracer.hpp"
#include "scene/scene.hpp"

#include <SDL_timer.h>
#include <iostream>
#include <random>

#ifdef OPENMP // just a defense in case OpenMP is not installed.

#include <omp.h>

#endif
namespace _462 {

// max number of threads OpenMP can use. Change this if you like.
#define MAX_THREADS 8
#define RECURSE_DEPTH 3

static const unsigned STEP_SIZE = 8;

Raytracer::Raytracer()
    : scene(0), width(0), height(0) { }

// random real_t in [0, 1)
static inline real_t random()
{
    return real_t(rand())/RAND_MAX;
}

Raytracer::~Raytracer() { }

/**
 * Initializes the raytracer for the given scene. Overrides any previous
 * initializations. May be invoked before a previous raytrace completes.
 * @param scene The scene to raytrace.
 * @param width The width of the image being raytraced.
 * @param height The height of the image being raytraced.
 * @return true on success, false on error. The raytrace will abort if
 *  false is returned.
 */
bool Raytracer::initialize(Scene* scene, size_t num_samples,
			   size_t width, size_t height)
{
    /*
     * omp_set_num_threads sets the maximum number of threads OpenMP will
     * use at once.
     */
#ifdef OPENMP
    omp_set_num_threads(MAX_THREADS);
#endif
    this->scene = scene;
    this->num_samples = num_samples;
    this->width = width;
    this->height = height;

    current_row = 0;

    Ray::init(scene->camera);
    scene->initialize();

    const SphereLight* lights = scene->get_lights();

    // TODO any initialization or precompuation before the trace

    return true;
}



void Raytracer::initialize_intsec_info(IntersectInfo& intsec)
{
    intsec.intersection_found = false;
    intsec.model_tri = false;
    intsec.tri = false;
    intsec.sphere = false;
    intsec.t_hit = -1;
}

Color3 Raytracer::recursive_raytrace(const Scene* scene, Ray r, size_t depth) 
{
    if (depth <= 0) {
        return Color3::Black();
    } else {
        Geometry* const* geometries = scene->get_geometries();
        
        IntersectInfo intsec;
        initialize_intsec_info(intsec);
        scene->shoot_ray(r, intsec);
      
        if (intsec.intersection_found) {
            if (intsec.model_tri) {
                /*
                Mesh* m = geometries[intsec.geom_index]->mesh;
                MeshVertex vtx_a = 
                    m->get_vertices()[m->get_triangles()[intsec.tri_index].vertices[0]];
                MeshVertex vtx_b = 
                    m->get_vertices()[m->get_triangles()[intsec.tri_index].vertices[1]];
                MeshVertex vtx_c = 
                    m->get_vertices()[m->get_triangles()[intsec.tri_index].vertices[2]];
                */
                return Color3::White();
            } else {
                //sample color from geom object geometries[intsec.geom_index]
                ColorInfo colinf;
                colinf.p = intsec.e + (intsec.t_hit * intsec.d);
                colinf.scene = scene;
                colinf.tp = geometries[intsec.geom_index]->compute_tp(intsec, colinf);

                /**
                Color3 cp = 
                        geometries[intsec.geom_index]->compute_color(intsec, colinf);
                **/

                //reflection
                Ray reflection_r = Ray(colinf.p, 
                    intsec.d-(2.0*dot(intsec.d, intsec.n_hit)*intsec.n_hit));
                
                Color3 reflection_col = 
                    recursive_raytrace(scene, reflection_r, (depth-1)) 
                    * geometries[intsec.geom_index]->get_material()->specular
                    * colinf.tp;

                
                if (geometries[intsec.geom_index]->get_material()->refractive_index 
                    == 0) {
                    //opaque, phong illumination + reflection
                    Color3 cp = 
                        geometries[intsec.geom_index]->compute_color(intsec, colinf);
                    return cp + reflection_col;
                } else {
                    //transparent, refraction + reflection
                    Color3 refrac = Color3::Black();
                    return refrac + reflection_col;
                }
                
            }

        } else {
            return scene->background_color; 
        }
        
    }
}

/**
 * Performs a raytrace on the given pixel on the current scene.
 * The pixel is relative to the bottom-left corner of the image.
 * @param scene The scene to trace.
 * @param x The x-coordinate of the pixel to trace.
 * @param y The y-coordinate of the pixel to trace.
 * @param width The width of the screen in pixels.
 * @param height The height of the screen in pixels.
 * @return The color of that pixel in the final image.
 */
Color3 Raytracer::trace_pixel(const Scene* scene,
			      size_t x,
			      size_t y,
			      size_t width,
			      size_t height)
{
    assert(x < width);
    assert(y < height);

    real_t dx = real_t(1)/width;
    real_t dy = real_t(1)/height;

    Color3 res = Color3::Black();
    unsigned int iter;
    for (iter = 0; iter < num_samples; iter++)
    {
        // pick a point within the pixel boundaries to fire our
        // ray through.
        real_t i = real_t(2)*(real_t(x)+random())*dx - real_t(1);
        real_t j = real_t(2)*(real_t(y)+random())*dy - real_t(1);

        Ray r = Ray(scene->camera.get_position(), Ray::get_pixel_dir(i, j));

        res += recursive_raytrace(scene, r, RECURSE_DEPTH);
        // TODO return the color of the given pixel
        // you don't have to use this stub function if you prefer to
        // write your own version ofeRaytracer::raytrace.

    }
    res = clamp(res, 0.0, 1.0);
    return res*(real_t(1)/num_samples);
}



/**
 * Raytraces some portion of the scene. Should raytrace for about
 * max_time duration and then return, even if the raytrace is not copmlete.
 * The results should be placed in the given buffer.
 * @param buffer The buffer into which to place the color data. It is
 *  32-bit RGBA (4 bytes per pixel), in row-major order.
 * @param max_time, If non-null, the maximum suggested time this
 *  function raytrace before returning, in seconds. If null, the raytrace
 *  should run to completion.
 * @return true if the raytrace is complete, false if there is more
 *  work to be done.
 */
bool Raytracer::raytrace(unsigned char* buffer, real_t* max_time)
{
    // TODO Add any modifications to this algorithm, if needed.

    static const size_t PRINT_INTERVAL = 64;

    // the time in milliseconds that we should stop
    unsigned int end_time = 0;
    bool is_done;

    if (max_time)
    {
        // convert duration to milliseconds
        unsigned int duration = (unsigned int) (*max_time * 1000);
        end_time = SDL_GetTicks() + duration;
    }

    // until time is up, run the raytrace. we render an entire group of
    // rows at once for simplicity and efficiency.
    for (; !max_time || end_time > SDL_GetTicks(); current_row += STEP_SIZE)
    {
        // we're done if we finish the last row
        is_done = current_row >= height;
        // break if we finish
        if (is_done) break;

        int loop_upper = std::min(current_row + STEP_SIZE, height);

        // This tells OpenMP that this loop can be parallelized.
#pragma omp parallel for
        for (int c_row = current_row; c_row < loop_upper; c_row++)
        {
            /*
             * This defines a critical region of code that should be
             * executed sequentially.
             */
#pragma omp critical
            {
                if (c_row % PRINT_INTERVAL == 0)
                    printf("Raytracing (Row %d)\n", c_row);
            }

            for (size_t x = 0; x < width; x++)
            {
                // trace a pixel
                Color3 color = trace_pixel(scene, x, c_row, width, height);
                // write the result to the buffer, always use 1.0 as the alpha
                color.to_array(&buffer[4 * (c_row * width + x)]);
            }
        }
    }

    if (is_done) printf("Done raytracing!\n");

    return is_done;
}

} /* _462 */
