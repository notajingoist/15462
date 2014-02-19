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
    first_pass();
    //std::cout << "Subdivision not yet implemented" << std::endl;
    return true;
}

void Mesh::build_edge(WingedEdge & e, unsigned int curr_index,
    unsigned int prev_index, unsigned int next_index, 
    unsigned int start_index, unsigned int end_index,
    unsigned int face_index)
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
    e.face_index = face_index;
    e.odd_vertex_index = -1;

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
                    i0, i1, i); 
        build_edge(e1, e_index+1, e_index, e_index+2, 
                    i1, i2, i);
        build_edge(e2, e_index+2, e_index+1, e_index,
                    i2, i0, i);

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

void Mesh::add_odd_vertices() 
{
     for (unsigned int i = 0; i < edge_list.size(); i++) {
        WingedEdge e = edge_list[i];
        if (!e.is_subdivided) {
            unsigned int a_index = 
                vertex_list[e.start_index].vertices_index;
            unsigned int b_index = 
                vertex_list[e.end_index].vertices_index; 
            Vector3 a = vertices[a_index].position;
            Vector3 b = vertices[b_index].position;

            MeshVertex new_mesh_vertex;
            unsigned int curr_vertices_index = vertices.size();
            unsigned int curr_vertex_list_index = vertex_list.size();
            
            if (e.sym_index != -1) {
                //interior
                WingedEdge e_sym = edge_list[e.sym_index];

                unsigned int c_index = 
                    vertex_list[e.next_index].vertices_index;
                unsigned int d_index = 
                    vertex_list[e_sym.next_index].vertices_index;
                Vector3 c = vertices[c_index].position;
                Vector3 d = vertices[d_index].position;

                new_mesh_vertex.position = create_interior_odd(a, b, c, d);
                vertices.push_back(new_mesh_vertex);

                e_sym.odd_vertex_index = curr_vertex_list_index; 
                e_sym.is_subdivided = true;
            } else {
                //boundary
                new_mesh_vertex.position = create_boundary_odd(a, b);
                vertices.push_back(new_mesh_vertex); 
            }

            WingedVertex new_vertex;
            build_vertex(new_vertex, e.curr_index, curr_vertices_index);
            
            e.odd_vertex_index = curr_vertex_list_index;
            e.is_subdivided = true;
    
        } else {
            //edge already subdivided 
        }
    }   
}

void Mesh::add_triangle(unsigned int v0_index, unsigned int v1_index,
    unsigned int v2_index) 
{
    MeshTriangle t;

    t.vertices[0] = v0_index;
    t.vertices[1] = v1_index;
    t.vertices[2] = v2_index;

    triangles.push_back(t);
}

void Mesh::update_triangles()
{
    for (unsigned int i = 0; i < face_list.size(); i++) {
        WingedFace f = face_list[i];
        WingedEdge e0 = edge_list[f.edge_index];
        WingedEdge e1 = edge_list[edge_list[f.edge_index].next_index];
        WingedEdge e2 = edge_list[edge_list[f.edge_index].prev_index];

        unsigned int odd_v0 = vertex_list[e0.odd_vertex_index].vertices_index;
        unsigned int odd_v1 = vertex_list[e1.odd_vertex_index].vertices_index;
        unsigned int odd_v2 = vertex_list[e2.odd_vertex_index].vertices_index;
    
        /* update middle triangle (replace old triangle's indices into 
         * vertices with new indices that correspond to the newly created
         * odd vertices 
         */
        MeshTriangle t_mid = triangles[f.triangles_index];
        t_mid.vertices[0] = odd_v0; 
        t_mid.vertices[1] = odd_v1;
        t_mid.vertices[2] = odd_v2;
       
        unsigned int even_v0 = vertex_list[e0.start_index].vertices_index; 
        unsigned int even_v1 = vertex_list[e0.end_index].vertices_index;
        unsigned int even_v2 = 
            vertex_list[edge_list[e0.next_index].end_index].vertices_index;
 
        /* add 3 new triangles */
        add_triangle(even_v0, odd_v0, odd_v2);
        add_triangle(odd_v0, even_v1, odd_v1);
        add_triangle(odd_v2, odd_v1, even_v2);
    }
}

void Mesh::first_pass() 
{
    add_odd_vertices();
    //update_triangles();
}

Vector3 Mesh::create_interior_odd(Vector3 a, Vector3 b,
    Vector3 c, Vector3 d)
{
    return (3.0/8.0) * (a + b) + (1.0/8.0) * (c + d);
}

Vector3 Mesh::create_boundary_odd(Vector3 a, Vector3 b)
{
    return (1.0/2.0) * (a + b);
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













