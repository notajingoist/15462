#include "p5/collisions.hpp"

namespace _462 {

bool collides( SphereBody& body1, SphereBody& body2, real_t collision_damping )
{
    //detect collision. If there is one, update velocity
    Vector3 v1 = body1.velocity;
    Vector3 v2 = body2.velocity;
    Vector3 p1 = body1.position;
    Vector3 p2 = body2.position;
    real_t m1 = body1.mass;
    real_t m2 = body2.mass;

    Vector3 relative_dir = p2 - p1;
    Vector3 relative_vel = v1 - v2;

    Vector3 unit_relative_dir = normalize(relative_dir);
    bool headed_towards = dot(relative_vel, unit_relative_dir) > 0.0;

    if (!headed_towards) {
        return false;
    }

    if (distance(p1, p2) < (body1.radius + body2.radius)) {
        //collision
        Vector3 v1_p = v1 - v2;
        Vector3 v2_p = Vector3::Zero();

        Vector3 d = normalize(p2 - p1);
        Vector3 v2_pp = 2.0 * d * (m1 / (m1 + m2)) * (dot(v1_p, d));
        
        Vector3 u2 = v2 + v2_pp;
        Vector3 u1 = ((m1 * v1) + (m2 * v2) - (m2 * u2)) / m1;

        body1.velocity = compute_damped_velocity(u1, collision_damping);
        body2.velocity = compute_damped_velocity(u2, collision_damping);

        //printf("sphere collision occurred\n");
        return true;
    }
    return false;
}

bool collides( SphereBody& body1, TriangleBody& body2, real_t collision_damping )
{
    //detect collision. If there is one, update velocity
    Vector3 p1 = body1.position;
    Vector3 p2 = body2.position;
    Vector3 v1 = body1.velocity;
    Vector3 vtx_a = body2.vertices[0];
    Vector3 vtx_b = body2.vertices[1];
    Vector3 vtx_c = body2.vertices[2];

    Vector3 n =  normalize(cross(vtx_b - vtx_a, vtx_c - vtx_a));
    n = (dot(p1 - vtx_a, n) > 0.0) ? n : -n;
    bool headed_towards = dot(v1, n) < 0.0;
    if (!headed_towards) {
        return false;
    }
    
    Vector3 a = p1 - p2;
    real_t d = dot(a, n);
    Vector3 p_p = p1 - (d * n);
    
    Vector3 n_a = normalize(cross(vtx_c - vtx_b, p_p - vtx_b));
    Vector3 n_b = normalize(cross(vtx_a - vtx_c, p_p - vtx_c));
    Vector3 n_c = normalize(cross(vtx_b - vtx_a, p_p - vtx_a));

    Vector3 unnormalized_n = cross(vtx_b - vtx_a, vtx_c - vtx_a);
    real_t n_squared = squared_length(unnormalized_n);
    real_t alpha = dot(unnormalized_n, n_a) / n_squared;
    real_t beta = dot(unnormalized_n, n_b) / n_squared;
    real_t gamma = dot(unnormalized_n, n_c) / n_squared;

    bool point_within = (0.0 < alpha && alpha < 1.0) 
        && (0.0 < beta && beta < 1.0)
        && (0.0 < gamma && gamma < 1.0);

    if (point_within) {
        if (distance(p_p, p1) < body1.radius) {
            body1.velocity = compute_new_velocity(v1, n, collision_damping);
            //printf("triangle collision occurred\n");
            return true;
        }
    } else {
        //p_p is not in triangle
        Vector3 norm_b_a = normalize(vtx_b - vtx_a);
        Vector3 norm_c_a = normalize(vtx_c - vtx_a);
        Vector3 norm_c_b = normalize(vtx_c - vtx_b);
        Vector3 p_p1 = (dot(p_p - vtx_a, norm_b_a) * norm_b_a) + vtx_a;
        Vector3 p_p2 = (dot(p_p - vtx_b, norm_c_a) * norm_c_a) + vtx_a;
        Vector3 p_p3 = (dot(p_p - vtx_b, norm_c_b) * norm_c_b) + vtx_b;
        
        if (distance(p_p1, p1) < body1.radius
            || distance(p_p2, p1) < body1.radius
            || distance(p_p3, p1) < body1.radius) {
            body1.velocity = compute_new_velocity(v1, n, collision_damping);
            //printf("special triangle collision occurred\n");
            return true;
        } 
    }
    return false;
}

Vector3 compute_new_velocity(Vector3 old_velocity, Vector3 normal, 
    real_t collision_damping)
{
    Vector3 new_velocity = old_velocity - (2.0 * (dot(old_velocity, normal)) 
        * normal);
    return compute_damped_velocity(new_velocity, collision_damping);
}

Vector3 compute_damped_velocity(Vector3 v, real_t collision_damping) 
{
    Vector3 damped_velocity = v - (collision_damping * v);
    return (length(damped_velocity) > EPSILON) ? damped_velocity 
        : Vector3::Zero();
}


bool collides( SphereBody& body1, PlaneBody& body2, real_t collision_damping )
{
    // detect collision. If there is one, update velocity
    Vector3 p1 = body1.position;
    Vector3 p2 = body2.position;
    Vector3 v1 = body1.velocity;
    Vector3 n = body2.normal; //is n normalized?

    Vector3 a = p1 - p2;
    real_t d = dot(a, n);

    if (abs(d) < body1.radius) {
        body1.velocity = compute_new_velocity(v1, n, collision_damping);
        //printf("plane collision occurred\n");
        return true;
    }
    return false;
}

}
