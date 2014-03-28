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

Color3 Triangle::compute_color(IntersectInfo& intsec, ColorInfo& colinf) const
{
    //colinf.material = material; for each vertex
    return Color3::Red();
}

void Triangle::intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const
{
    Vector3 trans_e = invMat.transform_point(r.e); 
    Vector3 trans_d = invMat.transform_vector(r.d);
    
    Vertex vtx_a = vertices[0];
    Vertex vtx_b = vertices[1];
    Vertex vtx_c = vertices[2];
   
    Vector3 vtx_a_pos = vtx_a.position;
    Vector3 vtx_b_pos = vtx_b.position;
    Vector3 vtx_c_pos = vtx_c.position;
    
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
    real_t t = (-1)*(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/M;
    if (t <= SLOP) { //<= ? 
        return;
    }

    real_t gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/M; 
    if ((gamma < 0) || (gamma > 1)) {
        return;
    }

    real_t beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
    if ((beta < 0) || (beta > (1 - gamma))) {
        return;
    }

    Vector3 pre_n = cross((vtx_b_pos - vtx_a_pos), (vtx_c_pos - vtx_a_pos));
    Vector3 n = normalize(normMat*pre_n);
    if (!intsec.intersection_found || (t < intsec.t_hit)) {
        intsec.e = r.e;
        intsec.d = r.d;
        intsec.intersection_found = true;
        intsec.t_hit = t;
        intsec.n_hit = n;
        intsec.geom_index = geom_index;
    }
    
}

} /* _462 */









