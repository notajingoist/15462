/**
 * @file sphere.cpp
 * @brief Function defnitions for the Sphere class.
 *
 * @author Kristin Siu (kasiu)
 * @author Eric Butler (edbutler)
 */

#include "scene/sphere.hpp"
#include "application/opengl.hpp"

namespace _462 {

#define SPHERE_NUM_LAT 80
#define SPHERE_NUM_LON 100

#define SPHERE_NUM_VERTICES ( ( SPHERE_NUM_LAT + 1 ) * ( SPHERE_NUM_LON + 1 ) )
#define SPHERE_NUM_INDICES ( 6 * SPHERE_NUM_LAT * SPHERE_NUM_LON )
// index of the x,y sphere where x is lat and y is lon
#define SINDEX(x,y) ((x) * (SPHERE_NUM_LON + 1) + (y))
#define VERTEX_SIZE 8
#define TCOORD_OFFSET 0
#define NORMAL_OFFSET 2
#define VERTEX_OFFSET 5

static unsigned int Indices[SPHERE_NUM_INDICES];
static float Vertices[VERTEX_SIZE * SPHERE_NUM_VERTICES];

static void init_sphere()
{
    static bool initialized = false;
    if ( initialized )
        return;

    for ( int i = 0; i <= SPHERE_NUM_LAT; i++ ) {
        for ( int j = 0; j <= SPHERE_NUM_LON; j++ ) {
            real_t lat = real_t( i ) / SPHERE_NUM_LAT;
            real_t lon = real_t( j ) / SPHERE_NUM_LON;
            float* vptr = &Vertices[VERTEX_SIZE * SINDEX(i,j)];

            vptr[TCOORD_OFFSET + 0] = lon;
            vptr[TCOORD_OFFSET + 1] = 1-lat;

            lat *= PI;
            lon *= 2 * PI;
            real_t sinlat = sin( lat );

            vptr[NORMAL_OFFSET + 0] = vptr[VERTEX_OFFSET + 0] = sinlat * sin( lon );
            vptr[NORMAL_OFFSET + 1] = vptr[VERTEX_OFFSET + 1] = cos( lat ),
            vptr[NORMAL_OFFSET + 2] = vptr[VERTEX_OFFSET + 2] = sinlat * cos( lon );
        }
    }

    for ( int i = 0; i < SPHERE_NUM_LAT; i++ ) {
        for ( int j = 0; j < SPHERE_NUM_LON; j++ ) {
            unsigned int* iptr = &Indices[6 * ( SPHERE_NUM_LON * i + j )];

            unsigned int i00 = SINDEX(i,  j  );
            unsigned int i10 = SINDEX(i+1,j  );
            unsigned int i11 = SINDEX(i+1,j+1);
            unsigned int i01 = SINDEX(i,  j+1);

            iptr[0] = i00;
            iptr[1] = i10;
            iptr[2] = i11;
            iptr[3] = i11;
            iptr[4] = i01;
            iptr[5] = i00;
        }
    }

    initialized = true;
}

Sphere::Sphere()
    : radius(0), material(0) {}

Sphere::~Sphere() {}

void Sphere::render() const
{
    // create geometry if we haven't already
    init_sphere();

    if ( material )
        material->set_gl_state();

    // just scale by radius and draw unit sphere
    glPushMatrix();
    glScaled( radius, radius, radius );
    glInterleavedArrays( GL_T2F_N3F_V3F, VERTEX_SIZE * sizeof Vertices[0], Vertices );
    glDrawElements( GL_TRIANGLES, SPHERE_NUM_INDICES, GL_UNSIGNED_INT, Indices );
    glPopMatrix();

    if ( material )
        material->reset_gl_state();
}

void Sphere::intersects_ray(Ray r, IntersectInfo& intsec) const
{
    Vector3 trans_e = invMat.transform_point(r.e); 
    Vector3 trans_d = invMat.transform_vector(r.d);
    
    real_t A = dot(trans_d, trans_d);
    real_t B = dot((2*trans_d), (trans_e));
    real_t C = dot((trans_e), (trans_e)) - (radius*radius);
    real_t discriminant = (B*B) - (4*A*C);
    //real_t t_hit, t_leave;
    //Vector3 n_hit, n_leave;

    if (discriminant < 0) {
        //no solutions, ray and sphere do not intersect
        intsec.intersects = false;
        //return 0;
    } else if (discriminant > 0) {
        //2 solutions, one where ray enters sphere, one where it leaves
        real_t t1 = (dot(-1*trans_d, (trans_e)) + sqrt(discriminant))/(dot(trans_d, trans_d));
        real_t t2 = (dot(-1*trans_d, (trans_e)) - sqrt(discriminant))/(dot(trans_d, trans_d));
        
        intsec.t_hit = (t1 < t2) ? t1 : t2;
        intsec.t_leave = (t1 < t2) ? t2 : t1;
        intsec.n_hit = normalize(normMat*(2*((trans_e + intsec.t_hit*trans_d))));
        intsec.n_leave = normalize(normMat*(2*((trans_e + intsec.t_leave*trans_d))));
       
        intsec.intersects = (intsec.t_hit > 0) ? true : false; 
    } else {
        //1 solution, ray grazes sphere at one point
        intsec.t_hit = (dot(-1*trans_d, (trans_e)) + sqrt(discriminant))/(dot(trans_d, trans_d));
        intsec.n_hit = normalize(normMat*(2*((trans_e + intsec.t_hit*trans_d))));
    
        intsec.intersects = (intsec.t_hit > 0) ? true : false;
    }
}


} /* _462 */


