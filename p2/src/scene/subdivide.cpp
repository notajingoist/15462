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
    build_adjacency_structure();
    std::cout << "Subdivision not yet implemented" << std::endl;
    return true;
}

void Mesh::build_edge(WingedEdge & e, unsigned int curr_index,
    unsigned int prev_index, unsigned int next_index, 
    unsigned int start_index, unsigned int end_index)

/*WingedEdge prev, WingedEdge next,
    WingedVertex start, WingedVertex end, WingedEdge sym) */
{
    
    unsigned int sym_index = -1;
    for (unsigned int i = 0; i < edge_list.size(); i++) {
        /*
        printf("start_index: %d, edge_list[i] end_index: %d\n", 
            start_index, edge_list[i].end_index);
        printf("end_index: %d, edge_list[i] start_index: %d\n\n",
            end_index, edge_list[i].start_index);
        */
        if ((edge_list[i].start_index == end_index) 
            && (edge_list[i].end_index == start_index)) {
            //found symmetric edge
            sym_index = i;
            //printf("found sym_index %d\n", sym_index);
            edge_list[i].sym_index = curr_index;
        }
    }

    e.curr_index = curr_index;
    e.prev_index = prev_index;
    e.next_index = next_index;
    e.start_index = start_index;
    e.end_index = end_index;
    e.sym_index = sym_index;
    e.is_subdivided = false;

    edge_list.push_back(e);
}

void Mesh::build_vertex(WingedVertex & v, 
    unsigned int edge_index, unsigned int vertices_index) 
{
    bool exists = false;
    for (unsigned int i = 0; i < vertex_list.size(); i++) {
        if (vertex_list[i].vertices_index == vertices_index) { 
            exists = true;
        }
    }

    if (!exists) {
        v.edge_index = edge_index;
        v.vertices_index = vertices_index;
        vertex_list.push_back(v);
    }
}

void Mesh::build_face(WingedFace & f, 
    unsigned int edge_index, unsigned int triangles_index)
{
    f.edge_index = edge_index;
    f.triangles_index = triangles_index;

    face_list.push_back(f);
}

void Mesh::build_adjacency_structure()
{

    for (unsigned int i = 0; i < triangles.size(); i++) {
        unsigned int i0 = triangles[i].vertices[0];
        unsigned int i1 = triangles[i].vertices[1];
        unsigned int i2 = triangles[i].vertices[2];
        unsigned int e_index = edge_list.size();
        unsigned int v_index = vertex_list.size();
    
        WingedEdge e0, e1, e2; // 0 1 2 
        WingedVertex v0, v1, v2; // 0 1 2
        WingedFace f;
        
        build_edge(e0, e_index, e_index+2, e_index+1, 
                    i0, i1); 
        build_edge(e1, e_index+1, e_index, e_index+2, 
                    i1, i2);
        build_edge(e2, e_index+2, e_index+1, e_index,
                    i2, i0);

        build_vertex(v0, e0.curr_index, i0);
        build_vertex(v1, e1.curr_index, i1);
        build_vertex(v2, e2.curr_index, i2);
        
        build_face(f, e0.curr_index, i); 
    }
/*
    for (unsigned int i = 0; i < edge_list.size(); i++) {
        printf("start: %d, end:  %d\n", edge_list[i].start_index, 
            edge_list[i].end_index);
        printf("curr_index: %d\n", edge_list[i].curr_index);
        printf("sym_index: %d\n", edge_list[i].sym_index);
    }
*/
/*
    for (unsigned int i = 0; i < vertex_list.size(); i++) {
        printf("edge
    }
  */  
/*
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













