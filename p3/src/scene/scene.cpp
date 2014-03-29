/**
 * @file scene.cpp
 * @brief Function definitions for scenes.
 *
 * @author Eric Butler (edbutler)
 * @author Kristin Siu (kasiu)
 */

#include "scene/scene.hpp"

namespace _462 {


Geometry::Geometry():
    position(Vector3::Zero()),
    orientation(Quaternion::Identity()),
    scale(Vector3::Ones())
{

}

Geometry::~Geometry() { }

bool Geometry::initialize()
{
	make_inverse_transformation_matrix(&invMat, position, orientation, scale);
	Matrix4 mat;
	make_transformation_matrix(&mat, position, orientation, scale);
	make_normal_matrix(&normMat, mat);

	return true;
}

Color3 Geometry::compute_lights_color(IntersectInfo& intsec, ColorInfo& colinf) const
{
    Color3 c_all_lights = Color3::Black();
    for (size_t i = 0; i < colinf.scene->num_lights(); i++) {
        for (size_t iter = 0; iter < colinf.num_samples; iter++) {
            real_t x = random_gaussian();
            real_t y = random_gaussian();
            real_t z = random_gaussian();
            
            Vector3 random_p = normalize(Vector3(x, y, z));
            Vector3 light_pos = (colinf.scene->get_lights()[i].radius*random_p) 
                + (colinf.scene->get_lights()[i].position);

            Color3 c = colinf.scene->get_lights()[i].color;
            real_t ac = colinf.scene->get_lights()[i].attenuation.constant;
            real_t al = colinf.scene->get_lights()[i].attenuation.linear;
            real_t aq = colinf.scene->get_lights()[i].attenuation.quadratic;

            //Vector3 light_pos = colinf.scene->get_lights()[i].position;
            Vector3 light_dir = normalize(light_pos - colinf.p); //L
            real_t light_dist = distance(colinf.p, light_pos); //d
            Ray shadow_r = Ray(colinf.p, light_dir);
            
            real_t b = 1;
            IntersectInfo b_intsec;
            Raytracer::initialize_intsec_info(b_intsec);
            colinf.scene->shoot_ray(shadow_r, b_intsec);
            if (b_intsec.intersection_found) {
                Vector3 obj_pos = colinf.p + (b_intsec.t_hit * light_dir);
                real_t obj_dist = distance(colinf.p, obj_pos);
                if (obj_dist <= light_dist) {
                    b = 0;
                }
            } 

            real_t n_dot_l = dot(intsec.n_hit, light_dir);
            real_t max_n_dot_l = (n_dot_l > 0) ? n_dot_l : 0;

            Color3 ci = c*(1.0/(ac + (light_dist*al) + (light_dist*light_dist*aq)));

            c_all_lights += b*ci*colinf.kd*max_n_dot_l; 
        }
    }
    return c_all_lights*(real_t(1)/colinf.num_samples);
}

Color3 Geometry::compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const
{
    return Color3::White();
}

Color3 Geometry::compute_color(IntersectInfo& intsec, ColorInfo& colinf) const
{
    return Color3::Black();
}

void Geometry::intersects_ray(Ray r, IntersectInfo& intsec, size_t geom_index) const
{
}

Color3 Geometry::get_specular(IntersectInfo& intsec) const
{
    return Color3::Black();
}

real_t Geometry::get_refractive_index(IntersectInfo& intsec) const
{
    return 0;
}

SphereLight::SphereLight():
    position(Vector3::Zero()),
    color(Color3::White()),
	radius(real_t(0))
{
    attenuation.constant = 1;
    attenuation.linear = 0;
    attenuation.quadratic = 0;
}

Scene::Scene()
{
    reset();
}

Scene::~Scene()
{
    reset();
}

bool Scene::initialize()
{
	bool res = true;
	for (unsigned int i = 0; i < num_geometries(); i++)
		res &= geometries[i]->initialize();
	return res;
}


Geometry* const* Scene::get_geometries() const
{
    return geometries.empty() ? NULL : &geometries[0];
}

size_t Scene::num_geometries() const
{
    return geometries.size();
}

const SphereLight* Scene::get_lights() const
{
    return point_lights.empty() ? NULL : &point_lights[0];
}

size_t Scene::num_lights() const
{
    return point_lights.size();
}

Material* const* Scene::get_materials() const
{
    return materials.empty() ? NULL : &materials[0];
}

size_t Scene::num_materials() const
{
    return materials.size();
}

Mesh* const* Scene::get_meshes() const
{
    return meshes.empty() ? NULL : &meshes[0];
}

size_t Scene::num_meshes() const
{
    return meshes.size();
}

void Scene::reset()
{
    for ( GeometryList::iterator i = geometries.begin(); i != geometries.end(); ++i ) {
        delete *i;
    }
    for ( MaterialList::iterator i = materials.begin(); i != materials.end(); ++i ) {
        delete *i;
    }
    for ( MeshList::iterator i = meshes.begin(); i != meshes.end(); ++i ) {
        delete *i;
    }

    geometries.clear();
    materials.clear();
    meshes.clear();
    point_lights.clear();

    camera = Camera();

    background_color = Color3::Black();
    ambient_light = Color3::Black();
    refractive_index = 1.0;
}

void Scene::add_geometry( Geometry* g )
{
    geometries.push_back( g );
}

void Scene::add_material( Material* m )
{
    materials.push_back( m );
}

void Scene::add_mesh( Mesh* m )
{
    meshes.push_back( m );
}

void Scene::add_light( const SphereLight& l )
{
    point_lights.push_back( l );
}

void Scene::shoot_ray(Ray r, IntersectInfo& intsec) const
{
    for (size_t i = 0; i < num_geometries(); i++) {
        get_geometries()[i]->intersects_ray(r, intsec, i);
    }
}

} /* _462 */

