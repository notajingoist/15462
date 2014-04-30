#include "p5/collisions.hpp"

namespace _462 {

bool collides( SphereBody& body1, SphereBody& body2, real_t collision_damping )
{
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


    //Vector3 unit_v1_v2 = normalize(v1 - v2);
    //real_t proj_v1_v1v2 = dot(v1, unit_v1_v2);
    //real_t relative_velocity = length(v1) - length(v2);

    /*if (proj_v1_v1v2 > 0.0) {
        printf("truee\n");
    }*/

    //bool headed_towards = (relative_velocity > 0.0) && (proj_v1_v2 > 0.0);
    //bool headed_towards = proj_v1_v1v2;//true;//relative_velocity > 0.0;
    // TODO detect collision. If there is one, update velocity
    if (headed_towards && (distance(p1, p2) < (body1.radius + body2.radius))) {
        //collision
        Vector3 v1_p = v1 - v2;
        Vector3 v2_p = Vector3::Zero();

        Vector3 d = normalize(p2 - p1);
        Vector3 v2_pp = 2.0 * d * (m1 / (m1 + m2)) * (dot(v1_p, d));
        
        Vector3 u2 = v2 + v2_pp;
        Vector3 u1 = ((m1 * v1) + (m2 * v2) - (m2 * u2)) / m1;

        body1.velocity = u1;
        body2.velocity = u2;

        printf("sphere collision occurred\n");
        return true;
    } else {
        return false;
        //return false;
    }
}

bool collides( SphereBody& body1, TriangleBody& body2, real_t collision_damping )
{
    // TODO detect collision. If there is one, update velocity
    Vector3 p1 = body1.position;
    Vector3 p2 = body2.position;
    Vector3 v1 = body1.velocity;
    Vector3 vtx_a = body2.vertices[0];
    Vector3 vtx_b = body2.vertices[1];
    Vector3 vtx_c = body2.vertices[2];

    Vector3 n =  normalize(cross(vtx_b - vtx_a, vtx_c - vtx_a));
    //is n normalized?
    
    Vector3 a = p1 - p2;
    real_t d = dot(a, n);
    Vector3 p_p = p1 - (d * n);
    
    Vector3 n_a = normalize(cross(vtx_c - vtx_b, p_p - vtx_b));
    Vector3 n_b = normalize(cross(vtx_a - vtx_c, p_p - vtx_c));
    Vector3 n_c = normalize(cross(vtx_b - vtx_a, p_p - vtx_a));

    real_t n_squared = squared_length(n);
    real_t alpha = dot(n, n_a) / n_squared;
    real_t beta = dot(n, n_b) / n_squared;
    real_t gamma = dot(n, n_c) / n_squared;

    bool headed_towards = dot(v1, -n) > 0.0;

    bool point_within = (0 < alpha && alpha < 1) && (0 < beta && beta < 1)
        && (0 < gamma && gamma < 1);

    if (headed_towards && point_within) {
        if (distance(p_p, p1) < body1.radius) {
            Vector3 u = body1.velocity - (2.0 * (dot(body1.velocity, n)) * n);
            body1.velocity = u;
            printf("triangle collision occurred\n");
            return true;
        }
    } else if (headed_towards) {
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
            Vector3 u = body1.velocity - (2.0 * (dot(body1.velocity, n)) * n);
            body1.velocity = u;
            printf("triangle collision occurred\n");
            return true;
        } 
    }
    return false;
}

bool collides( SphereBody& body1, PlaneBody& body2, real_t collision_damping )
{
    // TODO detect collision. If there is one, update velocity
    Vector3 p1 = body1.position;
    Vector3 p2 = body2.position;
    Vector3 n = body2.normal; //is n normalized?

    Vector3 a = p1 - p2;
    real_t d = dot(a, n);

    if (abs(d) < body1.radius) {
        //collision
        /*printf("before %lf, %lf, %lf \n", body1.velocity.x, body1.velocity.y, body1.velocity.z); */ 

        Vector3 u = body1.velocity - (2.0 * (dot(body1.velocity, n)) * n);
        body1.velocity = u;
        printf("plane collision occurred\n");

        /*printf("after %lf, %lf, %lf \n", body1.velocity.x, body1.velocity.y, body1.velocity.z);  */
        return true;
    } else {
        //no collision, do nothing
        return false;
    }
}

}
