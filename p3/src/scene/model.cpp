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

Color3 Model::compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const
{
    printf("computing tp color\n");
    Triangle tri = Triangle();
    tri.from_mesh(mesh, intsec.tri_index, material);
    return tri.compute_tp(intsec, colinf);
}

Color3 Model::compute_color(IntersectInfo& intsec, ColorInfo& colinf) const
{
    printf("computing model color\n");
    Triangle tri = Triangle();
    tri.from_mesh(mesh, intsec.tri_index, material);
    return tri.compute_color(intsec, colinf);
}

void Model::intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const 
{
    IntersectInfo tri_intsec;
    Raytracer::initialize_intsec_info(tri_intsec);
    for (size_t i = 0; i < mesh->num_triangles(); i++) {
        /*
        MeshVertex vtx_a = mesh->get_vertices()[mesh->get_triangles()[i].vertices[0]];
        MeshVertex vtx_b = mesh->get_vertices()[mesh->get_triangles()[i].vertices[1]];
        MeshVertex vtx_c = mesh->get_vertices()[mesh->get_triangles()[i].vertices[2]];
        */
        
        Triangle tri = Triangle();
        tri.from_mesh(mesh, i, material);
        tri.intersects_ray(r, tri_intsec, i);
        if (tri_intsec.intersection_found && (!intsec.intersection_found 
            || (tri_intsec.t_hit < intsec.t_hit))) {
            intsec.e = tri_intsec.e;
            intsec.d = tri_intsec.d;
            printf("tri_intsec.e: %lf, %lf, %lf\n", tri_intsec.e.x, tri_intsec.e.y, tri_intsec.e.z);
            printf("r.e: %lf, %lf, %lf\n", r.e.x, r.e.y, r.e.z);
            intsec.intersection_found = true;
            intsec.t_hit = tri_intsec.t_hit;
            intsec.n_hit = tri_intsec.n_hit;
            intsec.geom_index = geom_index;
            intsec.tri_index = tri_intsec.geom_index;
            intsec.model_tri = true;
        }

    }
}

} /* _462 */
