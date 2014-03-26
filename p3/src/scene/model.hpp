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
    virtual void intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const; 
    void tri_intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index,
                            MeshVertex a, MeshVertex b, MeshVertex c) const;

    virtual Color3 compute_color(IntersectInfo& intsec) const;
};


} /* _462 */

#endif /* _462_SCENE_MODEL_HPP_ */

