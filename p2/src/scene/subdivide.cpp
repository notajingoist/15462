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

    int num_even_vertices = vertices.size();
    
    build_adjacency_structure();
    first_pass();
    second_pass(num_even_vertices);
    
    //re-calculate normals
    has_normals = false;
    create_gl_data();

    vertex_list.clear();
    edge_list.clear();
   
    return true;
}

/**
 * Adds a new WingedEdge edge to edge_list 
 */
void Mesh::build_edge(int curr_index,
    int prev_index, int next_index, 
    int start_index, int end_index)
{
    WingedEdge e; 
    int sym_index = -1;
    for (int i = 0; i < edge_list.size(); i++) {
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

/**
 * Adds a new WingedVertex vertex to vertex_list
 */
void Mesh::build_vertex(int edge_index, int vertices_index) 
{
    WingedVertex v;
    v.edge_index = edge_index;
    v.vertices_index = vertices_index;

    vertex_list.push_back(v);
}

/**
 * Sets the edge_index of the WingedVertex vertex at the given 
 * vertices_index to be the given edge_index
 */
void Mesh::set_vertex_edge_index(int edge_index, 
    int vertices_index)
{
    vertex_list[vertices_index].edge_index = edge_index;
}

/**
 * Populates the edge_list and vertex_list using the information given by 
 * MeshTriangle triangles in triangles and the MeshVertex vertices in vertices.
 * The WingedEdge and WingedVertex representatives associate adjacency 
 * information that is helpful for the first pass and second pass subdivision.
 */
void Mesh::build_adjacency_structure()
{
    //make one-to-one mapping of MeshVertex vertices from vertices
    //to WingedVertex vertices in vertex_list
    for (int i = 0; i < vertices.size(); i++) {
        build_vertex(-1, i);
    }

    //populate edge_list with the 3 edges associated with each triangle
    //set the 3 vertices' edge indices to their corresponding edges
    for (int i = 0; i < triangles.size(); i++) {
        int i0 = triangles[i].vertices[0];
        int i1 = triangles[i].vertices[1];
        int i2 = triangles[i].vertices[2];
        int e_index = edge_list.size();
        int v_index = vertex_list.size();
   
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

/**
 * creates a new odd vertex for each edge; position depends on whether the edge
 * is interior or boundary
 */
void Mesh::add_odd_vertices() 
{
     for (int i = 0; i < edge_list.size(); i++) {
        WingedEdge e = edge_list[i];
        
        //check that an odd vertex has not already been created for this edge
        if (!e.is_subdivided) {
            
            //use the edge's endpoints as the a and b positions
            int a_index = 
                vertex_list[e.start_index].vertices_index;
            int b_index = 
                vertex_list[e.end_index].vertices_index; 
            Vector3 a = vertices[a_index].position;
            Vector3 b = vertices[b_index].position;

            MeshVertex new_mesh_vertex;
            int curr_vertices_index = vertices.size();

            if (e.sym_index != -1) {
                //this edge is an interior edge

                WingedEdge e_sym = edge_list[e.sym_index];

                //use the end vertices of this edge's next edge and 
                //the symmetric edge's previous edge as the c and d positions
                int c_index = 
                    vertex_list[edge_list[e.next_index].end_index]
                    .vertices_index;
                int d_index = 
                    vertex_list[edge_list[e_sym.next_index].end_index]
                    .vertices_index;
                Vector3 c = vertices[c_index].position;
                Vector3 d = vertices[d_index].position;
                
                //since each edge needs only to be subdivided once,
                //set the symmetric edge's odd_vertex_index and is_subdivided
                //properties
                edge_list[e.sym_index].odd_vertex_index = curr_vertices_index;
                edge_list[e.sym_index].is_subdivided = true;
                
                //apply interior-odd formula
                new_mesh_vertex.position = create_interior_odd(a, b, c, d);
            } else {
                //this edge is a boundary edge

                //apply boundary-odd formula
                new_mesh_vertex.position = create_boundary_odd(a, b);
            }
            
            //add new odd WingedVertex to vertex_list
            build_vertex(e.curr_index, curr_vertices_index);

            //set this edge's odd_vertex_index and is_subdivided properties
            edge_list[i].odd_vertex_index = curr_vertices_index;
            edge_list[i].is_subdivided = true;

            //add new odd MeshVertex vertex to vertices
            vertices.push_back(new_mesh_vertex);

        }
    }   
}

/**
 * Add a MeshTriangle triangle with the given vertex indices to triangles
 */
void Mesh::add_triangle(int v0_index, int v1_index,
    int v2_index) 
{
    MeshTriangle t;

    t.vertices[0] = v0_index;
    t.vertices[1] = v1_index;
    t.vertices[2] = v2_index;

    triangles.push_back(t);
}

/**
 * Clear the current triangles and add 4 new triangles for each previously
 * existing triangle. Find these triangles based on the adjacency info
 * for each WingedEdge in edge_list. 
 */
void Mesh::update_triangles()
{
    triangles.clear();

    for (int i = 0; i < edge_list.size(); i++) {
        WingedEdge e0 = edge_list[i]; 
        if (!e0.is_visited) {
            //triangle associated with this edge (formed by this edge, its
            //previous edge, and its next edge) has not yet been visited, i.e.,
            //the 4 new triangles that will replace this triangle have 
            //not been created yet
            WingedEdge e1 = edge_list[e0.next_index];
            WingedEdge e2 = edge_list[e0.prev_index];
           
            //obtain the 3 odd vertices associated with this triangle 
            int odd_v0 = 
                vertex_list[e0.odd_vertex_index].vertices_index;
            int odd_v1 = 
                vertex_list[e1.odd_vertex_index].vertices_index;
            int odd_v2 = 
                vertex_list[e2.odd_vertex_index].vertices_index;
       
            //obtain the 3 even vertices associated with this triangle
            int even_v0 = vertex_list[e0.start_index].vertices_index; 
            int even_v1 = vertex_list[e1.start_index].vertices_index;
            int even_v2 = vertex_list[e2.start_index].vertices_index;
    
            //add 4 new triangles 
            add_triangle(odd_v0, odd_v1, odd_v2); 
            add_triangle(odd_v2, even_v0, odd_v0);
            add_triangle(odd_v0, even_v1, odd_v1);
            add_triangle(odd_v1, even_v2, odd_v2);

            //mark all edges of this triangle as visited
            edge_list[i].is_visited = true;
            edge_list[e0.next_index].is_visited = true;
            edge_list[e0.prev_index].is_visited = true;
        }
    }
}

/**
 * first pass - create new odd vertex for each edge, and replace each 
 * triangle with 4 new triangles formed by the new odd vertices
 */
void Mesh::first_pass() 
{
    add_odd_vertices();
    update_triangles();
}

/**
 * interior-odd formula
 */
Vector3 Mesh::create_interior_odd(Vector3 a, Vector3 b,
    Vector3 c, Vector3 d)
{
    return (3.0/8.0) * (a + b) + (1.0/8.0) * (c + d);
}


/**
 * boundary-odd formula
 */
Vector3 Mesh::create_boundary_odd(Vector3 a, Vector3 b)
{
    return (1.0/2.0) * (a + b);
}

/**
 * interior-even formula
 */
Vector3 Mesh::create_interior_even(WingedVertex v, 
    W_VertexList neighbor_vertex_list) 
{
    int num_neighbors = neighbor_vertex_list.size();
    real_t squared_component = ((3.0/8.0) + ((1.0/4.0) * 
        cos((2.0*PI)/num_neighbors))); 
    real_t beta = (1.0/num_neighbors) * ((5.0/8.0) - (squared_component
        * squared_component));

    Vector3 neighbor_sum;
    for (int i = 0; i < num_neighbors; i++) {
        neighbor_sum += 
            vertices[neighbor_vertex_list[i].vertices_index].position;
    }

    Vector3 v_pos = vertices[v.vertices_index].position;
    Vector3 new_vertex = v_pos * (1.0 - (num_neighbors * beta)) 
        + (beta * neighbor_sum); 

    return new_vertex;
}

/**
 * boundary-even formula
 */
Vector3 Mesh::create_boundary_even(WingedVertex v, Vector3 a, Vector3 b) 
{
    Vector3 v_pos = vertices[v.vertices_index].position;
    return (1.0/8.0) * (a + b) + (3.0/4.0) * (v_pos);
}

/**
 * second pass - re-calculate positions for all even vertices; positions 
 * depend on whether the vertex is interior or boundary
 */
void Mesh::second_pass(int num_even_vertices)
{
    //first time iterating through, only update
    //new_pos property for each WingedVertex so that
    //calculations for each even vertex's new position are unaffected 
    for (int i = 0; i < num_even_vertices; i++) {
        WingedVertex winged_vertex = vertex_list[i];
        MeshVertex mesh_vertex = vertices[winged_vertex.vertices_index];
        
        W_VertexList neighbor_vertex_list;
        int num_boundary_edge_neighbors = 0;
        Vector3 a, b;
        
        for (int j = 0; j < edge_list.size(); j++) {
            WingedEdge e = edge_list[j];
            WingedVertex other_v_start = vertex_list[e.start_index];
            WingedVertex other_v_end = vertex_list[e.end_index];
            if (other_v_start.vertices_index == winged_vertex.vertices_index) {
                //this edge has same start vertex as the current vertex
                //therefore this edge's end index is a neighbor
                neighbor_vertex_list.push_back(other_v_end);
                if (e.sym_index == -1) {
                    //this edge is a boundary edge
                    num_boundary_edge_neighbors++;

                    //record this edge's end index as the a position
                    a = vertices[other_v_end.vertices_index].position;
                }
            } else if (other_v_end.vertices_index 
                == winged_vertex.vertices_index) {
                //this edge has same end index as the current vertex
                //but can only be in this case if this edge does not have
                //a symmetric edge, i.e., it is a boundary edge
                if (e.sym_index == -1) {
                    //double check that this edge is a boundary edge anyway
                    num_boundary_edge_neighbors++; 

                    //record this edge's start index as the b position
                    b = vertices[other_v_start.vertices_index].position;
                }
            }
        }
        
        if (num_boundary_edge_neighbors == 2) {
            //vertex lies on a boundary edge, so it is a boundary vertex
            //apply boundary-even formula to obtain new position
            vertex_list[i].new_pos = create_boundary_even(winged_vertex, a, b); 
        } else {
            //vertex lies on only interior edges, so it is an interior vertex
            //apply interior-even formula to obtain new position
            vertex_list[i].new_pos = create_interior_even(winged_vertex, 
            neighbor_vertex_list);
        }
        
    }

    //second time iterating through, actually update
    //the MeshVertex position for rendering
    for (int i = 0; i < num_even_vertices; i++) {
        vertices[vertex_list[i].vertices_index].position = 
            vertex_list[i].new_pos;
    } 

}


} /* _462 */

