#include "p5/physics.hpp"

namespace _462 {

Physics::Physics()
{
    reset();
}

Physics::~Physics()
{
    reset();
}

void Physics::RK4_step(real_t dt_fraction, real_t weight) {
    set_forces(dt_fraction); //param actually unnecessary?
    for (size_t i = 0; i < num_spheres(); i++) {
        spheres[i]->state.dx += weight 
            * spheres[i]->step_position(dt_fraction, collision_damping);
        spheres[i]->state.dv += weight * dt_fraction
            * spheres[i]->get_acceleration();

        spheres[i]->state.dax += weight
            * spheres[i]->step_orientation(dt_fraction, collision_damping);
        spheres[i]->state.dav += weight * dt_fraction
            * spheres[i]->get_angular_acceleration();
    }
}

void Physics::RK4(real_t dt) {
    //reset states and derivatives
    for (size_t i = 0; i < num_spheres(); i++) {
        spheres[i]->state.dx = Vector3::Zero();
        spheres[i]->state.dv = Vector3::Zero();
        spheres[i]->state.dax = Vector3::Zero();
        spheres[i]->state.dav = Vector3::Zero();
    }

    RK4_step(dt * 0.0, 1.0/6.0);
    RK4_step(dt * 0.5, 1.0/3.0);
    RK4_step(dt * 0.5, 1.0/3.0);
    RK4_step(dt * 1.0, 1.0/6.0);


    for (size_t i = 0; i < num_spheres(); i++) { 
        spheres[i]->position += dt * spheres[i]->state.dx;
        spheres[i]->velocity += dt * spheres[i]->state.dv;
        
        Vector3 dax = dt * spheres[i]->state.dax;

        real_t x_radians = dax.x; //rotation around x axis
        real_t y_radians = dax.y; //rotation around y axis
        real_t z_radians = dax.z; //rotation around z axis

        Quaternion qx = Quaternion(Vector3::UnitX(), x_radians);
        Quaternion qy = Quaternion(Vector3::UnitY(), y_radians);
        Quaternion qz = Quaternion(Vector3::UnitZ(), z_radians);

        spheres[i]->orientation = 
            normalize(spheres[i]->orientation * qz); //roll
        spheres[i]->orientation = 
            normalize(spheres[i]->orientation * qx); //pitch
        spheres[i]->orientation = 
            normalize(spheres[i]->orientation * qy); //yaw
        
        spheres[i]->angular_velocity = 
            spheres[i]->initial_angular_velocity + 
            dt * spheres[i]->state.dav;
        //printf("%lf, %lf, %lf\n", spheres[i]->angular_velocity);
    }

    }

void Physics::detect_collisions()
{   
    for (size_t i = 0; i < num_spheres(); i++) {
        for (size_t j = 0; j < num_spheres(); j++) {
            if (i != j) {
                collides(*(spheres[i]), *(spheres[j]), collision_damping);
            }
        }

        for (size_t j = 0; j < num_planes(); j++) {
            collides(*(spheres[i]), *(planes[j]), collision_damping);
        }
        
        for (size_t j = 0; j < num_triangles(); j++) {
            collides(*(spheres[i]), *(triangles[j]), collision_damping);
        }
    }
}

void Physics::set_forces(real_t dt) 
{
    //reset force and apply gravity
    for (size_t i = 0; i < num_spheres(); i++) {
        spheres[i]->force = Vector3::Zero();
        spheres[i]->apply_force(gravity * 
            spheres[i]->mass, Vector3::Zero());
    }

    //apply spring forces
    for (size_t i = 0; i < num_springs(); i++) {
        springs[i]->step(dt);
    }
}

void Physics::save_initial_states() 
{
    for (size_t i = 0; i < num_spheres(); i++) {
        spheres[i]->initial_velocity = spheres[i]->velocity;
        spheres[i]->initial_position = spheres[i]->position;
        spheres[i]->initial_angular_velocity = spheres[i]->angular_velocity;
        spheres[i]->initial_orientation = spheres[i]->orientation;
    }
}

void Physics::update_graphics()
{
    for (size_t i = 0; i < num_spheres(); i++) {
        spheres[i]->update_graphics(); 
    }
}

void Physics::step( real_t dt )
{
    // step the world forward by dt. Need to detect collisions, apply
    // forces, and integrate positions and orientations.
    //
    // Note: put RK4 here, not in any of the physics bodies
    //
    // Must use the functions that you implemented
    //
    // Note, when you change the position/orientation of a physics object,
    // change the position/orientation of the graphical object that represents
    // it
      
    detect_collisions();
    save_initial_states(); //save initial state after changing it in collisions
    RK4(dt);
    update_graphics();

}

void Physics::add_sphere( SphereBody* b )
{
    spheres.push_back( b );
}

size_t Physics::num_spheres() const
{
    return spheres.size();
}

void Physics::add_plane( PlaneBody* p )
{
    planes.push_back( p );
}

size_t Physics::num_planes() const
{
    return planes.size();
}

void Physics::add_triangle( TriangleBody* t )
{
    triangles.push_back( t );
}

size_t Physics::num_triangles() const
{
    return triangles.size();
}

void Physics::add_spring( Spring* s )
{
    springs.push_back( s );
}

size_t Physics::num_springs() const
{
    return springs.size();
}

void Physics::reset()
{
    for ( SphereList::iterator i = spheres.begin(); i != spheres.end(); i++ ) {
        delete *i;
    }
    for ( PlaneList::iterator i = planes.begin(); i != planes.end(); i++ ) {
        delete *i;
    }
    for ( TriangleList::iterator i = triangles.begin(); i != triangles.end(); i++ ) {
        delete *i;
    }
    for ( SpringList::iterator i = springs.begin(); i != springs.end(); i++ ) {
        delete *i;
    }

    spheres.clear();
    planes.clear();
    triangles.clear();
    springs.clear();

    gravity = Vector3::Zero();
	collision_damping = 0.0;
}

}
