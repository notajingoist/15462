/**
 * @file model.cpp
 * @brief Model class
 *
 * @author Eric Butler (edbutler)
 * @author Zeyang Li (zeyangl)
 */

#include "scene/model.hpp"
#include "scene/material.hpp"
#include "application/opengl.hpp"
#include "scene/triangle.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>


namespace _462 {

Model::Model() : mesh( 0 ), material( 0 ) { }
Model::~Model() { }

void Model::render() const
{
    if ( !mesh )
        return;
    if ( material )
        material->set_gl_state();
    mesh->render();
    if ( material )
        material->reset_gl_state();
}

Color3 Model::get_specular(IntersectInfo& intsec) const
{
    return material->specular;
}

real_t Model::get_refractive_index(IntersectInfo& intsec) const
{
    return material->refractive_index;
}

Color3 Model::compute_kd(IntersectInfo& intsec, ColorInfo& colinf) const
{
    //Color3 ca = colinf.scene->ambient_light;
    //Color3 ka = material->ambient;
    Color3 kd = material->diffuse; 
    colinf.kd = kd;

   /* Color3 c_all_lights = compute_lights_color(intsec, colinf);
    Color3 cp = colinf.tp*(c_all_lights);
   
    return cp;*/
    return kd;
}

Color3 Model::compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const
{
    Triangle tri = Triangle();
    tri.from_mesh(mesh, intsec.tri_index, material);
    return tri.compute_tp(intsec, colinf);
}

void Model::find_min_max(real_t& x_min, real_t& x_max, real_t& y_min,
        real_t& y_max, real_t& z_min, real_t& z_max) const
{
    bool first_tri = true; 
    real_t trix_min, trix_max, triy_min, triy_max, triz_min, triz_max;
    for (size_t i = 0; i < mesh->num_triangles(); i++) {
        Triangle tri = Triangle();
        tri.from_mesh(mesh, i, material);
        tri.find_min_max(trix_min, trix_max, triy_min, triy_max, 
            triz_min, triz_max);
        
        if (first_tri) {
            x_min = trix_min;
            x_max = trix_max;
            y_min = triy_min;
            y_max = triy_max;
            z_min = triz_min;
            z_max = triz_max;
        } else {
            x_min = std::min(x_min, trix_min);
            x_max = std::max(x_max, trix_max);
            y_min = std::min(y_min, triy_min);
            y_max = std::max(y_max, triy_max);
            z_min = std::min(z_min, triz_min);
            z_max = std::max(z_max, triz_max);
        }
        first_tri = false;
    }
}

void Model::intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const 
{
    for (size_t i = 0; i < mesh->num_triangles(); i++) {
        MeshVertex vtx_a = mesh->get_vertices()[mesh->get_triangles()[i].vertices[0]];
        MeshVertex vtx_b = mesh->get_vertices()[mesh->get_triangles()[i].vertices[1]];
        MeshVertex vtx_c = mesh->get_vertices()[mesh->get_triangles()[i].vertices[2]];
        
        Vector3 vtx_a_pos = vtx_a.position;
        Vector3 vtx_b_pos = vtx_b.position;
        Vector3 vtx_c_pos = vtx_c.position;
        
        Vector3 vtx_a_n = vtx_a.normal;
        Vector3 vtx_b_n = vtx_b.normal;
        Vector3 vtx_c_n = vtx_c.normal;
        
        IntersectInfo tri_intsec;
        initialize_intsec_info(tri_intsec);
        
        intersects_tri_vertices(r, tri_intsec, i, vtx_a_pos, 
            vtx_b_pos, vtx_c_pos, vtx_a_n, vtx_b_n, vtx_c_n, invMat, normMat);
   
        if (tri_intsec.intersection_found && (!intsec.intersection_found 
            || (tri_intsec.t_hit < intsec.t_hit))) {
            intsec.e = r.e;
            intsec.d = r.d;
            intsec.intersection_found = true;
            intsec.t_hit = tri_intsec.t_hit;
            intsec.n_hit = tri_intsec.n_hit;
            intsec.geom_index = geom_index;
            intsec.tri_index = i;
            intsec.gamma = tri_intsec.gamma;
            intsec.beta = tri_intsec.beta;
            intsec.alpha = tri_intsec.alpha;
        }

    }
}

} /* _462 */
