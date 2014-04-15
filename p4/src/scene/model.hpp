/**
 * @file model.hpp
 * @brief Model class
 *
 * @author Eric Butler (edbutler)
 */

#ifndef _462_SCENE_MODEL_HPP_
#define _462_SCENE_MODEL_HPP_

#include "scene/scene.hpp"
#include "scene/mesh.hpp"

namespace _462 {

/**
 * A mesh of triangles.
 */
class Model : public Geometry
{
public:

    const Mesh* mesh;
    const Material* material;

    Model();
    virtual ~Model();

    virtual void render() const;
   
    virtual void find_min_max(real_t& x_min, real_t& x_max, real_t& y_min, 
        real_t& y_max, real_t& z_min, real_t& z_max) const;
    virtual void intersects_ray(Ray r, IntersectInfo& intsec, 
        size_t geom_index) const; 
    virtual Color3 compute_color(IntersectInfo& intsec, ColorInfo& colinf) const;
    virtual Color3 compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const;
    virtual Color3 get_specular(IntersectInfo& intsec) const;
    virtual real_t get_refractive_index(IntersectInfo& intsec) const;
};


} /* _462 */

#endif /* _462_SCENE_MODEL_HPP_ */

