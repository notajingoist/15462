varying vec3 norm;
varying vec3 cam_dir;
varying vec3 color;
uniform samplerCube tex;
// Declare any additional variables here. You may need some uniform variables.
void main(void)
{
	// Your shader code here.
	// Note that this shader won't compile since gl_FragColor is never set.

    vec3 normalized_norm = normalize(norm);
    vec3 normalized_cam = normalize(cam_dir);
    vec3 reflected_ray = reflect(normalized_cam, normalized_norm);
    
    gl_FragColor = textureCube(tex, reflected_ray);
    

}
