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

int Model::intersects_ray(Ray r) const 
{
    Triangle* all_triangles = new Triangle[mesh->num_triangles()];

    for (size_t i = 0; i < mesh->num_triangles(); i++) {
        MeshVertex vtx_a = mesh->get_vertices()[mesh->get_triangles()[i].vertices[0]];
        MeshVertex vtx_b = mesh->get_vertices()[mesh->get_triangles()[i].vertices[1]];
        MeshVertex vtx_c = mesh->get_vertices()[mesh->get_triangles()[i].vertices[2]];
        
        Triangle tri = Triangle();
         
        tri.vertices[0].position = vtx_a.position;
        tri.vertices[1].position = vtx_b.position;
        tri.vertices[2].position = vtx_c.position;
 
        tri.vertices[0].normal = vtx_a.normal;
        tri.vertices[1].normal = vtx_b.normal;
        tri.vertices[2].normal = vtx_c.normal;
 
        tri.vertices[0].tex_coord = vtx_a.tex_coord;
        tri.vertices[1].tex_coord = vtx_b.tex_coord;
        tri.vertices[2].tex_coord = vtx_c.tex_coord;

        tri.vertices[0].material = material;
        tri.vertices[1].material = material;
        tri.vertices[2].material = material;

        all_triangles[i] = tri; 
    }

    int intersection_result = 0;
    //real_t min_t = -1;
    for (size_t i = 0; i < mesh->num_triangles(); i++) {
        //real_t t = all_triangles[i].intersects_ray(r);
        if (all_triangles[i].intersects_ray(r)) {
            intersection_result = 1;
        }
    }

    delete[] all_triangles;
    return intersection_result;
}

} /* _462 */
