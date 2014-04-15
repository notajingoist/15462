/**
 * @file triangle.cpp
 * @brief Function definitions for the Triangle class.
 *
 * @author Eric Butler (edbutler)
 */

#include "scene/triangle.hpp"
#include "application/opengl.hpp"

namespace _462 {

Triangle::Triangle()
{
    vertices[0].material = 0;
    vertices[1].material = 0;
    vertices[2].material = 0;
}

Triangle::~Triangle() { }

void Triangle::render() const
{
    bool materials_nonnull = true;
    for ( int i = 0; i < 3; ++i )
        materials_nonnull = materials_nonnull && vertices[i].material;

    // this doesn't interpolate materials. Ah well.
    if ( materials_nonnull )
        vertices[0].material->set_gl_state();

    glBegin(GL_TRIANGLES);

    glNormal3dv( &vertices[0].normal.x );
    glTexCoord2dv( &vertices[0].tex_coord.x );
    glVertex3dv( &vertices[0].position.x );

    glNormal3dv( &vertices[1].normal.x );
    glTexCoord2dv( &vertices[1].tex_coord.x );
    glVertex3dv( &vertices[1].position.x);

    glNormal3dv( &vertices[2].normal.x );
    glTexCoord2dv( &vertices[2].tex_coord.x );
    glVertex3dv( &vertices[2].position.x);

    glEnd();

    if ( materials_nonnull )
        vertices[0].material->reset_gl_state();
}

void Triangle::from_mesh(const Mesh* mesh, size_t tri_index, const Material* material) 
{   
    for (size_t i = 0; i < 3; i++) {
        MeshVertex mesh_vtx = 
            mesh->get_vertices()[mesh->get_triangles()[tri_index].vertices[i]];
        vertices[i].position = mesh_vtx.position;
        vertices[i].normal = mesh_vtx.normal;
        vertices[i].tex_coord = mesh_vtx.tex_coord;
        vertices[i].material = material;
    }
}

Color3 Triangle::get_specular(IntersectInfo& intsec) const
{
    Vertex vtx_a = vertices[0];
    Vertex vtx_b = vertices[1];
    Vertex vtx_c = vertices[2];
    Color3 specular = (intsec.alpha*vtx_a.material->specular)
        + (intsec.beta*vtx_b.material->specular)
        + (intsec.gamma*vtx_c.material->specular);
    return specular;
}

real_t Triangle::get_refractive_index(IntersectInfo& intsec) const
{

    Vertex vtx_a = vertices[0];
    Vertex vtx_b = vertices[1];
    Vertex vtx_c = vertices[2];
    real_t refractive_index = (intsec.alpha*vtx_a.material->refractive_index)
        + (intsec.beta*vtx_b.material->refractive_index)
        + (intsec.gamma*vtx_c.material->refractive_index);
    return refractive_index;
}

Color3 Triangle::compute_color(IntersectInfo& intsec, ColorInfo& colinf) const
{
    Vertex vtx_a = vertices[0];
    Vertex vtx_b = vertices[1];
    Vertex vtx_c = vertices[2];
    
    Color3 ca = colinf.scene->ambient_light;
    Color3 ka = (intsec.alpha*vtx_a.material->ambient)
        + (intsec.beta*vtx_b.material->ambient)
        + (intsec.gamma*vtx_c.material->ambient);
    Color3 kd = (intsec.alpha*vtx_a.material->diffuse)
        + (intsec.beta*vtx_b.material->diffuse)
        + (intsec.gamma*vtx_c.material->diffuse);
    colinf.kd = kd;

    Color3 c_all_lights = compute_lights_color(intsec, colinf);
    Color3 cp = colinf.tp*((ca*ka) + c_all_lights);
   
    return cp;
}

Color3 Triangle::compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const
{
    Vertex vtx_a = vertices[0];
    Vertex vtx_b = vertices[1];
    Vertex vtx_c = vertices[2];
    
    Vector2 tex_coord = (intsec.alpha*vtx_a.tex_coord)
        + (intsec.beta*vtx_b.tex_coord)
        + (intsec.gamma*vtx_c.tex_coord);
  
    real_t tc_x_frac = tex_coord.x - floor(tex_coord.x);
    real_t tc_y_frac = tex_coord.y - floor(tex_coord.y);

    int wa, ha, wb, hb, wc, hc;
    vtx_a.material->get_texture_size(&wa, &ha);
    vtx_b.material->get_texture_size(&wb, &hb);
    vtx_c.material->get_texture_size(&wc, &hc);

    real_t tc_x_a = tc_x_frac * wa;
    real_t tc_y_a = tc_y_frac * ha;
    real_t tc_x_b = tc_x_frac * wb;
    real_t tc_y_b = tc_y_frac * hb;
    real_t tc_x_c = tc_x_frac * wc;
    real_t tc_y_c = tc_y_frac * hc;
    
    Color3 tp = (intsec.alpha*vtx_a.material->get_texture_pixel(tc_x_a, tc_y_a))
        + (intsec.beta*vtx_b.material->get_texture_pixel(tc_x_b, tc_y_b))
        + (intsec.gamma*vtx_c.material->get_texture_pixel(tc_x_c, tc_y_c));

    return tp;
}

void Triangle::find_min_max(real_t& x_min, real_t& x_max, real_t& y_min, 
    real_t& y_max, real_t& z_min, real_t& z_max) const
{
    Vertex vtx_a = vertices[0];
    Vertex vtx_b = vertices[1];
    Vertex vtx_c = vertices[2];

    Vector3 vtx_a_pos = mat.transform_point(vtx_a.position);
    Vector3 vtx_b_pos = mat.transform_point(vtx_b.position);
    Vector3 vtx_c_pos = mat.transform_point(vtx_c.position);

    x_min = get_min(vtx_a_pos.x, vtx_b_pos.x, vtx_c_pos.x);
    x_max = get_max(vtx_a_pos.x, vtx_b_pos.x, vtx_c_pos.x);
    y_min = get_min(vtx_a_pos.y, vtx_b_pos.y, vtx_c_pos.y);
    y_max = get_max(vtx_a_pos.y, vtx_b_pos.y, vtx_c_pos.y);
    z_min = get_min(vtx_a_pos.z, vtx_b_pos.z, vtx_c_pos.z);
    z_max = get_max(vtx_a_pos.z, vtx_b_pos.z, vtx_c_pos.z);
}

void Triangle::intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const
{
    Vertex vtx_a = vertices[0];
    Vertex vtx_b = vertices[1];
    Vertex vtx_c = vertices[2];
 
    Vector3 vtx_a_pos = vtx_a.position;
    Vector3 vtx_b_pos = vtx_b.position;
    Vector3 vtx_c_pos = vtx_c.position;
    
    Vector3 vtx_a_n = vtx_a.normal;
    Vector3 vtx_b_n = vtx_b.normal;
    Vector3 vtx_c_n = vtx_c.normal;

    Raytracer::intersects_tri_vertices(r, intsec, geom_index, vtx_a_pos, vtx_b_pos, 
        vtx_c_pos, vtx_a_n, vtx_b_n, vtx_c_n, invMat, normMat);
   
}

} /* _462 */


