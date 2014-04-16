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
//#include "scene/scene.hpp"

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
#define MAX_SUBPATH_LENGTH 4

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

    root_bbox = new Bbox();
    std::vector< size_t > indices;
    for (size_t i = 0; i < scene->num_geometries(); i++) {
        indices.push_back(i);
    }

    root_bbox->initialize_bbox(indices, scene->get_geometries());
    return true;
}

Color3 Raytracer::compute_subrays_color(IntersectInfo& intsec, ColorInfo& colinf,
    size_t curr_depth, size_t max_depth, IntersectInfo& final_intsec)
{   
    Vector3 random_dir = cos_weighted_hemi(intsec.n_hit);
    Ray subray = Ray(colinf.p, random_dir);
    
    IntersectInfo sub_intsec;
    sub_intsec.intersection_found = false;
    sub_intsec.t_hit = -1;
    root_bbox->intersects_ray(subray, sub_intsec, 0);
   
    if (sub_intsec.intersection_found) {
        ColorInfo sub_colinf;
        sub_colinf.p = sub_intsec.e + (sub_intsec.t_hit * sub_intsec.d);

        sub_colinf.tp = 
            scene->get_geometries()[sub_intsec.geom_index]->compute_tp(sub_intsec,
                sub_colinf);
        sub_colinf.kd = 
            scene->get_geometries()[sub_intsec.geom_index]->compute_kd(sub_intsec,
                sub_colinf);

        if (curr_depth < max_depth) {
            Color3 rest_of_subrays_color = 
                compute_subrays_color(sub_intsec, sub_colinf, curr_depth + 1,
                    max_depth, final_intsec);
            return (sub_colinf.tp * sub_colinf.kd) * rest_of_subrays_color;
        } else {
            final_intsec = sub_intsec;
            return (sub_colinf.tp * sub_colinf.kd); 
        }
    } else {
        return Color3::Black();
    }
}

Color3 Raytracer::compute_bd(IntersectInfo& intsec, ColorInfo& colinf)
{
    Color3 bd = Color3::Black();
    for (size_t i = 0; i < scene->num_lights(); i++) {           

        IntersectInfo final_forward_intsec, final_backward_intsec;
        final_forward_intsec.intersection_found = false;
        final_forward_intsec.t_hit = -1;
        final_backward_intsec.intersection_found = false;
        final_backward_intsec.t_hit = -1;

        size_t max_forward_depth = rand() % MAX_SUBPATH_LENGTH + 1;
        size_t max_backward_depth = rand() % MAX_SUBPATH_LENGTH + 1;
        
        Color3 forward_color = compute_subrays_color(intsec, colinf, 0, 
            max_forward_depth, final_forward_intsec);


        Color3 backward_color;
        real_t x = random_gaussian();
        real_t y = random_gaussian();
        real_t z = random_gaussian();
        
        Vector3 initial_backward_random_dir = normalize(Vector3(x, y, z));
        Vector3 light_pos = scene->get_lights()[i].position;
        Ray initial_backward_subray = Ray(light_pos, 
            initial_backward_random_dir);
        
        IntersectInfo backward_intsec;
        backward_intsec.intersection_found = false;
        backward_intsec.t_hit = -1;
        root_bbox->intersects_ray(initial_backward_subray, backward_intsec, 0);


        if (backward_intsec.intersection_found) {
            ColorInfo backward_colinf;
            backward_colinf.p = backward_intsec.e + (backward_intsec.t_hit 
                * backward_intsec.d);

            Color3 light_col = scene->get_lights()[i].color;
            //Vector3 light_dir = normalize(light_pos - colinf.p); //L
            real_t light_dist = distance(colinf.p, light_pos); //d

            backward_color = light_col * 
                compute_subrays_color(backward_intsec, backward_colinf, 0, 
                max_backward_depth, final_backward_intsec);
        } else {
            backward_color = Color3::Black();
        }
            

        if (final_forward_intsec.intersection_found &&
            final_backward_intsec.intersection_found) {

            Vector3 forward_p = final_forward_intsec.e 
                + (final_forward_intsec.t_hit * final_forward_intsec.d);
            Vector3 backward_p = final_backward_intsec.e 
                + (final_backward_intsec.t_hit * final_backward_intsec.d);

            IntersectInfo actual_intsec;
            actual_intsec.intersection_found = false;
            actual_intsec.t_hit = -1;

            real_t forward_backward_dist = distance(forward_p, backward_p);
            Vector3 forward_to_backward_dir = normalize(backward_p - forward_p);
            Ray forward_to_backward_ray = Ray(forward_p, forward_to_backward_dir);
            root_bbox->intersects_ray(forward_to_backward_ray, actual_intsec, 0);

            if (actual_intsec.intersection_found) {
            
                Vector3 actual_p = actual_intsec.e + (actual_intsec.t_hit * 
                    actual_intsec.d);
                real_t actual_dist = distance(forward_p, actual_p);       
 
                if (actual_dist < forward_backward_dist) {
                    bd += forward_color * backward_color;
                }
            } else {
                return bd;
            }
        } else {
            return bd;
        }
    }
 
    return bd; 
}

