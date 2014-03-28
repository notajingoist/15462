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

Color3 Model::compute_color(IntersectInfo& intsec, ColorInfo& colinf) const
{
    Color3 ca = colinf.scene->ambient_light;
    Color3 ka = material->ambient;
    Color3 kd = material->diffuse; 
    colinf.kd = kd;

    Color3 c_all_lights = compute_lights_color(intsec, colinf);
    Color3 cp = colinf.tp*((ca*ka) + c_all_lights);
   
    return cp;
}

Color3 Model::compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const
{
    Triangle tri = Triangle();
    tri.from_mesh(mesh, intsec.tri_index, material);
    return tri.compute_tp(intsec, colinf);
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
        Raytracer::initialize_intsec_info(tri_intsec);
        
        Raytracer::intersects_tri_vertices(r, tri_intsec, i, vtx_a_pos, 
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
            //intsec.model_tri = true;
            intsec.gamma = tri_intsec.gamma;
            intsec.beta = tri_intsec.beta;
            intsec.alpha = tri_intsec.alpha;
        }

    }
}

} /* _462 */
