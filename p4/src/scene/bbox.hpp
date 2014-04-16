/**
 * @file bbox.hpp
 * @brief Class defnition for Bbox.
 *
 * @author Jing Xiao (jingxiao)
 */

#ifndef _462_SCENE_BBOX_HPP_
#define _462_SCENE_BBOX_HPP_

#include "scene/scene.hpp"

namespace _462 {

struct BboxComp {
    BboxComp(Geometry* const* geometry_list, size_t axis) : 
        geometry_list(geometry_list), axis(axis) { }

    bool operator()(size_t i, size_t j) {

        if (axis == 1) {
            return (geometry_list[i]->position.x < 
                geometry_list[j]->position.x);

        } else if (axis == 2) {
            return (geometry_list[i]->position.y <
                geometry_list[j]->position.y);
        } else {
            return (geometry_list[i]->position.z <
                geometry_list[j]->position.z);
        }
    }
    
private:
    Geometry* const* geometry_list;
    size_t axis;
};

/**
 * A bbox
 */
class Bbox : public Geometry
{
public:
    virtual void intersects_ray(Ray r, IntersectInfo& intsec, 
        size_t geom_index) const;
    
    real_t get_sa();
    bool hits_bbox(Ray r) const;
    void initialize_bbox(std::vector< size_t > indices, 
        Geometry* const* geom_list);
    std::vector< size_t > sort_geom_indices(size_t axis);
    real_t sum_left_right_volumes(std::vector< size_t > indices);
    void set_bounds();
    
    Bbox* left;
    Bbox* right;
    std::vector< size_t > geom_indices;
    real_t bx_min, bx_max, by_min, by_max, bz_min, bz_max;
    Geometry* const* geometry_list;
    
    Bbox();
    virtual ~Bbox();
    virtual void render() const {}

    /*
    real_t radius;
    const Material* material;

    Bbox();
    virtual ~Bbox();
    virtual void render() const;
    virtual Color3 compute_color(IntersectInfo& intsec, ColorInfo& colinf) const;
    virtual Color3 compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const;
    virtual Color3 get_specular(IntersectInfo& intsec) const;
    virtual real_t get_refractive_index(IntersectInfo& intsec) const;
    */
};

} /* _462 */

#endif /* _462_SCENE_BBOX_HPP_ */

