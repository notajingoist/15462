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
#define RECURSE_DEPTH 5

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

void Raytracer::intersects_tri_vertices(Ray r, IntersectInfo& intsec, 
    size_t geom_index, Vector3 vtx_a_pos, Vector3 vtx_b_pos, Vector3 vtx_c_pos,
    Vector3 vtx_a_n, Vector3 vtx_b_n, Vector3 vtx_c_n, Matrix4 invMat, 
    Matrix3 normMat)
{
    Vector3 trans_e = invMat.transform_point(r.e); 
    Vector3 trans_d = invMat.transform_vector(r.d);
    
    real_t a = vtx_a_pos.x - vtx_b_pos.x; //xa - xb
    real_t b = vtx_a_pos.y - vtx_b_pos.y; //ya - yb
    real_t c = vtx_a_pos.z - vtx_b_pos.z; //za - zb
    real_t d = vtx_a_pos.x - vtx_c_pos.x; //xa - xc
    real_t e = vtx_a_pos.y - vtx_c_pos.y; //ya - yc
    real_t f = vtx_a_pos.z - vtx_c_pos.z; //za - zc
    real_t g = trans_d.x; //xd
    real_t h = trans_d.y; //yd
    real_t i = trans_d.z; //zd
    real_t j = vtx_a_pos.x - trans_e.x; //xa - xe
    real_t k = vtx_a_pos.y - trans_e.y; //ya - ye
    real_t l = vtx_a_pos.z - trans_e.z; //za - ze

    real_t M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
    if (M == 0.0) {
        return;
    }

    real_t t = (-1.0)*(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/M;
    if (t <= SLOP) { //<= ? 
        return;
    }

    real_t gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/M; 
    if ((gamma < 0.0) || (gamma > 1.0)) {
        return;
    }

    real_t beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
    if ((beta < 0.0) || (beta > (1.0 - gamma))) {
        return;
    }

    real_t alpha = 1.0 - gamma - beta;
    Vector3 pre_n = (alpha*vtx_a_n) + (beta*vtx_b_n) 
        + (gamma*vtx_c_n);
    Vector3 n = normalize(normMat*(normalize(pre_n)));
    if (!intsec.intersection_found || (t < intsec.t_hit)) {
        intsec.e = r.e;
        intsec.d = r.d;
        intsec.intersection_found = true;
        intsec.t_hit = t;
        intsec.n_hit = n;
        intsec.geom_index = geom_index;
        intsec.gamma = gamma;
        intsec.beta = beta;
        intsec.alpha = alpha;
    }
}

void Raytracer::initialize_intsec_info(IntersectInfo& intsec)
{
    intsec.intersection_found = false;
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
            ColorInfo colinf;
            colinf.num_samples = num_samples;
            colinf.p = intsec.e + (intsec.t_hit * intsec.d);
            colinf.scene = scene;
            colinf.tp = geometries[intsec.geom_index]->compute_tp(intsec, colinf);
            real_t refrac_index = 
                geometries[intsec.geom_index]->get_refractive_index(intsec);

            Ray reflection_r = Ray(colinf.p, 
                intsec.d-(2.0*dot(intsec.d, intsec.n_hit)*intsec.n_hit));
            Color3 reflection_col = recursive_raytrace(scene, reflection_r, 
                (depth-1))
                * geometries[intsec.geom_index]->get_specular(intsec) 
                * colinf.tp;
            if (refrac_index == 0.0) {
                //opaque, phong illumination + reflection
                Color3 phong_col = 
                    geometries[intsec.geom_index]->compute_color(intsec, colinf);
                return phong_col + reflection_col;
            } else {
                //transparent, refraction + reflection
                real_t d_dot_n = dot(intsec.n_hit, intsec.d);
                real_t nt, n;
                Vector3 refrac_norm;

                if (d_dot_n < 0) {
                    //entering
                    n = scene->refractive_index;
                    nt = refrac_index;
                    refrac_norm = intsec.n_hit;
                } else {
                    //leaving
                    refrac_norm = -intsec.n_hit;
                    d_dot_n = -d_dot_n;
                    n = refrac_index;
                    nt = scene->refractive_index;
                }

                real_t sqrt_term = 1.0-(((n*n)*(1.0-(d_dot_n*d_dot_n)))/(nt*nt));
                if (sqrt_term <= 0) {
                    //total internal reflection
                    return reflection_col;
                } else {
                    Vector3 t_dir = 
                        (n*(intsec.d-(refrac_norm*d_dot_n))/nt)
                        - (refrac_norm*(sqrt(sqrt_term)));
                    Ray transmission_r = Ray(colinf.p, t_dir);
                    Color3 refraction_col = 
                        recursive_raytrace(scene, transmission_r, (depth-1)); 
                    real_t R0 = ((nt-1.0)/(nt+1.0))*((nt-1.0)/(nt+1.0));
                    real_t R = R0 + (1.0-R0)*(pow(1.0-fabs(d_dot_n),5));
                    Color3 cp = (R*reflection_col) + ((1.0-R)*refraction_col);
                    return cp;
                }
            }
        } else { //intersection not found
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

        res += clamp(recursive_raytrace(scene, r, RECURSE_DEPTH), 0.0, 1.0);
        // TODO return the color of the given pixel
        // you don't have to use this stub function if you prefer to
        // write your own version ofeRaytracer::raytrace.

    }
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
