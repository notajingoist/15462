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

Color3 Sphere::compute_color(IntersectInfo& intsec, ColorInfo& colinf) const
{
    /**
    Vector3 p = intsec.e + (intsec.t_hit * intsec.d);     
    for (size_t i = 0; i < colinf.scene->num_lights(); i++) {
        Vector3 light_pos = colinf.scene->get_lights()[i].position;
        Vector3 light_dir = normalize(light_pos - p); //L
        Ray shadow_r = Ray(p, light_dir);
        
        IntersectInfo b_intsec;
        Raytracer::initialize_intsec_info(b_intsec);
        colinf.scene->shoot_ray(shadow_r, b_intsec);
        if (b_intsec.intersection_found) {
            return Color3::Black();
        } else {
            return Color3::White();
        }
    }
    return Color3::White();
    **/
    
    Vector3 p = intsec.e + (intsec.t_hit * intsec.d);     
    
    //ambient color of light
    Color3 ca = colinf.scene->ambient_light;
    //material's ambient color
    Color3 ka = material->ambient;
    //material's diffuse color
    Color3 kd = material->diffuse;
    
    real_t xc = position.x;
    real_t yc = position.y;
    real_t zc = position.z;
    
    real_t theta = acos((p.z-zc)/radius);
    real_t phi = atan2((p.y-yc), (p.x-xc));
    phi = (phi < 0) ? (phi + 2*PI) : phi;

    real_t u = phi/(2*PI);
    real_t v = (PI-theta)/PI;

    //texture color at point p
    Color3 tp = material->get_texture_pixel(u, v); 

    Color3 c_all_lights = Color3::Black();
    for (size_t i = 0; i < colinf.scene->num_lights(); i++) {
        Color3 c = colinf.scene->get_lights()[i].color;
        real_t ac = colinf.scene->get_lights()[i].attenuation.constant;
        real_t al = colinf.scene->get_lights()[i].attenuation.linear;
        real_t aq = colinf.scene->get_lights()[i].attenuation.quadratic;

        Vector3 light_pos = colinf.scene->get_lights()[i].position;
        Vector3 light_dir = normalize(light_pos - p); //L
        real_t light_dist = distance(p, light_pos); //d
        Ray shadow_r = Ray(p, light_dir);
        
        real_t b = 1;
        IntersectInfo b_intsec;
        Raytracer::initialize_intsec_info(b_intsec);
        colinf.scene->shoot_ray(shadow_r, b_intsec);
        if (b_intsec.intersection_found) {
            Vector3 obj_pos = p + (b_intsec.t_hit * light_dir);
            real_t obj_dist = distance(p, obj_pos);
            if (obj_dist <= light_dist) {
                b = 0;
            }
        } 

        real_t n_dot_l = dot(intsec.n_hit, light_dir);
        real_t max_n_dot_l = (n_dot_l > 0) ? n_dot_l : 0;

        Color3 ci = c*(1.0/(ac + (light_dist*al) + (light_dist*light_dist*aq)));

        c_all_lights += b*ci*kd*max_n_dot_l; 
    }

    Color3 cp = tp*((ca*ka) + c_all_lights);
    
    Vector3 reflection_r = intsec.d - (2.0*dot(intsec.d, intsec.n_hit)*intsec.n_hit);
    
    
    return cp;
    //return clamp(cp, 0.0, 1.0);
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
    /**
    real_t t1 = (dot(-1*trans_d, (trans_e)) + sqrt(discriminant))/(dot(trans_d, trans_d));
    real_t t2 = (dot(-1*trans_d, (trans_e)) - sqrt(discriminant))/(dot(trans_d, trans_d));
    **/

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

    Vector3 n_hit = normMat*(normalize(trans_e + t_hit*trans_d));
    if (!intsec.intersection_found || (t_hit < intsec.t_hit)) {
        intsec.e = r.e;
        intsec.d = r.d;
        intsec.intersection_found = true;
        intsec.t_hit = t_hit;
        intsec.n_hit = n_hit;
        intsec.geom_index = geom_index;
    }

    /**
    else if (discriminant > 0) {
        //2 solutions, one where ray enters sphere, one where it leaves
        real_t t1 = (dot(-1*trans_d, (trans_e)) + sqrt(discriminant))/(dot(trans_d, trans_d));
        real_t t2 = (dot(-1*trans_d, (trans_e)) - sqrt(discriminant))/(dot(trans_d, trans_d));
        
        real_t t_hit = (t1 < t2) ? t1 : t2;
        real_t t_leave = (t1 < t2) ? t2 : t1;
        Vector3 n_hit = normMat*(normalize(trans_e + t_hit*trans_d));
        //normalize(normMat*(2*((trans_e + t_hit*trans_d))));

        Vector3 n_leave = normMat*(normalize(trans_e + t_leave*trans_d));
        //normalize(normMat*(2*((trans_e + t_leave*trans_d))));
       
        if ((t_hit > SLOP) && (!intsec.intersection_found
            || (t_hit < intsec.t_hit))) {
            intsec.e = trans_e;
            intsec.d = trans_d;
            intsec.intersection_found = true;
            intsec.t_hit = t_hit;
            intsec.t_leave = t_leave;
            intsec.n_hit = n_hit;
            intsec.n_leave = n_leave;
            intsec.geom_index = geom_index;
        }
    } else {
        //1 solution, ray grazes sphere at one point
        real_t t_hit = (dot(-1*trans_d, (trans_e)) + sqrt(discriminant))/(dot(trans_d, trans_d));
        Vector3 n_hit = normMat*(normalize(trans_e + t_hit*trans_d));
    
        if ((t_hit > SLOP) && (!intsec.intersection_found
            || (t_hit < intsec.t_hit))) {
            intsec.e = trans_e;
            intsec.d = trans_d;
            intsec.intersection_found = true;
            intsec.t_hit = t_hit;
            intsec.n_hit = n_hit;
            intsec.geom_index = geom_index;
        }
    }
    **/
}


} /* _462 */
