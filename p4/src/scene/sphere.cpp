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

Color3 Sphere::get_specular(IntersectInfo& intsec) const
{
    return material->specular;
}

real_t Sphere::get_refractive_index(IntersectInfo& intsec) const
{
    return material->refractive_index;
}

Color3 Sphere::compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const
{
    real_t xc = position.x;
    real_t yc = position.y;
    real_t zc = position.z;
    
    real_t theta = acos((colinf.p.z-zc)/radius);
    real_t phi = atan2((colinf.p.y-yc), (colinf.p.x-xc));
    phi = (phi < 0) ? (phi + 2*PI) : phi;

    real_t u = phi/(2*PI);
    real_t v = (PI-theta)/PI;

    //texture color at point p
    return material->get_texture_pixel(u, v); 
}

Color3 Sphere::compute_kd(IntersectInfo& intsec, ColorInfo& colinf) const
{
    //ambient color of light
   // Color3 ca = colinf.scene->ambient_light;
    //material's ambient color
   // Color3 ka = material->ambient;
    //material's diffuse color
    Color3 kd = material->diffuse;

    colinf.kd = kd;
    return kd;
    //colinf.ca = ca;
    //colinf.ka = ka;
    //colinf.kd = kd;
    
   // Color3 c_all_lights = compute_lights_color(intsec, colinf);
    //Color3 cp = colinf.tp*(c_all_lights);

    //return cp;
}

void Sphere::find_min_max(real_t& x_min, real_t& x_max, real_t& y_min, 
    real_t& y_max, real_t& z_min, real_t& z_max) const 
{
    //should I multiply scale.blah by radius?
    //printf("%f, %f, %f\n", scale.x, scale.y, scale.z);
    //printf("%f\n", radius);

    
    real_t max_scale_component = get_max((scale.x*radius), (scale.y*radius), 
        (scale.z*radius));
  
  /*
    if ((scale.x == 1.0) && (scale.y == 1.0) && (scale.z == 1.0)) {
        max_scale_component = radius;
    }*/
    /*
    max_scale_component = (radius > max_scale_component) 
        ? radius : max_scale_component; */
    x_min = position.x - max_scale_component;
    x_max = position.x + max_scale_component;
    y_min = position.y - max_scale_component;
    y_max = position.y + max_scale_component;
    z_min = position.z - max_scale_component;
    z_max = position.z + max_scale_component;
}

void Sphere::intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const
{
    Vector3 trans_e = invMat.transform_point(r.e); 
    Vector3 trans_d = invMat.transform_vector(r.d);
  
    real_t A = dot(trans_d, trans_d);
    real_t B = dot((2.0*trans_d), (trans_e));
    real_t C = dot((trans_e), (trans_e)) - (radius*radius);
    real_t discriminant = (B*B) - (4.0*A*C);

    if (discriminant < 0.0) {
        return;
    }

    real_t t1 = ((-1.0*B)+sqrt(discriminant))/(2.0*A);
    real_t t2 = ((-1.0*B)-sqrt(discriminant))/(2.0*A);

    real_t t_hit;
    if (t1 <= SLOP && t2 <= SLOP) {
        return;
    } else if (t1 <= SLOP) {
        t_hit = t2;
    } else if (t2 <= SLOP) {
        t_hit = t1;
    } else {
        t_hit = (t1 < t2) ? t1 : t2;
    }

    Vector3 n_hit = 
        normalize(normMat*(normalize(trans_e + t_hit*trans_d)));
    if (!intsec.intersection_found || (t_hit < intsec.t_hit)) {
        intsec.e = r.e;
        intsec.d = r.d;
        intsec.intersection_found = true;
        intsec.t_hit = t_hit;
        intsec.n_hit = n_hit;
        intsec.geom_index = geom_index;

        //printf("intersection found for sphere with radius: %f\n", radius);
    }
}


} /* _462 */
