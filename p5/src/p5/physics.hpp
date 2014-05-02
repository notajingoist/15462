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

/*struct State {
    Vector3 initial_position;
    Vector3 initial_velocity;
    Quaternion initial_orientation;
    Vector3 initial_angular_velocity;
};*/

struct State;

class Physics
{
public:
    Vector3 gravity;
	real_t collision_damping;
    
    Physics();
    ~Physics();

    void RK4(real_t dt);
    void RK4_step(real_t dt, real_t dt_fraction, real_t weight);
    void detect_collisions();
    void set_forces(real_t dt);
    void save_initial_states();
    void update_graphics();
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
    /*typedef std::vector< Derivative* > DerivativeList;
    typedef std::vector< DerivativeList > StateList;*/

    SpringList springs;
    SphereList spheres;
    PlaneList planes;
    TriangleList triangles;
    /*DerivativeList k1_derivs;
    DerivativeList k2_derivs;
    DerivativeList k3_derivs;
    DerivativeList k4_derivs;
    StateList states;
    states.push_back(k1_derivs);
    states.push_back(k2_derivs);
    states.push_back(k3_derivs);
    states.push_back(k4_derivs);*/
};

}

#endif

