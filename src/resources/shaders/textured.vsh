#version 440

uniform mat4 u_mvp;

in vec3 a_vp;
in vec2 a_tex;
out vec2 tex_coord;

void main()
{
    gl_Position = u_mvp * vec4( a_vp, 1.0 );
    tex_coord = a_tex;
}
