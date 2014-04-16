/**
 * @file scene.hpp
 * @brief Class definitions for scenes.
 *
 */

#ifndef _462_SCENE_SCENE_HPP_
#define _462_SCENE_SCENE_HPP_
#define SLOP 0.001

#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "math/matrix.hpp"
#include "math/random462.hpp"
#include "math/camera.hpp"
#include "scene/material.hpp"
#include "scene/mesh.hpp"
//#include "scene/bbox.hpp"
//#include "p4/raytracer.hpp"
//#include "scene/bvh.hpp"
#include "ray.hpp"
#include <string>
#include <vector>
#include <cfloat>

namespace _462 {
class Scene;
struct IntersectInfo {
    Vector3 e;
    Vector3 d;

    size_t geom_index;
    size_t tri_index;
    
    real_t t_hit;
    Vector3 n_hit;
    
    bool intersection_found;
    
    real_t alpha;
    real_t beta;
    real_t gamma;

    //Geometry* const* geometry_list;
};

struct ColorInfo {
    const Scene* scene;
    Vector3 p;
    Color3 tp;
    Color3 kd;
    size_t num_samples;
};


//struct ColorInfo;
class Geometry
{
public:
    Geometry();
    virtual ~Geometry();

    /*
       World transformation are applied in the following order:
       1. Scale
       2. Orientation
       3. Position
    */

    // The world position of the object.
    Vector3 position;

    // The world orientation of the object.
    // Use Quaternion::to_matrix to get the rotation matrix.
    Quaternion orientation;

    // The world scale of the object.
    Vector3 scale;

    // Inverse transformation matrix
	Matrix4 invMat;
    // Normal transformation matrix
	Matrix3 normMat;

    //local to world
    Matrix4 mat;

    /**
     * Renders this geometry using OpenGL in the local coordinate space.
     */
    virtual void render() const = 0;

    virtual void intersects_ray(Ray r, IntersectInfo& intsec, 
        size_t geom_index) const;
    virtual void find_min_max(real_t& x_min, real_t& x_max, real_t& y_min, 
        real_t& y_max, real_t& z_min, real_t& z_max) const;
    
    virtual Color3 compute_kd(IntersectInfo& intsec, ColorInfo& colinf) const;
    virtual Color3 compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const;
    virtual Color3 get_specular(IntersectInfo& intsec) const;
    virtual real_t get_refractive_index(IntersectInfo& intsec) const;
	bool initialize();

    static real_t get_max(real_t a, real_t b, real_t c);
    static real_t get_min(real_t a, real_t b, real_t c);

    static void initialize_intsec_info(IntersectInfo& intsec);
    static void intersects_tri_vertices(Ray r, IntersectInfo&, 
        size_t geom_index, Vector3 vtx_a_pos, Vector3 vtx_b_pos, Vector3 vtx_c_pos, 
            Vector3 vtx_a_n, Vector3 vtx_b_n, Vector3 vtx_c_n, Matrix4 invMat, 
            Matrix3 normMat);

 };


struct SphereLight
{
    struct Attenuation
    {
        real_t constant;
        real_t linear;
        real_t quadratic;
    };

    SphereLight();

	bool intersect(const Ray& r, real_t& t);

    // The position of the light, relative to world origin.
    Vector3 position;
    // The color of the light (both diffuse and specular)
    Color3 color;
    // attenuation
    Attenuation attenuation;
	real_t radius;
};

/**
 * The container class for information used to render a scene composed of
 * Geometries.
 */
class Scene
{
public:

    /// the camera
    Camera camera;
    /// the background color
    Color3 background_color;
    /// the amibient light of the scene
    Color3 ambient_light;
    /// the refraction index of air
    real_t refractive_index;

    /// Creates a new empty scene.
    Scene();

    /// Destroys this scene. Invokes delete on everything in geometries.
    ~Scene();

	bool initialize();

    // accessor functions
    Geometry* const* get_geometries() const;
    size_t num_geometries() const;
    const SphereLight* get_lights() const;
    size_t num_lights() const;
    Material* const* get_materials() const;
    size_t num_materials() const;
    Mesh* const* get_meshes() const;
    size_t num_meshes() const;

    /// Clears the scene, and invokes delete on everything in geometries.
    void reset();

    // functions to add things to the scene
    // all pointers are deleted by the scene upon scene deconstruction.
    void add_geometry( Geometry* g );
    void add_material( Material* m );
    void add_mesh( Mesh* m );
    void add_light( const SphereLight& l );

    void shoot_ray(Ray r, IntersectInfo& intsec) const;
    
private:

    typedef std::vector< SphereLight > SphereLightList;
    typedef std::vector< Material* > MaterialList;
    typedef std::vector< Mesh* > MeshList;
    typedef std::vector< Geometry* > GeometryList;

    // list of all lights in the scene
    SphereLightList point_lights;
    // all materials used by geometries
    MaterialList materials;
    // all meshes used by models
    MeshList meshes;
    // list of all geometries. deleted in dctor, so should be allocated on heap.
    GeometryList geometries;

private:

    // no meaningful assignment or copy
    Scene(const Scene&);
    Scene& operator=(const Scene&);

};

} /* _462 */

#endif /* _462_SCENE_SCENE_HPP_ */
