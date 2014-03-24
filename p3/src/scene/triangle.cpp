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

int Triangle::intersects_ray(Ray r) const
{
    Vector3 trans_e = invMat.transform_point(r.e); 
    Vector3 trans_d = invMat.transform_vector(r.d);

    Vertex vtx_a = vertices[0];
    Vertex vtx_b = vertices[1];
    Vertex vtx_c = vertices[2];

    real_t a = vtx_a.position.x - vtx_b.position.x; //xa - xb
    real_t b = vtx_a.position.y - vtx_b.position.y; //ya - yb
    real_t c = vtx_a.position.z - vtx_b.position.z; //za - zb
    real_t d = vtx_a.position.x - vtx_c.position.x; //xa - xc
    real_t e = vtx_a.position.y - vtx_c.position.y; //ya - yc
    real_t f = vtx_a.position.z - vtx_c.position.z; //za - zc
    real_t g = trans_d.x; //xd
    real_t h = trans_d.y; //yd
    real_t i = trans_d.z; //zd
    real_t j = vtx_a.position.x - trans_e.x; //xa - xe
    real_t k = vtx_a.position.y - trans_e.y; //ya - ye
    real_t l = vtx_a.position.z - trans_e.z; //za - ze

    real_t M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
    //real_t = (-1)*(f*

    return 0;
}

} /* _462 */
