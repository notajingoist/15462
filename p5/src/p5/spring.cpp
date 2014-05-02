#include "math/math.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "math/matrix.hpp"
#include "p5/spring.hpp"
#include "p5/body.hpp"
#include "p5/spherebody.hpp"
#include <iostream>
#include <stdio.h>

namespace _462 {

Spring::Spring()
{
    body1_offset = Vector3::Zero();
    body2_offset = Vector3::Zero();
    damping = 0.0;
}

void Spring::step( real_t dt )
{
    // TODO apply forces to attached bodies
    Vector3 f;

    Vector3 rotated_b1_offset = body1->orientation * 
        body1_offset;
    Vector3 rotated_b2_offset = body2->orientation * 
        body2_offset;

    Vector3 a1 = body1->position + rotated_b1_offset; //attachment pt 1
    Vector3 a2 = body2->position + rotated_b2_offset; //attachment pt 2
    real_t dist = distance(a1, a2); 
    real_t displacement = dist - equilibrium;

    Vector3 v1 = body1->velocity - body2->velocity;
    
    Vector3 d1 = a1 - a2;
    Vector3 unit_d1 = normalize(d1);
    
    real_t dxdt1 = dot(unit_d1, v1);

    f = unit_d1 * ((-constant * displacement) - (damping * dxdt1));

    body1->apply_force(f, rotated_b1_offset);
    body2->apply_force(-f, rotated_b2_offset);
}


}


