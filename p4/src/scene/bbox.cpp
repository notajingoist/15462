/**
 * @file bbox.cpp
 * @brief Function defnitions for the Bbox class.
 *
 * @author Jing Xiao (jingxiao)
 */

#include "scene/bbox.hpp"
#include "application/opengl.hpp"

namespace _462 {

Bbox::Bbox() {}

Bbox::~Bbox() {}

void Bbox::initialize_bbox(std::vector< size_t > indices,
    Geometry* const* geom_list)
{
    geometry_list = geom_list;
    geom_indices = indices;
    
    if (geom_indices.size() <= 1) {
        //left and right remain NULL
        left = NULL;
        right = NULL;

    } else {
        //do I need this?
        std::vector< size_t > left_indices;
        std::vector< size_t > right_indices;
        
        left = new Bbox();
        right = new Bbox();

        if (geom_indices.size() == 2) {
            left_indices.push_back(geom_indices[0]);
            right_indices.push_back(geom_indices[1]);
        } else {
            std::vector< size_t > indices_x = sort_geom_indices(1);
            std::vector< size_t > indices_y = sort_geom_indices(2);
            std::vector< size_t > indices_z = sort_geom_indices(3);

            real_t vol_x = sum_left_right_volumes(indices_x);
            real_t vol_y = sum_left_right_volumes(indices_y);
            real_t vol_z = sum_left_right_volumes(indices_z);

            real_t min_vol = get_min(vol_x, vol_y, vol_z);

            if (min_vol == vol_x) {
                geom_indices = indices_x;
            } else if (min_vol == vol_y) {
                geom_indices = indices_y;
            } else {
                geom_indices = indices_z;
            }

            /*
            if ((vol_x <= vol_y) && (vol_x <= vol_z)) {
                geom_indices = indices_x;
            } else if ((vol_y <= vol_x) && (vol_y <= vol_z)) {
                geom_indices = indices_y;
            } else {
                geom_indices = indices_z;
            }
            */

            for (size_t i = 0; i < (geom_indices.size()/2); i++) {
                left_indices.push_back(geom_indices[i]);
            }
            for (size_t i = (geom_indices.size()/2); 
                i < geom_indices.size(); i++) {
                right_indices.push_back(geom_indices[i]);
            }
        }

        left->initialize_bbox(left_indices, geometry_list);
        right->initialize_bbox(right_indices, geometry_list);
    }

    set_bounds();
}

real_t Bbox::sum_left_right_volumes(std::vector< size_t > indices)
{
    std::vector< size_t > left_indices;
    std::vector< size_t > right_indices;

    for (size_t i = 0; i < (indices.size()/2); i++) {
        left_indices.push_back(indices[i]);
    }

    for (size_t i = (indices.size()/2); 
        i < geom_indices.size(); i++) {
        right_indices.push_back(indices[i]);
    }

    left->geom_indices = left_indices;
    right->geom_indices = right_indices;

    real_t left_vol = left->get_volume();
    real_t right_vol = right->get_volume();

    return left_vol + right_vol;
}


real_t Bbox::get_volume()
{
    /*real_t volume = 0.0;
    for (size_t i = 0; i < geom_indices.size(); i++) {
        volume += geometry_list[indices[i]]->get_volume(); 
    }*/

    return (bx_max - bx_min) * (by_max - by_min) * (bz_max - bz_min);
}

std::vector< size_t > Bbox::sort_geom_indices(size_t axis)
{
    std::vector< size_t > indices = geom_indices;
    std::sort (indices.begin(), indices.end(), BboxComp(geometry_list, axis));
    return indices;
}

void Bbox::set_bounds()
{

    bool first_geom = true;
    real_t x_min, x_max, y_min, y_max, z_min, z_max;
    for (size_t i = 0; i < geom_indices.size(); i++) {
        geometry_list[geom_indices[i]]->find_min_max(x_min, x_max, 
            y_min, y_max, z_min, z_max);
        if (first_geom) {
            bx_min = x_min;
            bx_max = x_max;
            by_min = y_min;
            by_max = y_max;
            bz_min = z_min;
            bz_max = z_max;
        } else {
            bx_min = std::min(bx_min, x_min);
            bx_max = std::max(bx_max, x_max);
            by_min = std::min(by_min, y_min);
            by_max = std::max(by_max, y_max);
            bz_min = std::min(bz_min, z_min);
            bz_max = std::max(bz_max, z_max);
        }
        first_geom = false;
    }
}

bool Bbox::hits_bbox(Ray r) const
{
    real_t tx_min, tx_max, ty_min, ty_max, tz_min, tz_max;
    real_t t_min, t_max;

    real_t recip_xd = 1.0/r.d.x;
    real_t recip_yd = 1.0/r.d.y;
    real_t recip_zd = 1.0/r.d.z;
    
    if (recip_xd >= 0) {
        tx_min = (bx_min - r.e.x) * recip_xd;
        tx_max = (bx_max - r.e.x) * recip_xd;
    } else {
        tx_min = (bx_max - r.e.x) * recip_xd;
        tx_max = (bx_min - r.e.x) * recip_xd;
    }
    
    if (recip_yd >= 0) {
        ty_min = (by_min - r.e.y) * recip_yd;
        ty_max = (by_max - r.e.y) * recip_yd;
    } else {
        ty_min = (by_max - r.e.y) * recip_yd;
        ty_max = (by_min - r.e.y) * recip_yd;
    }

    t_min = tx_min;
    t_max = tx_max;

    if ((t_min > ty_max) || (ty_min > t_max)) {
        return false;
    }
    
    if (ty_min > t_min) {
        t_min = ty_min;
    }
    if (ty_max < t_max) {
        t_max = ty_max;
    }

    if (recip_zd >= 0) {
        tz_min = (bz_min - r.e.z) * recip_zd;
        tz_max = (bz_max - r.e.z) * recip_zd;
    } else {
        tz_min = (bz_max - r.e.z) * recip_zd;
        tz_max = (bz_min - r.e.z) * recip_zd;
    }

    if ((t_min > tz_max) || (tz_min > t_max)) {
        return false;
    }

    if (tz_min > t_min) {
        t_min = tz_min;
    }
    if (tz_max < t_max) {
        t_max = tz_max;
    }
    
    //if ((tx_min > ty_max) || (ty_min > tx_max)) {
    //    return false;
    //}

    return true;
}

void Bbox::intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const
{
    //calculations in world coordinates
    
    //if hits box, then check left and right
    //else no left or right, check the one geometry in geometry list
    //else doesn't hit box, then no intersection found

    if (hits_bbox(r)) {
        //intsec.intersection_found = true;
        
        if (left != NULL && right != NULL) {
            IntersectInfo right_intsec, left_intsec;
            initialize_intsec_info(right_intsec);
            initialize_intsec_info(left_intsec);
            
            //dummy geom_index value = 0
            left->intersects_ray(r, left_intsec, 0);
            right->intersects_ray(r, right_intsec, 0);

            if (left_intsec.intersection_found &&
                right_intsec.intersection_found) {
                intsec = (left_intsec.t_hit < right_intsec.t_hit) 
                    ? left_intsec : right_intsec;
            } if (left_intsec.intersection_found) {
                intsec = left_intsec;    
            } else {
                intsec = right_intsec;
            }
            
        } else if (left != NULL) {
            left->intersects_ray(r, intsec, 0);
        } else if (right != NULL) {
            right->intersects_ray(r, intsec, 0);
        } else {
            //leaf node, so there should only be 1 geometry index in geom_indices
            for (size_t i = 0; i < geom_indices.size(); i++) {
                 geometry_list[geom_indices[i]]->intersects_ray(r, intsec,
                    geom_indices[i]);
            }
        }
    } else {
        //already false
        intsec.intersection_found = false;
    }
    
}

} /* _462 */
