/**
 * @file project.cpp
 * @brief OpenGL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "p1/project.hpp"

// use this header to include the OpenGL headers
// DO NOT include gl.h or glu.h directly; it will not compile correctly.
#include "application/opengl.hpp"

// A namespace declaration. All proejct files use this namespace.
// Add this declration (and its closing) to all source/headers you create.
// Note that all #includes should be BEFORE the namespace declaration.
namespace _462 {

// definitions of functions for the OpenglProject class

// constructor, invoked when object is created
OpenglProject::OpenglProject()
{
    // TODO any basic construction or initialization of members
    // Warning: Although members' constructors are automatically called,
    // ints, floats, pointers, and classes with empty contructors all
    // will have uninitialized data!
}

// destructor, invoked when object is destroyed
OpenglProject::~OpenglProject()
{
    // TODO any final cleanup of members
    // Warning: Do not throw exceptions or call virtual functions from deconstructors!
    // They will cause undefined behavior (probably a crash, but perhaps worse).
}

/**
 * Initialize the project, doing any necessary opengl initialization.
 * @param camera An already-initialized camera.
 * @param scene The scene to render.
 * @return true on success, false on error.
 */
bool OpenglProject::initialize( Camera* camera, Scene* scene )
{
    // copy scene
    this->scene = *scene;

    mesh = this->scene.mesh;                          
    mesh_pos_data = this->scene.mesh_position;    
    mesh_pos = mesh_pos_data.position;           
    mesh_orient = mesh_pos_data.orientation;  
    mesh_scale = mesh_pos_data.scale;            
    mesh_axis;                                   
    mesh_angle;                                   
    mesh_orient.to_axis_angle(&mesh_axis, &mesh_angle);  
                                                         
    hm = this->scene.heightmap;                     
    hm_pos_data = this->scene.heightmap_position; 
    hm_pos = hm_pos_data.position;               
    hm_orient = hm_pos_data.orientation;      
    hm_scale = hm_pos_data.scale;                
    hm_axis;                                     
    hm_angle;                                     
    hm_orient.to_axis_angle(&hm_axis, &hm_angle);        

    hm_vertices = new GLdouble[3*6*RESOL*RESOL];
    hm_avg_normals = new Vector3[6*RESOL*RESOL]; 
    hm_normals = new GLdouble[3*6*RESOL*RESOL];
   
    mesh_vertices = new GLdouble[3*mesh.num_vertices];
    mesh_avg_normals = new Vector3[mesh.num_vertices];
    mesh_normals = new GLdouble[3*mesh.num_vertices];

    compute_pool();

    // TODO opengl initialization code and precomputation of mesh/heightmap
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    set_lighting();
   
    return true;
}

/**
 * Clean up the project. Free any memory, etc.
 */
void OpenglProject::destroy()
{
    // TODO any cleanup code, e.g., freeing memory
    delete[] hm_vertices;
    delete[] hm_normals;
    delete[] hm_avg_normals;
    delete[] mesh_vertices;
    delete[] mesh_normals;
    delete[] mesh_avg_normals;
}

/**
 * Perform an update step. This happens on a regular interval.
 * @param dt The time difference from the previous frame to the current.
 */
void OpenglProject::update( real_t dt )
{
    // update our heightmap
    scene.heightmap->update( dt );

    // TODO any update code, e.g. commputing heightmap mesh positions and normals
    compute_water();
    compute_height_map();
}

/**
 * Clear the screen, then render the mesh using the given camera.
 * @param camera The logical camera to use.
 * @see math/camera.hpp
 */
