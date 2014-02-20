#include "scene/mesh.hpp"

#include<stdio.h>

namespace _462 {

bool Mesh::subdivide()
{
    /*
      You should implement loop subdivision here.

      Triangles are stored in an std::vector<MeshTriangle> in 'triangles'.
      Vertices are stored in an std::vector<MeshVertex> in 'vertices'.

      Check mesh.hpp for the Mesh class definition.
     */

    unsigned int num_even_vertices = vertices.size();
    build_adjacency_structure();
    first_pass();
    second_pass(num_even_vertices);
    has_normals = false;
    create_gl_data();
   
    vertex_list.clear();
    edge_list.clear();
   
    return true;
}

void Mesh::build_edge(unsigned int curr_index,
    unsigned int prev_index, unsigned int next_index, 
    unsigned int start_index, unsigned int end_index)
{
    WingedEdge e; 
    unsigned int sym_index = -1;
    for (unsigned int i = 0; i < edge_list.size(); i++) {
        WingedEdge other_e = edge_list[i];
        
        //check if this edge is the symmetric edge of e
        //if it is, its start index should = e's end index
        //and its end index should = e's start index
        if ((other_e.start_index == end_index) &&
            (other_e.end_index == start_index)) {
            //found symmetric edge
            sym_index = i;
            edge_list[i].sym_index = curr_index;
        }
    }

    e.curr_index = curr_index;
    e.prev_index = prev_index;
    e.next_index = next_index;
    e.sym_index = sym_index;
    e.start_index = start_index;
    e.end_index = end_index;
    e.odd_vertex_index = -1;
    e.is_subdivided = false;
    e.is_visited = false;
    
    edge_list.push_back(e);
}

void Mesh::build_vertex(unsigned int edge_index, unsigned int vertices_index) 
{
    WingedVertex v;
    v.edge_index = edge_index;
    v.vertices_index = vertices_index;

    vertex_list.push_back(v);
}

void Mesh::set_vertex_edge_index(unsigned int edge_index, 
    unsigned int vertices_index)
{
    vertex_list[vertices_index].edge_index = edge_index;
}


void Mesh::build_adjacency_structure()
{
    //make one-to-one mapping of vertex_list to vertices

    for (unsigned int i = 0; i < vertices.size(); i++) {
        WingedVertex v;
        v.edge_index = -1;
        v.vertices_index = i;
        vertex_list.push_back(v);
    }

    for (unsigned int i = 0; i < triangles.size(); i++) {
        unsigned int i0 = triangles[i].vertices[0];
        unsigned int i1 = triangles[i].vertices[1];
        unsigned int i2 = triangles[i].vertices[2];
        unsigned int e_index = edge_list.size();
        unsigned int v_index = vertex_list.size();
   
        set_vertex_edge_index(e_index, i0);
        set_vertex_edge_index(e_index+1, i1);
        set_vertex_edge_index(e_index+2, i2);
        
        build_edge(e_index, e_index+2, e_index+1, 
                    i0, i1); 
        build_edge(e_index+1, e_index, e_index+2, 
                    i1, i2);
        build_edge(e_index+2, e_index+1, e_index,
                    i2, i0);
    }
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

            if (e.sym_index != -1) {
                //interior
                WingedEdge e_sym = edge_list[e.sym_index];

                unsigned int c_index = 
                    vertex_list[edge_list[e.next_index].end_index]
                    .vertices_index;
                unsigned int d_index = 
                    vertex_list[edge_list[e_sym.next_index].end_index]
                    .vertices_index;
                Vector3 c = vertices[c_index].position;
                Vector3 d = vertices[d_index].position;
                  
                edge_list[e.sym_index].odd_vertex_index = curr_vertices_index;
                edge_list[e.sym_index].is_subdivided = true;
                
                new_mesh_vertex.position = create_interior_odd(a, b, c, d);
            } else {
                //boundary
                new_mesh_vertex.position = create_boundary_odd(a, b);
            }
            
            build_vertex(e.curr_index, curr_vertices_index);

            edge_list[i].odd_vertex_index = curr_vertices_index;
            edge_list[i].is_subdivided = true;
            vertices.push_back(new_mesh_vertex);

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
    triangles.clear();
    for (unsigned int i = 0; i < edge_list.size(); i++) {
        WingedEdge e0 = edge_list[i]; 
        if (!e0.is_visited) {
            //triangles not updated
            WingedEdge e1 = edge_list[e0.next_index];
            WingedEdge e2 = edge_list[e0.prev_index];
            
            unsigned int odd_v0 = 
                vertex_list[e0.odd_vertex_index].vertices_index;
            unsigned int odd_v1 = 
                vertex_list[e1.odd_vertex_index].vertices_index;
            unsigned int odd_v2 = 
                vertex_list[e2.odd_vertex_index].vertices_index;
       
            unsigned int even_v0 = vertex_list[e0.start_index].vertices_index; 
            unsigned int even_v1 = vertex_list[e1.start_index].vertices_index;
            unsigned int even_v2 = vertex_list[e2.start_index].vertices_index;
    
            //add 4 new triangles 
            add_triangle(odd_v0, odd_v1, odd_v2); 
            add_triangle(odd_v2, even_v0, odd_v0);
            add_triangle(odd_v0, even_v1, odd_v1);
            add_triangle(odd_v1, even_v2, odd_v2);

            edge_list[i].is_visited = true;
            edge_list[e0.next_index].is_visited = true;
            edge_list[e0.prev_index].is_visited = true;
        } else {
            //triangles already updated
        }
    }
}

void Mesh::first_pass() 
{
    add_odd_vertices();
    update_triangles();
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

Vector3 Mesh::create_interior_even(WingedVertex v, 
    W_VertexList neighbor_vertex_list) 
{
    unsigned int num_neighbors = neighbor_vertex_list.size();
    real_t squared_component = ((3.0/8.0) + ((1.0/4.0) * 
        cos((2.0*PI)/num_neighbors))); 
    real_t beta = (1.0/num_neighbors) * ((5.0/8.0) - (squared_component
        * squared_component));

    Vector3 neighbor_sum;
    for (unsigned int i = 0; i < num_neighbors; i++) {
        neighbor_sum += 
            vertices[neighbor_vertex_list[i].vertices_index].position;
    }

    Vector3 v_pos = vertices[v.vertices_index].position;
    Vector3 new_vertex = v_pos * (1.0 - (num_neighbors * beta)) 
        + (beta * neighbor_sum); 

    return new_vertex;
}

Vector3 Mesh::create_boundary_even(WingedVertex v, Vector3 a, Vector3 b) 
{
    Vector3 v_pos = vertices[v.vertices_index].position;
    return (1.0/8.0) * (a + b) + (3.0/4.0) * (v_pos);
}

void Mesh::second_pass(unsigned int num_even_vertices)
{
    /* first time iterating through, only update
     * new_pos property for each WingedVertex so that
     * calculations for each even vertex are unaffected 
     */
    for (unsigned int i = 0; i < num_even_vertices; i++) {
        WingedVertex winged_vertex = vertex_list[i];
        MeshVertex mesh_vertex = vertices[winged_vertex.vertices_index];
        
        W_VertexList neighbor_vertex_list;
        unsigned int num_boundary_edge_neighbors = 0;
        Vector3 a, b;
        
        for (unsigned int j = 0; j < edge_list.size(); j++) {
            WingedEdge e = edge_list[j];
            WingedVertex other_v_start = vertex_list[e.start_index];
            WingedVertex other_v_end = vertex_list[e.end_index];
            if (other_v_start.vertices_index == winged_vertex.vertices_index) {
                //this edge has same start vertex
                neighbor_vertex_list.push_back(other_v_end);
                if (e.sym_index == -1) {
                    num_boundary_edge_neighbors++;
                    a = vertices[other_v_end.vertices_index].position;
                }
            } else if (other_v_end.vertices_index 
                == winged_vertex.vertices_index) {
                if (e.sym_index == -1) {
                    num_boundary_edge_neighbors++; 
                    b = vertices[other_v_start.vertices_index].position;
                }
            }
        }
        
        if (num_boundary_edge_neighbors == 2) {
            //boundary vertex
            vertex_list[i].new_pos = create_boundary_even(winged_vertex, a, b); 
        } else {
            //interior vertex
            vertex_list[i].new_pos = create_interior_even(winged_vertex, 
            neighbor_vertex_list);
        }
        
    }

    /* second time iterating through, actually update
     * the MeshVertex position for rendering
     */
    for (unsigned int i = 0; i < num_even_vertices; i++) {
        vertices[vertex_list[i].vertices_index].position = 
            vertex_list[i].new_pos;
    } 

}


} /* _462 */

