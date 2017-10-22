#version 440

uniform vec3 u_light_dir;
uniform vec3 u_color;

in vec3 v_nrm;

out vec4 frag_colour;

void main()
{
    float diffuse = max( dot( u_light_dir, v_nrm ), 0.0 );
    frag_colour = vec4( diffuse * u_color, 1.0 );
}