void OpenglProject::render( const Camera* camera )
{
    // TODO render code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* projection transformation */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera->get_fov_degrees(), camera->get_aspect_ratio(), 
        camera->get_near_clip(), camera->get_far_clip()); 
    
    /* viewing transformation */
    glMatrixMode (GL_MODELVIEW); 
    glLoadIdentity();  
    Vector3 cam_pos = camera->get_position();
    Vector3 cam_dir = camera->get_direction();
    Vector3 cam_target = cam_pos + cam_dir;
    Vector3 cam_up = camera->get_up();
    
    gluLookAt (cam_pos.x, cam_pos.y, cam_pos.z, cam_target.x, 
        cam_target.y, cam_target.z, cam_up.x, cam_up.y, cam_up.z);

    /* render pool */
    glPushMatrix();
        glTranslated(mesh_pos.x, mesh_pos.y, mesh_pos.z);
        glRotated(mesh_angle*(180/PI), mesh_axis.x, mesh_axis.y, mesh_axis.z);
        glScaled(mesh_scale.x, mesh_scale.y, mesh_scale.z);
        glColor3d(1.0, 0.2, 0.3);

        GLfloat mat_specular_pool[] = { 1.0, 1.0, 1.0, 0.0 };
        GLfloat mat_shininess_pool[] = { 80.0 };
        GLfloat mat_diffuse_pool[] = { 0.1, 0.0, 0.5, 1.0 };
        GLfloat mat_ambient_pool[] = { 0.7, 0.7, 0.7, 1.0 };

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_pool);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_pool);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_pool);     
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_pool);

        glVertexPointer(3, GL_DOUBLE, 0, mesh_vertices);
        glNormalPointer(GL_DOUBLE, 0, mesh_normals);
        for (unsigned int i = 0; i < mesh.num_triangles; i++) {
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 
                mesh.triangles[i].vertices);
        }
    glPopMatrix();

    /* render waves */
    glPushMatrix();
        glTranslated(hm_pos.x, hm_pos.y, hm_pos.z);
        glRotated(hm_angle*(180/PI), hm_axis.x, hm_axis.y, hm_axis.z);
        glScaled(hm_scale.x, hm_scale.y, hm_scale.z); 
        glColor3d(0.2, 0.2, 1.0);
           
        GLfloat mat_specular_water[] = { 1.0, 1.0, 1.0, 0.0 };
        GLfloat mat_shininess_water[] = { 60.0 };

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_water);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_water);
     
        glVertexPointer(3, GL_DOUBLE, 0, hm_vertices);
        glNormalPointer(GL_DOUBLE, 0, hm_normals);
        glDrawArrays(GL_TRIANGLES, 0, 6*RESOL*RESOL);
    glPopMatrix();

    glFlush();
}

/**
 * Adds lighting to the scene (ambient, diffuse, specular).
 */
void OpenglProject::set_lighting() 
{
    glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0);    
    glEnable(GL_DEPTH_TEST);    
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 0.9, 0.8, -0.3, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 
}

/**
 * Computes the vertices used to create the triangle mesh
 * for the water. Uses the y values calculated with compute
 * height (and the x, z values within the (-1, -1) to (1, 1)
 * range. 
 *
 * Computes normals for each vertex of the triangle mesh.
 */
