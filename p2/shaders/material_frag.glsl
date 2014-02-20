varying vec3 norm;
varying vec3 cam_dir;
varying vec3 color;
uniform samplerCube tex;

void main(void)
{
    vec3 normalized_norm = normalize(norm);
    vec3 normalized_cam = normalize(cam_dir);
    vec3 reflected_ray = reflect(normalized_cam, normalized_norm);
    
    gl_FragColor = textureCube(tex, reflected_ray);
}
