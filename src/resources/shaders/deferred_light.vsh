#version 440

in vec3 a_pos;
in vec2 a_tex;

out vec2 v_tex_coords;

void main()
{
    v_tex_coords = a_tex;
    gl_Position = vec4( a_pos, 1.0 );
}
