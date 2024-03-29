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
    
    initial_position = position;
    initial_velocity = velocity;
    initial_orientation = orientation;
    initial_angular_velocity = angular_velocity;
    
    force = Vector3::Zero();
    torque = Vector3::Zero();
}

Vector3 SphereBody::get_acceleration() 
{
    return force / mass;
}

Vector3 SphereBody::get_angular_acceleration()
{
    real_t I = (2.0/5.0) * mass * (radius * radius);
    return torque / I;
}

void SphereBody::update_graphics() 
{
    sphere->position = position;
    sphere->orientation = orientation;

}

Vector3 SphereBody::step_position( real_t dt, real_t fraction, 
    real_t motion_damping )
{
    // Note: This function is here as a hint for an approach to take towards
    // programming RK4, you should add more functions to help you or change the
    // scheme
    // TODO return the delta in position dt in the future

    Vector3 dx = velocity * dt; //input.dx * dt
    Vector3 dv = get_acceleration() * dt; //input.dv * dt

    position = initial_position + (dx * fraction); //state.x 
    velocity = initial_velocity + (dv * fraction); //state.v = output.dx -> input.dx

    return dx;
}

Vector3 SphereBody::step_orientation( real_t dt, real_t fraction, 
    real_t motion_damping )
{
    // Note: This function is here as a hint for an approach to take towards
    // programming RK4, you should add more functions to help you or change the
    // scheme
    // TODO return the delta in orientation dt in the future
    // vec.x = rotation along x axis
    // vec.y = rotation along y axis
    // vec.z = rotation along z axis
    
    Vector3 dax = angular_velocity * dt;
    Vector3 dav = get_angular_acceleration() * dt;

    real_t x_radians = fraction * dax.x; //rotation around x axis
    real_t y_radians = fraction * dax.y; //rotation around y axis
    real_t z_radians = fraction * dax.z; //rotation around z axis

    Quaternion qx = Quaternion(Vector3::UnitX(), x_radians);
    Quaternion qy = Quaternion(Vector3::UnitY(), y_radians);
    Quaternion qz = Quaternion(Vector3::UnitZ(), z_radians);

    orientation = normalize(initial_orientation * qz); //roll
    orientation = normalize(orientation * qx); //pitch
    orientation = normalize(orientation * qy); //yaw
    angular_velocity = initial_angular_velocity + (dav * fraction);

    return dax;
}

void SphereBody::apply_force( const Vector3& f, const Vector3& offset )
{
    // TODO apply force/torque to sphere
    
    Vector3 off_cross_f = cross(offset, f);
    if (offset == Vector3::Zero() || off_cross_f == Vector3::Zero()) {
        force += f;
    } else {
        //linear force and angular force
        torque += off_cross_f; //or += ??
        
        Vector3 unit_offset = normalize(offset); 
        Vector3 linear_force = unit_offset * (dot(f, unit_offset));
        force += linear_force;
    }
}

}
