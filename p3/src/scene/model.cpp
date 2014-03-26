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

void Model::tri_intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index,
                                MeshVertex a, MeshVertex b, MeshVertex c) const
{
    Triangle tri = Triangle();
    tri.vertices[0].position = a.position;
    tri.vertices[1].position = b.position;
    tri.vertices[2].position = c.position;

    tri.intersects_ray(r, intsec, geom_index);
}

void Model::intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const 
{
    IntersectInfo tri_intsec;
    Raytracer::initialize_intsec_info(tri_intsec);
    for (size_t i = 0; i < mesh->num_triangles(); i++) {
        MeshVertex vtx_a = mesh->get_vertices()[mesh->get_triangles()[i].vertices[0]];
        MeshVertex vtx_b = mesh->get_vertices()[mesh->get_triangles()[i].vertices[1]];
        MeshVertex vtx_c = mesh->get_vertices()[mesh->get_triangles()[i].vertices[2]];
       
        tri_intersects_ray(r, tri_intsec, i, vtx_a, vtx_b, vtx_c);
        
        if (tri_intsec.intersection_found && (!intsec.intersection_found 
            || (tri_intsec.t_hit < intsec.t_hit))) {
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
