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
	make_transformation_matrix(&mat, position, orientation, scale);
	make_normal_matrix(&normMat, mat);

	return true;
}

void Geometry::initialize_intsec_info(IntersectInfo& intsec)
{
    intsec.intersection_found = false;
    intsec.t_hit = -1;
}

void Geometry::intersects_tri_vertices(Ray r, IntersectInfo& intsec, 
    size_t geom_index, Vector3 vtx_a_pos, Vector3 vtx_b_pos, Vector3 vtx_c_pos,
    Vector3 vtx_a_n, Vector3 vtx_b_n, Vector3 vtx_c_n, Matrix4 invMat, 
    Matrix3 normMat)
{
    Vector3 trans_e = invMat.transform_point(r.e); 
    Vector3 trans_d = invMat.transform_vector(r.d);
    
    real_t a = vtx_a_pos.x - vtx_b_pos.x; //xa - xb
    real_t b = vtx_a_pos.y - vtx_b_pos.y; //ya - yb
    real_t c = vtx_a_pos.z - vtx_b_pos.z; //za - zb
    real_t d = vtx_a_pos.x - vtx_c_pos.x; //xa - xc
    real_t e = vtx_a_pos.y - vtx_c_pos.y; //ya - yc
    real_t f = vtx_a_pos.z - vtx_c_pos.z; //za - zc
    real_t g = trans_d.x; //xd
    real_t h = trans_d.y; //yd
    real_t i = trans_d.z; //zd
    real_t j = vtx_a_pos.x - trans_e.x; //xa - xe
    real_t k = vtx_a_pos.y - trans_e.y; //ya - ye
    real_t l = vtx_a_pos.z - trans_e.z; //za - ze

    real_t M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
    if (M == 0.0) {
        return;
    }

    real_t t = (-1.0)*(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/M;
    if (t <= SLOP) { //<= ? 
        return;
    }

    real_t gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/M; 
    if ((gamma < 0.0) || (gamma > 1.0)) {
        return;
    }

    real_t beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
    if ((beta < 0.0) || (beta > (1.0 - gamma))) {
        return;
    }

    real_t alpha = 1.0 - gamma - beta;
    Vector3 pre_n = (alpha*vtx_a_n) + (beta*vtx_b_n) 
        + (gamma*vtx_c_n);
    Vector3 n = normalize(normMat*(normalize(pre_n)));
    if (!intsec.intersection_found || (t < intsec.t_hit)) {
        intsec.e = r.e;
        intsec.d = r.d;
        intsec.intersection_found = true;
        intsec.t_hit = t;
        intsec.n_hit = n;
        intsec.geom_index = geom_index;
        intsec.gamma = gamma;
        intsec.beta = beta;
        intsec.alpha = alpha;
    }
}

Color3 Geometry::compute_tp(IntersectInfo& intsec, ColorInfo& colinf) const
{
    return Color3::White();
}

Color3 Geometry::compute_kd(IntersectInfo& intsec, ColorInfo& colinf) const
{
    return Color3::Black();
}

real_t Geometry::get_max(real_t a, real_t b, real_t c)
{
    real_t max = a;
    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }
    return max;
}

real_t Geometry::get_min(real_t a, real_t b, real_t c)
{
    real_t min = a;
    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }
    return min;
}

void Geometry::find_min_max(real_t& x_min, real_t& x_max, real_t& y_min, 
    real_t& y_max, real_t& z_min, real_t& z_max) const 
{
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

