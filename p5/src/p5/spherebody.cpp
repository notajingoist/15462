#include "p5/spherebody.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "scene/sphere.hpp"
#include <iostream>
#include <stdio.h>
#include <exception>
#include <algorithm>

namespace _462 {

SphereBody::SphereBody( Sphere* geom )
{
    sphere = geom;
    position = sphere->position;
    radius = sphere->radius;
    orientation = sphere->orientation;
    mass = 0.0;
    velocity = Vector3::Zero();
    angular_velocity = Vector3::Zero();
    force = Vector3::Zero();
    torque = Vector3::Zero();
}

Vector3 SphereBody::get_acceleration() 
{
    //printf("%lf, %lf, %lf \n", force.x/mass, force.y/mass, force.z/mass);
    return force / mass;
}

void SphereBody::update(Vector3 updated_velocity, Vector3 updated_position)
{
    //internal
    velocity = updated_velocity;
    position = updated_position;

    //graphical
    sphere->position = updated_position; 
}

Vector3 SphereBody::step_position( real_t dt, real_t motion_damping )
{
    // Note: This function is here as a hint for an approach to take towards
    // programming RK4, you should add more functions to help you or change the
    // scheme
    // TODO return the delta in position dt in the future
    //Vector3 acceleration = force / mass;
   /* Vector3 dx = velocity * dt;
    Vector3 dv = acceleration * dt; 
    position += dx; 
    velocity += dv;
    return dx;*/
    //position = state.x;
    //velocity = state.v;

    //return velocity;

    return Vector3::Zero();
}

Vector3 SphereBody::step_orientation( real_t dt, real_t motion_damping )
{
    // Note: This function is here as a hint for an approach to take towards
    // programming RK4, you should add more functions to help you or change the
    // scheme
    // TODO return the delta in orientation dt in the future
    // vec.x = rotation along x axis
    // vec.y = rotation along y axis
    // vec.z = rotation along z axis
    

    
    return Vector3::Zero();
}

void SphereBody::apply_force( const Vector3& f, const Vector3& offset )
{
    // TODO apply force/torque to sphere
    
    Vector3 off_cross_f = cross(offset, f);
    if (offset == Vector3::Zero() || off_cross_f == Vector3::Zero()) {
        //linear force, f = gravity = a
        Vector3 linear_force = mass * f;
        force = linear_force;
        //Vector3 acceleration = force/mass;
        //velocity += acceleration * dt;
        //position += velocity * dt;

        //printf("mass: %lf \n", mass);
        //printf("%lf, %lf, %lf \n", force.x, force.y, force.z);
    } else {
        //linear force and angular force
        torque = off_cross_f;
        real_t I = (2.0/5.0) * mass * (radius * radius);
        Vector3 angular_accel = torque / I;
        Vector3 angular_force = mass * angular_accel;

        Vector3 unit_offset = normalize(offset); 
        Vector3 linear_force = offset * (dot(f, offset) / length(offset));
        force = linear_force + angular_force;
    }
}

}
