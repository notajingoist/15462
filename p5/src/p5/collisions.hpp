#ifndef _462_COLLISIONS_HPP_
#define _462_COLLISIONS_HPP_

#include "scene/sphere.hpp"
#include "p5/spherebody.hpp"
#include "p5/trianglebody.hpp"
#include "p5/planebody.hpp"
#include <stdio.h>
#define EPSILON 0.0001

namespace _462 {

bool collides( SphereBody& body1, SphereBody& body2, real_t collision_damping );
bool collides( SphereBody& body1, TriangleBody& body2, real_t collision_damping );
bool collides( SphereBody& body1, PlaneBody& body2, real_t collision_damping );
Vector3 compute_new_velocity(Vector3 old_velocity, Vector3 normal, 
    real_t collision_damping);
Vector3 compute_damped_velocity(Vector3 v, real_t collision_damping);

}

#endif