Color3 Raytracer::compute_lights_color(IntersectInfo& intsec, ColorInfo& colinf)
{
    Color3 c_all_lights = Color3::Black();
    for (size_t i = 0; i < scene->num_lights(); i++) {
        for (size_t iter = 0; iter < num_samples; iter++) {
            real_t x = random_gaussian();
            real_t y = random_gaussian();
            real_t z = random_gaussian();
            
            Vector3 random_p = normalize(Vector3(x, y, z));
            Vector3 light_pos = (scene->get_lights()[i].radius*random_p) 
                + (scene->get_lights()[i].position);

            Color3 c = scene->get_lights()[i].color;
            real_t ac = scene->get_lights()[i].attenuation.constant;
            real_t al = scene->get_lights()[i].attenuation.linear;
            real_t aq = scene->get_lights()[i].attenuation.quadratic;

            //Vector3 light_pos = colinf.scene->get_lights()[i].position;
            Vector3 light_dir = normalize(light_pos - colinf.p); //L
            real_t light_dist = distance(colinf.p, light_pos); //d
            Ray shadow_r = Ray(colinf.p, light_dir);
            
            real_t b = 1;
            IntersectInfo b_intsec;
            b_intsec.intersection_found = false;
            b_intsec.t_hit = -1;

            root_bbox->intersects_ray(shadow_r, b_intsec, 0);

            if (b_intsec.intersection_found) {
                Vector3 obj_pos = colinf.p + (b_intsec.t_hit * light_dir);
                real_t obj_dist = distance(colinf.p, obj_pos);
                if (obj_dist <= light_dist) {
                    b = 0;
                }
            } 

            real_t n_dot_l = dot(intsec.n_hit, light_dir);
            real_t max_n_dot_l = (n_dot_l > 0) ? n_dot_l : 0;

            Color3 ci = c*(1.0/(ac + (light_dist*al) + (light_dist*light_dist*aq)));

            Color3 diffuse = b*ci*max_n_dot_l;
            Color3 bd = compute_bd(intsec, colinf);
            c_all_lights += colinf.kd*(diffuse + bd); 
        }
    }
    return c_all_lights*(real_t(1)/num_samples);
}

Color3 Raytracer::recursive_raytrace(const Scene* scene, Ray r, size_t depth)
{
    if (depth <= 0) {
        return Color3::Black();
    } else {
        Geometry* const* geometries = scene->get_geometries();
        
        IntersectInfo intsec;
        intsec.intersection_found = false;
        intsec.t_hit = -1;
       
        //dummy geom_index value of 0;
        root_bbox->intersects_ray(r, intsec, 0);
        //scene->shoot_ray(r, intsec);
      
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
                colinf.kd = 
                    geometries[intsec.geom_index]->compute_kd(intsec, colinf);
                //Color3 bd = compute_bd();
                //Color3 diffuse = compute_diffuse(intsec, colinf);
                Color3 c_all_lights = compute_lights_color(intsec, colinf);
                Color3 phong_col = colinf.tp * c_all_lights; 
                //kd * (diffuse + bd);
                return phong_col + reflection_col;

                //Color3 phong_col = 
                //    geometries[intsec.geom_index]->compute_color(intsec, colinf);
                //return phong_col + reflection_col;
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
