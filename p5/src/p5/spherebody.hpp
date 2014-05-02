#ifndef _462_PHYSICS_SPHEREBODY_HPP_
#define _462_PHYSICS_SPHEREBODY_HPP_

#include "scene/sphere.hpp"
#include "p5/body.hpp"

namespace _462 {

class Sphere;
struct State {
    Vector3 dx; //change in position, dx/dt = velocity
    Vector3 dv; //change in velocity, dv/dt = acceleration
    Vector3 dax; //change in orientation, dax/dt = angular velocity
    Vector3 dav; //change in angular velocity, dv/dt = angular acceleration
};

class SphereBody : public Body
{
public:
    Sphere* sphere;
    real_t radius;
    real_t mass;
    Vector3 force;
    Vector3 torque;

    Vector3 initial_velocity;
    Vector3 initial_position;
    Vector3 initial_angular_velocity;
    Quaternion initial_orientation;

    State state;

    SphereBody( Sphere* geom );
    virtual ~SphereBody() { }
    virtual Vector3 step_position( real_t dt, real_t fraction, 
        real_t motion_damping );
    virtual Vector3 step_orientation( real_t dt, real_t fraction, 
        real_t motion_damping );
    virtual void apply_force( const Vector3& f, const Vector3& offset );
    virtual Vector3 get_acceleration();
    Vector3 get_angular_acceleration();
    void update_graphics();
};

}

#endif

