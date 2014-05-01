#ifndef _462_PHYSICS_PHYSICS_HPP_
#define _462_PHYSICS_PHYSICS_HPP_

#include "math/math.hpp"
#include "math/quaternion.hpp"
#include "math/vector.hpp"
#include "p5/body.hpp"
#include "p5/spherebody.hpp"
#include "p5/trianglebody.hpp"
#include "p5/planebody.hpp"
#include "p5/spring.hpp"
#include "p5/collisions.hpp"
#include <stdio.h>
#include <vector>

namespace _462 {

struct State {
    Vector3 initial_position;
    Vector3 initial_velocity;
    Quaternion initial_orientation;
    Vector3 initial_angular_velocity;
};

struct Derivative {
    Vector3 dx; //change in position, dx/dt = velocity
    Vector3 dv; //change in velocity, dv/dt = acceleration
};

class Physics
{
public:
    Vector3 gravity;
	real_t collision_damping;
    
    Physics();
    ~Physics();

    void RK4(Body& s, real_t dt);
    void detect_collisions(size_t i);
    void step( real_t dt );
    void add_sphere( SphereBody* s );
    size_t num_spheres() const;
    void add_plane( PlaneBody* p );
    size_t num_planes() const;
    void add_triangle( TriangleBody* t );
    size_t num_triangles() const;
    void add_spring( Spring* s );
    size_t num_springs() const;

    void reset();

private:
    typedef std::vector< Spring* > SpringList;
    typedef std::vector< SphereBody* > SphereList;
    typedef std::vector< PlaneBody* > PlaneList;
    typedef std::vector< TriangleBody* > TriangleList;

    SpringList springs;
    SphereList spheres;
    PlaneList planes;
    TriangleList triangles;
};

}

#endif

