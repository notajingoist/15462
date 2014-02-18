#include "scene/mesh.hpp"

namespace _462 {

bool Mesh::subdivide()
{
    /*
      You should implement loop subdivision here.

      Triangles are stored in an std::vector<MeshTriangle> in 'triangles'.
      Vertices are stored in an std::vector<MeshVertex> in 'vertices'.

      Check mesh.hpp for the Mesh class definition.
     */
/*
    even_vertices_size = vertices.size();

    build_adjacency_structure();
    first_pass();
    second_pass();    
*/
    std::cout << "Subdivision not yet implemented" << std::endl;
    return true;
}

void Mesh::build_adjacency_structure()
{/*
    unsigned int original_size = triangles.size();
    for (int i = 0; i < original_size; i++) {
        unsigned int i0 = triangles[i].vertices[0];
        unsigned int i1 = triangles[i].vertices[1];
        unsigned int i2 = triangles[i].vertices[2];
        
        Vector3 v0 = vertices[i0].position;
        Vector3 v1 = vertices[i1].position;
        Vector3 v2 = vertices[i2].position;

        adjacent_vertices
    }
*/
}

void Mesh::first_pass() 
{
/*
    //update vertices list with m new odd vertices
    //update triangles list with 

    //even vertices = vertices.size() before modifying with odd vertices
    
    for ab in edges
        
        faces = get_adjacent_faces(ab);
        leftFace = get_left_edge(ab);
        rightFace = get_right_edge(ab);

        leftE1 = leftFace.pred;
        leftE2 = leftFace.succ;

        rightE1 = rightFace.pred;
        rightE2 = rightFace.succ;
        if faces.size() > 1 then it's interior
            //interior case
            if (leftE1.start != a && leftE1.start != b) {
                c = leftE1.start;
                Vector3 new_v = create_interior_odd(a, b, c, d); 
                leftE1.end = 
            } else {
                c = leftE1.end;
            }

            if (rightE1.start != a && rightE1.start != b) {
                d = rightE1.start;
            } else {
                d = rightE1.end;
            }
                    else it's boundary
            //boundary case
*/
}

Vector3 Mesh::create_interior_odd(real_t a, real_t b, 
    real_t c, real_t d)
{
/*
    return (3.0/8.0) * (a + b) + (1.0/8.0) * (c + d);
*/
}

void Mesh::second_pass()
{
/*
    //for i=0; i < even_vertices.size() [aka size of unmodified vertices]
    //apply even vertices formula, change
    //vertices[i].position
*/
}


} /* _462 */