void OpenglProject::compute_height_map()
{
    for (unsigned int i = 0; i < 6*RESOL*RESOL; i++) {
        hm_avg_normals[i] = Vector3(0, 0, 0);
    }

    for (unsigned int x = 0; x < (RESOL-1); x++) {            
        for (unsigned int z = 0; z < (RESOL-1); z++) {        
            Vector3 v0 = xyz_vectors[x][z];            
            Vector3 v1 = xyz_vectors[x][z+1];          
            Vector3 v2 = xyz_vectors[x+1][z];          
            Vector3 v3 = xyz_vectors[x+1][z+1];        
            unsigned int start = 3*3*2*((x*(RESOL-1))+z);     
            hm_vertices[start] = v0.x;
            hm_vertices[start+1] = v0.y;
            hm_vertices[start+2] = v0.z;
            hm_vertices[start+3] = v1.x;
            hm_vertices[start+4] = v1.y;
            hm_vertices[start+5] = v1.z;
            hm_vertices[start+6] = v2.x;
            hm_vertices[start+7] = v2.y;
            hm_vertices[start+8] = v2.z;
                                                       
            hm_vertices[start+9] = v2.x;
            hm_vertices[start+10] = v2.y;
            hm_vertices[start+11] = v2.z;
            hm_vertices[start+12] = v1.x;
            hm_vertices[start+13] = v1.y;
            hm_vertices[start+14] = v1.z;
            hm_vertices[start+15] = v3.x;
            hm_vertices[start+16] = v3.y;
            hm_vertices[start+17] = v3.z;

            unsigned int norm_start = 6*((x*(RESOL-1))+z);
            Vector3 v012_v1v0 = v1 - v0; 
            Vector3 v012_v2v0 = v2 - v0;
            Vector3 v012_norm = cross(v012_v1v0, v012_v2v0);
            hm_avg_normals[norm_start] += v012_norm;
            hm_avg_normals[norm_start+1] += v012_norm;
            hm_avg_normals[norm_start+2] += v012_norm;        

            Vector3 v213_v1v2 = v1 - v2;
            Vector3 v213_v3v2 = v3 - v2;
            Vector3 v213_norm = cross(v213_v1v2, v213_v3v2);
            hm_avg_normals[norm_start+3] += v213_norm;
            hm_avg_normals[norm_start+4] += v213_norm;
            hm_avg_normals[norm_start+5] += v213_norm;

        }
    }

    for (unsigned int i = 0; i < 6*RESOL*RESOL; i++) {
        hm_avg_normals[i] = normalize(hm_avg_normals[i]);
        hm_normals[3*i] = hm_avg_normals[i].x;
        hm_normals[3*i+1] = hm_avg_normals[i].y;
        hm_normals[3*i+2] = hm_avg_normals[i].z;
    }

}

/**
 * Computes the y values at each point in the (-1, -1) to
 * (1, 1) x-z grid by calling the heightmap's compute
 * height function. Saves the corresponding (x, y, z) 
 * tuple as a vector.
 */
void OpenglProject::compute_water()
{
    for (unsigned int i = 0; i < RESOL; i++) {                            
        for (unsigned int j = 0; j < RESOL; j++) { 
            GLdouble x_pos = ((i-(RESOLD/2.0))/(RESOLD/2.0));
            GLdouble z_pos = ((j-(RESOLD/2.0))/(RESOLD/2.0));
            GLdouble y_pos = hm->compute_height(Vector2(x_pos, z_pos));
            xyz_vectors[i][j] = Vector3(x_pos, y_pos, z_pos);          
        }
    }
}

/**
 * Unpacks the vertices into x, y, z coordinates to be used
 * for rendering the pool with OpenGL vertex arrays.
 *
 * Computes the normals for the pool mesh. 
 */
void OpenglProject::compute_pool() 
{
    for (unsigned int i = 0; i < mesh.num_vertices; i++) {
        mesh_vertices[3*i] = mesh.vertices[i].x;
        mesh_vertices[3*i+1] = mesh.vertices[i].y;
        mesh_vertices[3*i+2] = mesh.vertices[i].z;
        
        mesh_avg_normals[i] = Vector3(0, 0, 0);

        for (unsigned int j = 0; j < mesh.num_triangles; j++) {
            
            if (i == mesh.triangles[j].vertices[0]
                || i == mesh.triangles[j].vertices[1]
                || i == mesh.triangles[j].vertices[2]) {
                Vector3 v0 = mesh.vertices[mesh.triangles[j].vertices[0]];
                Vector3 v1 = mesh.vertices[mesh.triangles[j].vertices[1]];
                Vector3 v2 = mesh.vertices[mesh.triangles[j].vertices[2]];
                Vector3 v1v0 = v1 - v0;
                Vector3 v2v0 = v2 - v0;
                Vector3 norm = cross(v1v0, v2v0);
                mesh_avg_normals[i] += norm;                     
            }
        }
        
        mesh_avg_normals[i] = normalize(mesh_avg_normals[i]);
        mesh_normals[3*i] = mesh_avg_normals[i].x;
        mesh_normals[3*i+1] = mesh_avg_normals[i].y;
        mesh_normals[3*i+2] = mesh_avg_normals[i].z;
    }


}

} /* _462 */
