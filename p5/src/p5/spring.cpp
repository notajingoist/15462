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
    Vector3 f1, f2;

    //Vector3 axis1, axis2;
    //real_t radians1, radians2;
    //body1->orientation.to_axis_angle(&axis1, &radians1);
    //body2->orientation.to_axis_angle(&axis2, &radians2);

    Vector3 rotated_b1_offset = body1_offset;// * body1->orientation;
    Vector3 rotated_b2_offset = body2_offset;// * body2->orientation;

    //rotate(body1->orientation * axis1, radians1);
    //rotate(body2->orientation * axis2, radians2);

    Vector3 a1 = body1->position + rotated_b1_offset; //attachment pt 1
    Vector3 a2 = body2->position + rotated_b2_offset; //attachment pt 2
    real_t dist = distance(a1, a2); 
    real_t displacement = dist - equilibrium;

    Vector3 v1 = body1->velocity - body2->velocity;
    Vector3 v2 = body2->velocity - body1->velocity;
    
    Vector3 d1 = a1 - a2;
    Vector3 d2 = a2 - a1;
    Vector3 unit_d1 = normalize(d1);
    Vector3 unit_d2 = normalize(d2);
    
    Vector3 dxdt1 = dot(unit_d1, v1) * unit_d1;
    Vector3 dxdt2 = dot(unit_d2, v2) * unit_d2;

    f1 = (-constant * displacement * unit_d1) - (damping * dxdt1);
    f2 = (-constant * displacement * unit_d2) - (damping * dxdt2);

    //f1 = Vector3::Zero();
    //f2 = Vector3::Zero();

    body1->apply_force(f1, rotated_b1_offset);
    body2->apply_force(f2, rotated_b2_offset);

    printf("f1: %lf, %lf, %lf\n", f1.x, f1.y, f1.z);
    printf("f2: %lf, %lf, %lf\n", f2.x, f2.y, f2.z);
}


}


