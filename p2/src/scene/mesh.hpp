/**
 * @file mesh.hpp
 * @brief Mesh class and OBJ loader.
 *
 * @author Eric Butler (edbutler)
 * @author Zeyang Li (zeyangl)
 */

#ifndef _462_SCENE_MESH_HPP_
#define _462_SCENE_MESH_HPP_

#include "math/vector.hpp"

#include <vector>
#include <cassert>

namespace _462 {

struct WingedEdge {
    /*WingedEdge *prev, *next;
    WingedEdge *sym;
    WingedFace *f;
    WingedVertex *start;
    WingedVertex *end;
    WingedVertex *odd;
    bool is_subdivided;
     */
  
    unsigned int curr_index; 
    unsigned int prev_index;
    unsigned int next_index;
    unsigned int sym_index;
    unsigned int start_index;
    unsigned int end_index;
    unsigned int face_index;
    unsigned int odd_vertex_index;
    bool is_subdivided;
};

struct WingedVertex {
    //WingedEdge *e;
    unsigned int edge_index;
    unsigned int vertices_index;
    Vector3 new_pos;
};

struct WingedFace {
    //WingedEdge *e;
    unsigned int edge_index;
    unsigned int triangles_index;
};

struct MeshVertex
{
    //Vector4 color;
    Vector3 position;
    Vector3 normal;
    Vector2 tex_coord;
};

struct MeshTriangle
{
    // index into the vertex list of the 3 vertices
    unsigned int vertices[3];
};

/**
 * A mesh of triangles.
 */
class Mesh
{
public:

    Mesh();
    virtual ~Mesh();

    typedef std::vector< MeshTriangle > MeshTriangleList;
    typedef std::vector< MeshVertex > MeshVertexList;

    // The list of all triangles in this model.
    MeshTriangleList triangles;

    // The list of all vertices in this model.
    MeshVertexList vertices;

    // scene loader stores the filename of the mesh here
    std::string filename;

    bool has_tcoords;
    bool has_normals;
    int has_colors;

    // Loads the model into a list of triangles and vertices.
    bool load();

    // Creates opengl data for rendering and computes normals if needed
    bool create_gl_data();

    bool subdivide();

    // Renders the mesh using opengl.
    void render() const;
private:
    typedef std::vector< float > FloatList;
    typedef std::vector< unsigned int > IndexList;

    typedef std::vector< WingedEdge > W_EdgeList;
    typedef std::vector< WingedVertex > W_VertexList;
    typedef std::vector< WingedFace > W_FaceList;

    // the vertex data used for GL rendering
    FloatList vertex_data;
    // the index data used for GL rendering
    IndexList index_data;

    // prevent copy/assignment
    Mesh( const Mesh& );
    Mesh& operator=( const Mesh& );

    W_EdgeList edge_list;
    W_VertexList vertex_list;
    W_FaceList face_list;

    void first_pass();
    void second_pass();
    void build_edge(WingedEdge & e, unsigned int curr_index,
        unsigned int prev_index, unsigned int next_index,
        unsigned int start_index, unsigned int end_index,
        unsigned int face_index);
    void build_vertex(WingedVertex & v, unsigned int edge_index,
        unsigned int vertices_index);
    void build_face(WingedFace & f, unsigned int edge_index,
        unsigned int triangles_index);

    void build_adjacency_structure();
    Vector3 create_interior_odd(Vector3 a, Vector3 b, 
        Vector3 c, Vector3 d);
    Vector3 create_boundary_odd(Vector3 a, Vector3 b);
    void add_odd_vertices();
    void update_triangles();
    void add_triangle(unsigned int v0_index, unsigned int v1_index,
        unsigned int v2_index);
};


} /* _462 */

#endif /* _462_SCENE_MESH_HPP_ */
