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

/*void Physics::f(State& initial_state, Derivative& input, Derivative& output, 
    real_t dt_step) {
    State state; 
    state.x = initial_state.x + (dt_step * input.dx); //necessary?
    state.v = initial_state.v + (dt_step * input.dv); 
    output.dx = state.v;
    output.dv = initial_state.a;
    //printf("x: %lf, y: %lf, z: %lf accel\n", output.dv.x, output.dv.y, output.dv.z);

}*/

void Physics::RK4(Body& b, real_t dt) {
    /*Derivative k0, k1, k2, k3, k4;
    k0.dx = Vector3::Zero();
    k0.dv = Vector3::Zero();

    f(state, k0, k1, dt * 0.0);
    f(state, k1, k2, dt * 0.5);
    f(state, k2, k3, dt * 0.5);
    f(state, k3, k4, dt * 1.0);*/

    Derivative k1, k2, k3, k4;
    k1.dx = b.step_position(dt * 0.0, collision_damping);
    //apply force
    k2.dx = b.step_position(dt * 0.5, collision_damping);
    k3.dx = b.step_position(dt * 0.5, collision_damping);
    k4.dx = b.step_position(dt * 1.0, collision_damping);

    /*k1.dv = b.force / b.mass;
    k2.dv = b.force / b.mass;
    k3.dv = b.force / b.mass;
    k4.dv = b.force / b.mass;*/

    k1.dv = b.get_acceleration();
    k2.dv = b.get_acceleration();
    k3.dv = b.get_acceleration();
    k4.dv = b.get_acceleration();

    b.position += dt * (1.0/6.0) * (k1.dx + (2.0 * (k2.dx + k3.dx)) + k4.dx);
    b.velocity += dt * (1.0/6.0) * (k1.dv + (2.0 * (k2.dv + k3.dv)) + k4.dv);

    /*k1.dax = s.step_orientation(dt * 0.0, collision_damping);
    k2.dax = s.step_orientation(dt * 0.5, collision_damping);
    k3.dax = s.step_orientation(dt * 0.5, collision_damping);
    k4.dax = s.step_orientation(dt * 1.0, collision_damping);
    
    k1.dav = s.get_angular_acceleration();
    k2.dav = s.get_angular_acceleration();
    k3.dav = s.get_angular_acceleration();
    k4.dav = s.get_angular_acceleration();

    s.orientation *= Quaternion::Quaternion(axis, radians);
    s.angular_velocity += dt * (1.0/6.0) * (k1.dav + (2.0 * (k2.dav + k3.dav)) 
        + k4.dav);
    */


    //graphical
    //s.sphere->position = s.position;
    //s.sphere->orientation = s.orientation;
    /*output.dx = initial_state.s->position + 
        initial_state.s->step_position(dt_step, collision_damping);
    output.dv = calculate_acceleration(initial_state);*/
}

void Physics::detect_collisions(size_t i)
{   
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

void Physics::step( real_t dt )
{

    for (size_t i = 0; i < num_springs(); i++) {
        Vector3 b1_initial_position = springs[i]->body1->position;
        Vector3 b1_initial_velocity = springs[i]->body1->velocity;
        Quaternion b1_initial_orientation = springs[i]->body1->orientation;
        Vector3 b1_initial_angular_velocity = 
            springs[i]->body1->angular_velocity;

        Vector3 b2_initial_position = springs[i]->body2->position;
        Vector3 b2_initial_velocity = springs[i]->body2->velocity;
        Quaternion b2_initial_orientation = springs[i]->body2->orientation;
        Vector3 b2_initial_angular_velocity = 
            springs[i]->body2->angular_velocity;

        //springs[i]->apply_force();
        //springs[i].step(dt);
        //RK4(*(springs[i]->body1), dt);
        //RK4(*(springs[i]->body2), dt);
        
        //springs[i]->body1->apply_force();
        //springs[i]->body2->apply_force();
    }

    for (size_t i = 0; i < num_spheres(); i++) {
        //reset force
        //spheres[i]->force = Vector3::Zero();
        //add gravity
        spheres[i]->apply_force(gravity, Vector3::Zero());

        detect_collisions(i);
        RK4(*(spheres[i]), dt);
        spheres[i]->update_graphics();   

        //reset force
        spheres[i]->force = Vector3::Zero();
        
        /*State state;
        state.x = spheres[i]->position;
        state.v = spheres[i]->velocity; 
        state.a = spheres[i]->get_acceleration();*/
                //spheres[i]->update(state.v, state.x);
        //printf("x: %lf, y: %lf, z: %lf vel\n", state.v.x, state.v.y, state.v.z);
        //spheres[i]->step_position(dt, collision_damping);
    }

    
    // TODO step the world forward by dt. Need to detect collisions, apply
    // forces, and integrate positions and orientations.
    //
    // Note: put RK4 here, not in any of the physics bodies
    //
    // Must use the functions that you implemented
    //
    // Note, when you change the position/orientation of a physics object,
    // change the position/orientation of the graphical object that represents
    // it
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
